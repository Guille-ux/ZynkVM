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


#ifndef _ZYNK_CORE_H
#define _ZYNK_CORE_H

#include "../common/common.h"
#include "chunk.h"
#include "opcodes.h"
#include "debug.h"

#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define MEM_SIZE 1024*1024 // memoria ram que tendra 1M para la prueba más tarde sera 1024*1024*100 es decir, 100 megas
#define ARENA_COUNT 1024 * 1024 / 4
#define ARENA_SIZE (MEM_SIZE / ARENA_COUNT)

Arena arenas[ARENA_COUNT];
uint8_t memory[MEM_SIZE];
ArenaManager manager;

void init_sys() {
    cinit_sys(manager, memory, arenas, MEM_SIZE, ARENA_COUNT);
}

uint8_t *reallocate_block(uint8_t *pointer, uint32_t old_size, uint32_t new_size) {
    if (new_size==0) {
        free_block(pointer); // liberar bloque
        return (uint8_t *)NULL;
    }
    uint8_t *reallocated = (uint8_t *)sysarena_alloc(&manager, new_size);
    tmemcpy(reallocated, pointer, old_size);
    sysarena_free(&manager, pointer);
    return reallocated;
}

#endif