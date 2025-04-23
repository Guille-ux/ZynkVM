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

#ifndef _BLOCKS_H
#define _BLOCKS_H

#include "../types.h"
#include "mem_structs.h"

void init_block_manager(BlockManager *manager, uint8_t *memory, uint32_t size) {
    manager->pool=memory;
    manager->size = size;
    manager->free_list = (MemBlock*)memory;
    manager->free_list->size = size;
    manager->free_list->next = NULL;
    manager->free_list->is_free = 1;
}

void *allocate_block(BlockManager *manager, uint32_t size) {
    //implementar
}


void free_block(BlockManager *manager, void *ptr) {
    //implementar
}

#endif