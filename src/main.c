/*
 * BrainGear - A Brainfuck compiler/interpreter implemented in C.
 * Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "include/interpreter.h"
#include "include/compiler.h"
#include "include/vm.h"

#define PAGE_SIZE 512

void usage(const char* prog)
{
    fprintf(stderr, "Usage: %s <compile|run> <input .bf file> [output .c file]\n", prog);
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc < 3)
        usage(argv[0]);

    unsigned char *bytecode = vm_optimize_file(argv[2]);

    if (!strcmp(argv[1], "run"))
        bf_run(bytecode);
    else if (!strcmp(argv[1], "compile")) {
        if (argc != 4)
            usage(argv[0]);
        
        bf_compile(bytecode, argv[3]);
    }
    else
        usage(argv[0]);

    return 0;
}