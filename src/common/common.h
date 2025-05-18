/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
/* 
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/* GNU General Public License for more details.
/* 
/* You should have received a copy of the GNU General Public License
/* along with this program. If not, see <https://www.gnu.org/licenses/>.
/* 
/* Copyright (c) 2025 Guillermo Leira Temes
/* */

#ifndef _ZYNK_COMMON_H
#define _ZYNK_COMMON_H

#include "../../.conduitpkg/sysarena/src/include/sysarena.h"
#include "../utils/types.h"
#include "../utils/tmem.h"

#define EIGHT_TO_SIZE sizeof(size_t)
#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define MEM_SIZE 1024*1024 // memoria ram que tendra 1M para la prueba más tarde sera 1024*1024*100 es decir, 100 megas
#define ARENA_COUNT 1024 * 1024 / 4
#define ARENA_SIZE (MEM_SIZE / ARENA_COUNT)
#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define STACK_MAX 256 //limite actual del stack

typedef uint32_t common_size;

void *reallocate(ArenaManager *manager, void *pointer, size_t old_size, size_t new_size) {
    if (new_size==0) {
        sysarena_free(manager, pointer); // liberar bloque
        return null;
    }
    uint8_t *reallocated = sysarena_alloc(manager, new_size);
    tmemcpy(reallocated, (uint8_t *)pointer, old_size);
    sysarena_free(manager, pointer);
    return (ptr_t)reallocated;
}

void store8InSizeLEndian(uint8_t *source, common_size *dest) { // i need this for the stack
    for (char i=0;i<sizeof(common_size);i++) {
        *dest|=(common_size)source[i] << (8*i);
    }
}

void storeSizeIn8LEndian(common_size *source, uint8_t *dest) { // the same, but turned
    for (char i=0;i<sizeof(common_size);i++) {
        dest[i]=(uint8_t)(*source >> (i*8))&0xFF;
    }
}

void store8InSizeBEndian(uint8_t *source, common_size *dest) { // i need this for the stack
    for (char i=0;i<sizeof(common_size);i++) {
        *dest|=(common_size)source[i] << (8*(sizeof(common_size)-i-1));
    }
}

void storeSizeIn8BEndian(common_size *source, uint8_t *dest) { // the same, but turned
    for (char i=0;i<sizeof(common_size);i++) {
        dest[i]=(uint8_t)(*source >> (8*(sizeof(common_size)-i-1)))&0xFF;
    }
}

#endif
