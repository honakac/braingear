#pragma once

#include "vm.h"
#include <stdint.h>

inline static void vi_set(Memory *mem, arg_type arg) {
    mem->buffer[mem->pos] = (int8_t)arg;
}

inline static void vi_add(Memory *mem, arg_type arg) {
    mem->buffer[mem->pos] += (int8_t)arg;
}

inline static void vi_move(Memory *mem, arg_type arg) {
    mem->pos += arg;
}

inline static void vi_input(Memory *mem, arg_type arg) {
    int c = getchar();
    if(c != EOF) 
        mem->buffer[mem->pos] = (int8_t)c;
    else
        mem->buffer[mem->pos] = 0;
}
inline static void vi_output(Memory *mem, arg_type arg) {
    putchar(mem->buffer[mem->pos]);
}

inline static void vi_jumpifzero(Memory *mem, arg_type arg) {
    if (mem->buffer[mem->pos] == 0) {
        mem->index = arg;
    }
}
inline static void vi_jumpifnotzero(Memory *mem, arg_type arg) {
    if (mem->buffer[mem->pos] != 0) {
        mem->index = arg;
    }
}