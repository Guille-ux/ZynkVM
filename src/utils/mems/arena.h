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

#define MEM_SIZE ((1024)*(1024))*100 //100 megas de memoria
#define N_ARENAS 128 // no se, algo
#define ARENA_SIZE ((MEM_SIZE)/(N_ARENAS))

typedef struct {
    uint8_t mem[ARENA_SIZE]; // puntero al bloque principal
    uint32_t size; // tamaño
    uint32_t used; // parte usada
} MemArena;

MemArena memory[N_ARENAS];

void init_arena(MemArena *arena) {
    arena->size = ARENA_SIZE;
    arena->used = 0;
}

void init_arena_mem() {
    for (int i=0; i<N_ARENAS; i++) {
        init_arena(&memory[i]);
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

void free(MemArena *arena, uint32_t size) {
    if (size > arena->size) {}
    else {
        arena->used -= size;
    }
}


#endif