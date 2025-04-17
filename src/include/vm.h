#pragma once

#include <glib.h>
#include <stdio.h>
#include <stddef.h>

#define MEMORY_SIZE 65540

enum {
    IT_NONE = 0,
    IT_ADD = '+',
    IT_MOVE = '>',
    IT_INPUT = ',',
    IT_OUTPUT = '.',
    IT_JZ = '[',
    IT_JNZ = ']',
    IT_SET = '=',
    IT_ZERO = '0',
    IT_CLEAR_MOVE = '#'
};

typedef signed long arg_type;

typedef struct {
    char *buffer;
    unsigned short pos;
} Memory;

typedef struct {
    char command;
    arg_type arg;
} Instruction;

typedef struct {
    GArray *instructions;
    Memory mem;
} VM;

void vm_init(VM *vm);
void vm_optimize(VM *vm, const char *code);
void vm_run(VM *vm);
void vm_compile(VM *vm, const char *outputFile);