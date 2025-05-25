/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* any later version.
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

#ifndef ZYNK_COMMON_H
#define ZYNK_COMMON_H

#ifndef STANDALONE
#include <stdio.h>
#endif

#include "../.conduitpkg/sysarena/src/include/types.h"
#include "../.conduitpkg/sysarena/src/include/sysarena.h"
#include "core/tmem.h"

#define EIGHT_TO_SIZE sizeof(size_t)
#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define MEM_SIZE 1024*1024 // memoria ram que tendra 1M para la prueba más tarde sera 1024*1024*100 es decir, 100 megas
#define ARENA_COUNT 1024 * 1024 / 4
#define ARENA_SIZE (MEM_SIZE / ARENA_COUNT)
#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2
#define STACK_MAX 1024*1024 //limite actual del stack creo que 1M es suficiente de momento, en el futuro sera usando sysarena

typedef uint32_t common_size;

void *reallocate(ArenaManager *manager, void *pointer, size_t old_size, size_t new_size);

void store8InSizeLEndian(uint8_t *source, common_size *dest);

void storeSizeIn8LEndian(common_size *source, uint8_t *dest);

void store8InSizeBEndian(uint8_t *source, common_size *dest);

void storeSizeIn8BEndian(common_size *source, uint8_t *dest);


#endif