#include "include/vm.h"
#include "glib.h"
#include <strings.h>
#include "include/instructions.h"

Memory mem;

void vm_init(VM *vm)
{
    mem.pos = 0;
    vm->instructions = g_array_new(TRUE, TRUE, sizeof(Instruction));
    memset(mem.buffer, 0, sizeof(mem.buffer));
}

void vm_optimize(VM *vm, char *code)
{
    size_t i = 0;

    for (char *ptr = code; *ptr; ptr++, i++) {
        char ch = *ptr;

        Instruction ins;
        ins.command = ch;
        ins.arg = 0;

        arg_type count = 0;
        switch (ch) {
            case '[': case ']':
                if (*ptr == ']' && *(ptr-1) == '-' && *(ptr-2) == '[') {
                    vm->instructions->len -= 2;

                    ins.command = '=';
                    ins.arg = 0;
                }
                break;

            case '+': case '-':
                count = 0;
                for (; *ptr; ptr++) {
                    if (*ptr == '+')      count++;
                    else if (*ptr == '-') count--;
                    else                  break;
                }
                ptr--;

                ins.arg = count;
                break;
            case '>': case '<':
                count = 0;
                for (; *ptr; ptr++) {
                    if (*ptr == '>')      count++;
                    else if (*ptr == '<') count--;
                    else                  break;
                }
                ptr--;

                ins.arg = count;
                break;
        }

        g_array_append_val(vm->instructions, ins);
    }

    for (size_t i = 0; i < vm->instructions->len; i++) {
        Instruction *ins = &g_array_index(vm->instructions, Instruction, i);

        arg_type count = 0;
        arg_type shift = 0;
        switch (ins->command) {
            case '[': case ']':
                count = 1;
                shift = 0;

                do {
                    shift += (ins->command == '[' ? 1 : -1);

                    if (i + shift >= vm->instructions->len || i + shift < 0)
                        break;

                    char cur = g_array_index(vm->instructions, Instruction, i + shift).command;
                    if      (cur == (ins->command == '[' ? '[' : ']')) count++;
                    else if (cur == (ins->command == '[' ? ']' : '[')) count--;
                } while (count > 0
                         && i + shift < vm->instructions->len
                         && i + shift > 0);

                (&g_array_index(vm->instructions, Instruction, i))->arg = i + shift;
                break;
        }
    }
}

void vm_run(VM vm)
{
    Memory *mem = &vm.mem;

    GArray* instructions = vm.instructions;
    Instruction* ins_arr = (Instruction*)instructions->data;
    size_t len = instructions->len;
    
    for (mem->index = 0; mem->index < len; mem->index++) {
        Instruction ins = ins_arr[mem->index];
        // printf("%lu %c %lu\n", vm.memory.index, ins.command, mem, ins.arg);
        // ins.func(mem, ins.arg);

        switch (ins.command) {
            case '+': vi_add(mem, ins.arg); break;
            case '-': vi_add(mem, ins.arg); break;
            case '>': vi_move(mem, ins.arg); break;
            case '<': vi_move(mem, ins.arg); break;
            case ',': vi_input(mem, ins.arg); break;
            case '.': vi_output(mem, ins.arg); break;
            case '[': vi_jumpifzero(mem, ins.arg); break;
            case ']': vi_jumpifnotzero(mem, ins.arg); break;
            case '=': vi_set(mem, ins.arg); break;
        }
    }
}

void vm_compile(VM vm, char *outputFile)
{
    FILE *file = fopen(outputFile, "w");
    if (!file) {
        printf("Failed to open file: %s\n", outputFile);
        return;
    }

    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <string.h>\n");
    fprintf(file, "int main() {\n");
    fprintf(file, "char buffer[65540];\n");
    fprintf(file, "unsigned short i = 0;\n");
    fprintf(file, "memset(buffer, 0, sizeof(buffer));\n");

    for (size_t i = 0; i < vm.instructions->len; i++) {
        Instruction ins = g_array_index(vm.instructions, Instruction, i);

        switch (ins.command) {
            case '+': case '-': fprintf(file, "buffer[i] += %d;\n", (int) ins.arg); break;
            case '>': case '<': fprintf(file, "i += %d;\n", (int) ins.arg); break;
            case ',': fprintf(file, "buffer[i] = getchar();\n"); break;
            case '.': fprintf(file, "putchar(buffer[i]);\n"); break;
            case '[': fprintf(file, "while (buffer[i] != 0) {\n"); break;
            case ']': fprintf(file, "}\n"); break;
            case '=': fprintf(file, "buffer[i] = %d;\n", (char) ins.arg); break;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
}