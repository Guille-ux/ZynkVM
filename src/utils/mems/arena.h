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

#ifndef _ARENA_H
#define _ARENA_H

#include "../types.h"
#include "mem_structs.h"

#define MEM_SIZE ((1024)*(1024))*100 //100 megas de memoria
#define N_ARENAS 128 // no se, algo
#define ARENA_SIZE ((MEM_SIZE)/(N_ARENAS))


MemArena memory[N_ARENAS];
uint8_t big[MEM_SIZE];

void init_arena(MemArena *arena, int nblock) {
    arena->size = ARENA_SIZE;
    arena->used = 0;
    arena->mem = &big[nblock*ARENA_SIZE];
}

void init_arena_mem() {
    for (int i=0; i<N_ARENAS; i++) {
        init_arena(&memory[i], i);
    }
}

MemArena* get_aval(uint32_t size) { //MemArena no esta mal, no mlo probe pero creo que funcionara
    for (int i=0;i<N_ARENAS; i++) {
        if (memory[i].size - memory[i].used >= size) {
            MemArena *ptr = &memory[i];
            return ptr;
        }
    }
}

void* allocate(uint32_t size) {
    MemArena *ptr = get_aval(size);
    void *pt = &ptr->mem[ptr->used];
    return pt;
}

void free(MemArena *arena) {
    arena->used=0;
}

MemArena* from_where(uint8_t *pt) {
    for (int i = 0; i < N_ARENAS; i++) {
        uint8_t *start = memory[i].mem;
        uint8_t *end = start + memory[i].size;
        if (pt >= start && pt < end) {
            return &memory[i];
        }
    }
    return (MemArena *)NULL;
}

void ffree(uint8_t *pt) {
    MemArena *arena = from_where(pt);
    if (arena != NULL) {
        free(arena);
    }
}

uint32_t total_used_mem() {
    uint32_t sum = 0;
    for (int i = 0; i < N_ARENAS; i++) {
        sum += memory[i].used;
    }
    return sum;
}

void clean(MemArena *arena) {
    uint8_t *start_point = &arena->mem[arena->used];
    uint32_t used = arena->used;
    uint32_t size = ARENA_SIZE - used;
    for (uint32_t i=used; i < ARENA_SIZE; i++) {
        arena->mem[i] = 0;
    }
}

void eclean(MemArena *arena) {
    for (uint32_t i=0; i<ARENA_SIZE; i++) {
        arena->mem[i]=0;
    }
}

#endif