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

#ifndef _MARENA_H
#define _MARENA_H

#include "../types.h"
#include "mem_structs.h"
#include "farena.h"

MMarena new_marena(uint8_t *memory, uint32_t size, uint32_t n_arenas) {
    MMarena n_marena;
    n_marena.size = size;
    n_marena.n_arenas = n_arenas;
    n_marena.arenas_size = size / n_arenas;
    n_marena.memory = memory;
    MemArena* arenas;
    finit_arena_mem(n_arenas, arenas, memory, size);
    return n_marena;
}
void init_marena(MMarena *n_marena, uint8_t *memory, uint32_t size, uint32_t n_arenas) {
    n_marena->size = size;
    n_marena->n_arenas = n_arenas;
    n_marena->arenas_size = size / n_arenas;
    n_marena->memory = memory;
    MemArena* arenas;
    finit_arena_mem(n_arenas, arenas, memory, size);
}


void freem(uint8_t *pt, MMarena *memall) {
    ffreef(pt, memall->n_arenas, memall->arenas);
}

void fmalloc(uint32_t size, MMarena *memall) {
    fallocate(size, memall->n_arenas, memall->arenas);
}

#endif