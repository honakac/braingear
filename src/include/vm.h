#pragma once

#include <glib.h>
#include <stdio.h>
#include <stddef.h>

typedef long arg_type;

typedef struct {
    char buffer[65540];
    unsigned short pos;
    size_t index;
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
void vm_optimize(VM *vm, char *code);
void vm_run(VM vm);
void vm_compile(VM vm, char *outputFile);