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

#ifndef _FARENA_H
#define _FARENA_H

#include "../types.h"
#include "mem_structs.h"
//this is a more customizable version of arena

void finit_arena(MemArena *arena, int nblock, uint8_t *big, uint32_t arena_size) {
    arena->size = arena_size;
    arena->used = 0;
    arena->mem = &big[nblock*arena_size];
}

void finit_arena_mem(uint32_t n_arenas, MemArena *memory) {
    for (int i=0; i<n_arenas; i++) {
        finit_arena(&memory[i], i);
    }
}

MemArena* fget_aval(uint32_t size, uint32_t n_arenas, MemArena *memory) { //MemArena no esta mal, no mlo probe pero creo que funcionara
    for (int i=0;i<n_arenas; i++) {
        if (memory[i].size - memory[i].used >= size) {
            MemArena *ptr = &memory[i];
            return ptr;
        }
    }
}

void* fallocate(uint32_t size, uint32_t n_arenas, MemArena *memory) {
    MemArena *ptr = fget_aval(size, n_arenas, memory);
    void *pt = &ptr->mem[ptr->used];
    return pt;
}

void freef(MemArena *arena) {
    arena->used=0;
}

MemArena* ffrom_where(uint8_t *pt, uint32_t n_arenas, MemArena *memory) {
    for (int i = 0; i < n_arenas; i++) {
        uint8_t *start = memory[i].mem;
        uint8_t *end = start + memory[i].size;
        if (pt >= start && pt < end) {
            return &memory[i];
        }
    }
    return NULL;
}

void ffreef(uint8_t *pt, uint32_t n_arenas, MemArena *memory) {
    MemArena *arena = ffrom_where(pt, n_arenas, memory);
    if (arena != NULL) {
        freef(arena);
    }
}

uint32_t ftotal_used_mem(MemArena *memory, uint32_t n_arenas) {
    uint32_t sum = 0;
    for (int i = 0; i < n_arenas; i++) {
        sum += memory[i].used;
    }
    return sum;
}



#endif