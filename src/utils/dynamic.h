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

#ifndef _ZYNK_DYNAMIC_H
#define _ZYNK_DYNAMIC_H

#include "types.h"
#include "mems/arena.h"

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