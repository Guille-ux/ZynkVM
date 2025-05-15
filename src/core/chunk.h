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

#ifndef _ZYNK_CHUNK_H
#define _ZYNK_CHUNK_H

#include "../common/common.h"
#include "types/value.h"

typedef struct { //oh algo dinámico
    size_t count;
    size_t capacity;
    uint8_t* code;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk) { 
    chunk->count=0;
    chunk->capacity=0;
    chunk->code=(uint8_t *)NULL;
    initArray(&chunk->constants);
}

void free_chunk(ArenaManager *manager, Chunk *chunk) {
    sysarena_free(manager, chunk->code);
    init_chunk(chunk);
    freeArray(manager, &chunk->constants);
}

int addConstant(ArenaManager *manager, Chunk *chunk, Value value) {
    writeArray(manager, &chunk->constants, value);
    return chunk->constants.count - 1;
}

void cinit_sys(ArenaManager manager, uint8_t *memoryl, Arena *arenis, size_t size, size_t arena_count) {
    sysarena_init(&manager, memoryl, arenis, size, arena_count);
}

void writeChunk(ArenaManager *manager, Chunk *chunk, uint8_t byte) {
    if (chunk->count >= chunk->capacity) {
        size_t old = chunk->capacity;
        uint8_t *new_code = NULL;

        if (old == 0) {
            new_code = sysarena_alloc(manager, INITIAL);
            chunk->capacity = INITIAL;
        } else {
            new_code = sysarena_alloc(manager, old * GROW_FACTOR);
            tmemcpy(new_code, chunk->code, old);
            chunk->capacity *= GROW_FACTOR;
        }

        chunk->code = new_code;
        sysarena_free(manager, chunk->code);
    }

    chunk->code[chunk->count]=byte;
    chunk->count++;
}

#endif