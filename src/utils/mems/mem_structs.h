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

#ifndef _MEM_STRUCTS_H
#define _MEM_STRUCTS_H

#include "../types.h"

typedef struct {
    uint8_t *mem; // puntero al bloque principal
    uint32_t size; // tamaño
    uint32_t used; // parte usada
} MemArena;

//blocks for the future

typedef struct {
    uint32_t size;
    uint32_t n_arenas;
    uint32_t arenas_size;
    uint8_t *memory;
    MemArena *arenas;
} MMarena; //Marena Manager

typedef struct MemBlock {
    uint8_t value;
    struct MemBlock *next;
    Bool free;
} MemBlock;

typedef struct {
    uint32_t size;
    MemBlock *free_list;
} BlockManager;


#endif