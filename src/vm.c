#include "include/vm.h"
#include "glib.h"
#include <strings.h>
#define different_signs(a, b) ((a > 0 && b < 0) || (a < 0 && b > 0))

void vm_init(VM *vm)
{
    vm->instructions = g_array_new(TRUE, TRUE, sizeof(Instruction));
}

void vm_detect_brackets(VM *vm)
{
    for (size_t i = 0; i < vm->instructions->len; i++) {
        Instruction ins = g_array_index(vm->instructions, Instruction, i);

        arg_type count = 0;
        arg_type shift = 0;
        switch (ins.command) {
            case IT_JZ: case IT_JNZ:
                count = 1;
                shift = 0;

                do {
                    shift += (ins.command == IT_JZ ? 1 : -1);

                    if (i + shift >= vm->instructions->len || (signed long)(i + shift) < 0) {
                        fprintf(stderr, "Invalid jump\n");
                        exit(1);
                    }

                    char cur = g_array_index(vm->instructions, Instruction, i + shift).command;
                    if      (cur == (ins.command == IT_JZ ? IT_JZ  : IT_JNZ)) count++;
                    else if (cur == (ins.command == IT_JZ ? IT_JNZ : IT_JZ)) count--;
                } while (count > 0
                         && i + shift < vm->instructions->len
                         && i + shift > 0);

                (&g_array_index(vm->instructions, Instruction, i))->arg = i + shift;
                break;
            default:
                break;
        }

        ins = g_array_index(vm->instructions, Instruction, i);
    }
}

void vm_optimize(VM *vm, const char *code)
{
    for (char *ptr = (char*) code; *ptr; ptr++) {
        char ch = *ptr;

        Instruction ins;
        ins.command = IT_NONE;
        ins.arg = 0;

        arg_type count = 0;
        switch (ch) {
            case '[': case ']':
                ins.command = ch;

                if (*ptr == ']' && *(ptr-1) == '-' && *(ptr-2) == '[') {
                    vm->instructions->len -= 2;

                    ins.command = IT_ZERO;
                    ins.arg = 0;
                }
                break;

            case '+': case '-':
                for (; *ptr; ptr++) {
                    if (*ptr == '+')      count++;
                    else if (*ptr == '-') count--;
                    else                  break;
                }
                ptr--;

                ins.command = count == 0 ? IT_NONE 
                                         : IT_ADD;
                ins.arg = count;
                break;
            case '>': case '<': {
                for (; *ptr; ptr++) {
                    if (*ptr == '>')      count++;
                    else if (*ptr == '<') count--;
                    else                  break;
                }
                ptr--;

                ins.command = count == 0 ? IT_NONE 
                                         : IT_MOVE;
                ins.arg = count;
                break;
            }
            case '.':
                ins.command = IT_OUTPUT;
                break;
            case ',':
                ins.command = IT_INPUT;
                break;
        }

        if (ins.command != IT_NONE)
            g_array_append_val(vm->instructions, ins);
    }
    for (size_t i = 0; i < vm->instructions->len; i++) {
        Instruction ins = g_array_index(vm->instructions, Instruction, i);

        arg_type count = 0;
        switch (ins.command) {
            case IT_ZERO: {
                size_t start = i;
                count = 0;

                while (g_array_index(vm->instructions, Instruction, i  ).command == IT_ZERO
                    && g_array_index(vm->instructions, Instruction, i+1).command == IT_MOVE
                    && g_array_index(vm->instructions, Instruction, i+1).arg == 1) {
                    count++;
                    i+=2;
                }
                
                if (count > 0) {
                    if (g_array_index(vm->instructions, Instruction, i).command == IT_ZERO) {
                        g_array_remove_range(vm->instructions, start + 2, count * 2 - 2);

                        *(&g_array_index(vm->instructions, Instruction, start+1)) = (Instruction) {
                            .command = IT_ZERO,
                            .arg = 0
                        };

                        i = start+1;
                    } else {
                        g_array_remove_range(vm->instructions, start + 1, count * 2 - 1);

                        i = start;
                    }

                    *(&g_array_index(vm->instructions, Instruction, start)) = (Instruction) {
                        .command = IT_CLEAR_MOVE,
                        .arg = count
                    };

                }
                break;
            }
            case IT_MOVE:
                if (g_array_index(vm->instructions, Instruction, i+1).command == IT_ADD
                 && g_array_index(vm->instructions, Instruction, i+2).command == IT_MOVE
                 && different_signs(ins.arg, g_array_index(vm->instructions, Instruction, i+2).arg)
                 && labs(ins.arg) == labs(g_array_index(vm->instructions, Instruction, i+2).arg)) {
                    *(&g_array_index(vm->instructions, Instruction, i)) = (Instruction) {
                        .command = IT_MOVE_ADD,
                        .arg = ins.arg,
                        .arg2 = g_array_index(vm->instructions, Instruction, i+1).arg
                    };
                    g_array_remove_range(vm->instructions, i + 1, 2);
                }
                break;
            case IT_JZ:
                // это пиздецкое условие для оптимизации циклов. тут можно было бы сделать лучше, но я не хочу это делать
                if (g_array_index(vm->instructions, Instruction, i+1).command == IT_ADD
                 && g_array_index(vm->instructions, Instruction, i+1).arg == -1
                 && g_array_index(vm->instructions, Instruction, i+2).command == IT_MOVE
                 && g_array_index(vm->instructions, Instruction, i+3).command == IT_ADD
                 && g_array_index(vm->instructions, Instruction, i+3).arg > 0
                 && g_array_index(vm->instructions, Instruction, i+4).command == IT_MOVE
                 && g_array_index(vm->instructions, Instruction, i+5).command == IT_JNZ
                 && labs(g_array_index(vm->instructions, Instruction, i+2).arg) == labs(g_array_index(vm->instructions, Instruction, i+4).arg)
                 && different_signs(g_array_index(vm->instructions, Instruction, i+2).arg, g_array_index(vm->instructions, Instruction, i+4).arg)) {
                    *(&g_array_index(vm->instructions, Instruction, i)) = (Instruction) {
                        .command = IT_FOLD_LOOP,
                        .arg = g_array_index(vm->instructions, Instruction, i+2).arg,
                        .arg2 = g_array_index(vm->instructions, Instruction, i+3).arg
                    };
                    g_array_remove_range(vm->instructions, i + 1, 5);
                }
                break;
        }
    }

    vm_detect_brackets(vm);
}

void vm_run(VM *vm)
{
    Memory mem;
    mem.buffer = (char*)malloc(MEMORY_SIZE);
    memset(mem.buffer, 0, MEMORY_SIZE);
    mem.pos = 0;

    GArray* instructions = vm->instructions;
    Instruction* ins_arr = (Instruction*)instructions->data;
    size_t len = instructions->len;
    
    int temp = 0;
    for (size_t ip = 0; ip < len; ip++) {
        Instruction ins = ins_arr[ip];

        switch (ins.command) {
            case IT_ADD:
                mem.buffer[mem.pos] += (int8_t)ins.arg;
                break;
            case IT_MOVE:
                mem.pos += ins.arg;
                break;

            case IT_INPUT:
                if((temp = getchar()) != EOF) 
                    mem.buffer[mem.pos] = (int8_t)temp;
                else
                    mem.buffer[mem.pos] = 0;
                break;
            case IT_OUTPUT:
                putchar(mem.buffer[mem.pos]);
                break;

            case IT_JZ:
                if (mem.buffer[mem.pos] == 0)
                    ip = ins.arg;
                break;
            case IT_JNZ:
                if (mem.buffer[mem.pos] != 0)
                    ip = ins.arg;
                break;

            case IT_SET:
                mem.buffer[mem.pos] = (int8_t)ins.arg;
                break;
            case IT_ZERO:
                mem.buffer[mem.pos] = 0;
                break;

            case IT_CLEAR_MOVE:
                memset(mem.buffer + mem.pos, 0, ins.arg);
                mem.pos += ins.arg;
                break;
            
            case IT_MOVE_ADD:
                mem.buffer[mem.pos += ins.arg] += ins.arg2;
                mem.pos -= ins.arg;
                break;

            case IT_FOLD_LOOP:
                mem.buffer[mem.pos + ins.arg] += mem.buffer[mem.pos] * ins.arg2;
                mem.buffer[mem.pos] = 0;
                break;

            default:
                break;
        }
    }

    free(mem.buffer);
}

void vm_compile(VM *vm, const char *outputFile)
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

    size_t len = vm->instructions->len;
    for (size_t i = 0; i < len; i++) {
        Instruction ins = g_array_index(vm->instructions, Instruction, i);

        switch (ins.command) {
            case IT_NONE:             break;
            case IT_ADD:              fprintf(file, "buffer[i] += %d;\n", (int) ins.arg); break;
            case IT_MOVE:             fprintf(file, "i += %d;\n", (int) ins.arg); break;
            case IT_INPUT:            fprintf(file, "buffer[i] = getchar();\n"); break;
            case IT_OUTPUT:           fprintf(file, "putchar(buffer[i]);\n"); break;
            case IT_JZ:               fprintf(file, "while (buffer[i] != 0) {\n"); break;
            case IT_JNZ:              fprintf(file, "}\n"); break;
            case IT_SET:              fprintf(file, "buffer[i] = %d;\n", (char) ins.arg); break;
            case IT_ZERO:             fprintf(file, "buffer[i] = 0;\n"); break;

            case IT_CLEAR_MOVE:       fprintf(file, "memset(buffer + i, 0, %ld); i += %ld;\n", ins.arg, ins.arg); break;
            case IT_MOVE_ADD:         fprintf(file, "buffer[i += %ld] += %ld; i -= %ld;\n", ins.arg, ins.arg2, ins.arg); break;
            case IT_FOLD_LOOP:        fprintf(file, "buffer[i + %ld] += buffer[i] * %ld; buffer[i] = 0;\n", ins.arg, ins.arg2); break;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
}