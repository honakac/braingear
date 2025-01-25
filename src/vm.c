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

#include "include/vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 512
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    unsigned char *buffer;
    size_t size;
    size_t pos;
} Memory;

static void push_byte(Memory *mem, unsigned char byte)
{
    mem->buffer[mem->pos++] = byte;
    mem->buffer[mem->pos] = 0;
    
    if (mem->pos >= mem->size - PAGE_SIZE)
        mem->buffer = realloc(mem->buffer, (mem->size += PAGE_SIZE));
    
    if (mem->buffer == NULL) perror("vm: memory allocation failed");

    // printf("Push byte: 0x%02x (%c)\n", byte, byte);
}

static long long get_repeat(FILE *fp, char byte)
{
    long long repeatCount = 1;

    char newByte = 0;
    for (;(newByte = fgetc(fp)) != EOF && newByte == byte;)
            repeatCount++;
    
    if (newByte != EOF)
        fseek(fp, -1, SEEK_CUR);
    
    return repeatCount;
}

unsigned char *vm_optimize_file(char *file)
{
    Memory code = { malloc(PAGE_SIZE), PAGE_SIZE, 0 };
    if (code.buffer == NULL) perror("vm: memory allocation failed");

    memset(code.buffer, 0, code.size-1);
    
    FILE *fp = fopen(file, "r");
    if (fp == NULL) perror("fopen");

    push_byte(&code, 255);

    for (char byte = 0; (byte = fgetc(fp)) != EOF;) {
        switch (byte) {
            case '+': case '-': case '>': case '<':
                long long repeat = get_repeat(fp, byte);

                do {
                    push_byte(&code, byte);
                    push_byte(&code, MIN(254, repeat));
                    repeat -= 254;
                } while (repeat > 254);
                break;
            case '.': case ',': case '[': case ']':
                push_byte(&code, byte);
                break;
        }
    }
    
    push_byte(&code, 255);
    push_byte(&code, 255);

    fclose(fp);

    return code.buffer;
}