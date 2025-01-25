/**
 * BrainGear - A Brainfuck compiler/interpreter implemented in C.
 * Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "include/compiler.h"
#include "include/interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bf_compile(unsigned char *bytecode, const char *filepath)
{
    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) perror("compiler: failed opening output file");

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "#include <string.h>\n\n");
    fprintf(fp, "int main()\n{\n");
    fprintf(fp, "    unsigned short i = 0;\n");
    fprintf(fp, "    char m[%d];\n", MEMORY_SIZE);
    fprintf(fp, "    memset(m, 0, sizeof(m));\n");
    
    for (unsigned char *ptr = bytecode+1; *ptr; ptr++) {
        switch (*ptr) {
            case '+': fprintf(fp, "    m[i] += %d;\n", (unsigned int) *(++ptr)); break;
            case '-': fprintf(fp, "    m[i] -= %d;\n", (unsigned int) *(++ptr)); break;
            case '<': fprintf(fp, "    i -= %d;\n", (unsigned int) *(++ptr)); break;
            case '>': fprintf(fp, "    i += %d;\n", (unsigned int) *(++ptr)); break;
            case '.': fprintf(fp, "    putchar(m[i]);\n"); break;
            case ',': fprintf(fp, "    m[i] = getchar();\n"); break;
            case '[': fprintf(fp, "    while (m[i]) {\n"); break;
            case ']': fprintf(fp, "    }\n"); break;
        }
    }

    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");

    fclose(fp);
    free(bytecode);
}