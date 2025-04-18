#include "include/vm.h"
#include <stdio.h>
#include <glib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s COMMAND OPTIONS\n", argv[0]);
        printf("Commands:\n");
        printf("    compile <file.bf> <file.c>  - Compiles a Brainfuck file into C code\n");
        printf("    run     <file.bf>           - Runs a Brainfuck file\n");
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        printf("Failed to open file: %s\n", argv[2]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    GString *data = g_string_sized_new(fsize); // Предвыделение памяти
    g_string_append_len(data, (gchar*)malloc(fsize), fsize); // Чтение за один вызов
    fread(data->str, 1, fsize, file);
    fclose(file);

    VM vm;
    vm_init(&vm);
    vm_optimize(&vm, data->str);

    if (strcmp(argv[1], "run") == 0)
        vm_run(&vm);
    else if (strcmp(argv[1], "compile") == 0) {
        if (argc != 4) {
            printf("Usage: %s compile <file.bf> <file.c>\n", argv[0]);
            return 1;
        }

        vm_compile(&vm, argv[3]);
    }

    g_string_free(data, TRUE);
    g_array_free(vm.instructions, TRUE);
    return 0;
}