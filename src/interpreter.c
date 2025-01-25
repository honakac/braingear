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

#include "include/interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bf_run(unsigned char* bytecode)
{
    struct {
        char buffer[MEMORY_SIZE];
        unsigned short index;
    } memory;
    memset(memory.buffer, 0, sizeof(memory.buffer) - 1);
    memory.index = 0;

    for (unsigned char *ptr = bytecode+1; *ptr != (unsigned char) 255; ptr++) {
        unsigned char ch = *ptr;

        switch (ch)
        {
            case '>': memory.index += *(++ptr); break;
            case '<': memory.index -= *(++ptr); break;
            case '+': memory.buffer[memory.index] += *(++ptr); break;
            case '-': memory.buffer[memory.index] -= *(++ptr); break;
            case '.': putchar(memory.buffer[memory.index]); break;
            case ',': memory.buffer[memory.index] = getchar(); break;
            case '[':
                if (memory.buffer[memory.index] == 0) {
                    size_t brackets = 1;
                    do {
                        ptr++;
                        if (*ptr == '[') brackets++;
                        else if (*ptr == ']') brackets--;
                    } while (brackets > 0 && *ptr != 0xFF);
                }
                break;
            case ']':
                if (memory.buffer[memory.index] != 0) {
                    size_t brackets = 1;
                    do {
                        ptr--;
                        if (*ptr == ']') brackets++;
                        else if (*ptr == '[') brackets--;
                    } while (brackets > 0 && *ptr != 0xFF);
                }
                break;
        }
    }

    free(bytecode);
}