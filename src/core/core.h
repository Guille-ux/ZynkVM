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

#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define MEM_SIZE 1024*1024 // memoria ram que tendra 1M para la prueba más tarde sera 1024*1024*100 es decir, 100 megas


MemBlock block_list[MEM_SIZE];
BlockManager manager;

void init_sys() {
    init_block_manager(&manager, block_list, 0);
    init_blocks(&manager);
    // más cosas
}

MemBlock* gotoret(MemBlock *code, uint32_t index) {
    MemBlock *next = code;
    for (uint32_t i = 0; i < index; i++) {
        if (next->next == NULL) return NULL;
        next = next->next;
    }
    return next;
}


void writeChunk(Chunk *chunk, uint8_t byte) {
    if (chunk->count >= chunk->capacity) {
        uint32_t old = chunk->capacity;
        MemBlock *new_code = NULL;

        if (old == 0) {
            new_code = allocate_block(&manager, INITIAL);
            chunk->capacity = INITIAL;
        } else {
            new_code = allocate_block(&manager, old * GROW_FACTOR);
            blocpy(new_code, chunk->code, old);
            chunk->capacity *= GROW_FACTOR;
        }

        chunk->code = new_code;
    }

    MemBlock *target = gotoret(chunk->code, chunk->count);
    if (target != NULL) {
        target->value = byte;
        chunk->count++;
    }
}


MemBlock *reallocate_block(MemBlock *pointer, uint32_t old_size, uint32_t new_size) {
    if (new_size==0) {
        free_block(pointer); // liberar bloque
        return (MemBlock *)NULL;
    }
    MemBlock *reallocated = (MemBlock *)allocate_block(&manager, new_size);
    blocpy(reallocated, pointer, old_size);
    free_block(pointer);
    return reallocated;
}


#endif