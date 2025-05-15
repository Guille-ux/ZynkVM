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

#ifndef _ZYNK_COMMON_H
#define _ZYNK_COMMON_H

#include "../utils/types.h"
#include "../utils/tmem.h"
#include "../../.conduitpkg/sysarena/src/include/sysarena.h"

#define INITIAL 8 //cuando le falte memoria añadira uno más, asi gastare menos
#define GROW_FACTOR 2

uint8_t *reallocate(ArenaManager *manager, uint8_t *pointer, size_t old_size, size_t new_size) {
    if (new_size==0) {
        sysarena_free(manager, pointer); // liberar bloque
        return (uint8_t *)NULL;
    }
    uint8_t *reallocated = (uint8_t *)sysarena_alloc(manager, new_size);
    tmemcpy(reallocated, pointer, old_size);
    sysarena_free(manager, pointer);
    return reallocated;
}

#endif