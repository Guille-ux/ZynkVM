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
    sysarena_init(&manager, memory, arenas, ARENA_SIZE, ARENA_COUNT);
}

void cinit_sys(ArenaManager manager, uint8_t *memoryl, Arena *arenis, uint32_t size, uint32_t arena_count) {
    sysarena_init(&manager, memoryl, arenis, size, arena_count);
}

void writeChunk(Chunk *chunk, uint8_t byte) {
    if (chunk->count >= chunk->capacity) {
        uint32_t old = chunk->capacity;
        uint8_t *new_code = NULL;

        if (old == 0) {
            new_code = sysarena_alloc(&manager, INITIAL);
            chunk->capacity = INITIAL;
        } else {
            new_code = sysarena_alloc(&manager, old * GROW_FACTOR);
            tmemcpy(new_code, chunk->code, old);
            chunk->capacity *= GROW_FACTOR;
        }

        chunk->code = new_code;
        sysarena_free(&manager, chunk->code);
    }

    chunk->code[chunk->count]=byte;
    chunk->count++;
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