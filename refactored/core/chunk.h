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
    size_t *lines;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunk(ArenaManager *manager, Chunk *chunk);

size_t addConstant(ArenaManager *manager, Chunk *chunk, Value value);

void cinit_sys(ArenaManager manager, uint8_t *memoryl, Arena *arenis, size_t size, size_t arena_count);

void writeChunk(ArenaManager *manager, Chunk *chunk, uint8_t byte, size_t line);

#endif