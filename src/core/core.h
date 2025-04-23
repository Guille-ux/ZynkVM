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

#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define MEM_SIZE 1024*1024*100
#define N_BLOCKS 25 //para tener 4 megas por arena

uint8_t mem[MEM_SIZE];
MMarena morena; //es random el nombre

void init_marena_system() {
    init_marena(&morena, &mem, MEM_SIZE, N_BLOCKS);
}

void init_chunk(Chunk *chunk) { 
    chunk->count=0;
    chunk->capacity=0;
    chunk->code=NULL;
}

void writeChunk(Chunk *chunk, uint8_t byte) {
    if (chunk->count >= chunk->capacity) {
        uint32_t old = chunk->capacity;
        if (old==0) {
            uint8_t *n_code = fmalloc(INITIAL, &morena);
        } else {
            uint8_t *n_code = fmalloc(old*GROW_FACTOR, &morena);
        }
        tmemcpy(n_code, chunk->code, old);
        chunk->code=n_code;

    }
    chunk->code[chunk->count]=byte;
}



#endif