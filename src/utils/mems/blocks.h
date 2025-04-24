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
#include "stdio.h" //quitar luego

void init_block_manager(BlockManager *manager, MemBlock *memory, uint32_t size) {
    manager->size = size;
    manager->free_list = memory;
}

void init_block(MemBlock *block, BlockManager *manager, uint32_t pos) {
    block->free=true;
    block->value=0;
    block->next=(MemBlock *)NULL;
}

void init_blocks(BlockManager *manager) {
    for (uint32_t i=0; i<manager->size;i++) {
        init_block(&manager->free_list[i], manager, i);
    }
}

void *locate_free(BlockManager *manager) {
    for (uint32_t i=0; i<manager->size;i++) {
        if (manager->free_list[i].free==true) {
            return &manager->free_list[i];
        }
    }
    return NULL;
}

void *allocate_block(BlockManager *manager, uint32_t size) { // es simple, el sistema hace horrible manejar la memoria luego, pero elimina la fragmentación
    MemBlock *prev=(MemBlock *)NULL;
    MemBlock *start=(MemBlock *)NULL;
    MemBlock *current;
    uint32_t found=0;
    MemBlock *n;
    for (uint32_t i=0;i<manager->size&&found<size;i++) {
        n=&manager->free_list[i];
        if (n->free==true) {
            current=n;
            if (start==NULL) {
                start=current;
            }
            current->free=false;//asegurarnos de que no esta libre
            current->value = 0;//limpiando el valor
            if (prev!=NULL) {
                // xD, lo habia puesto al reves → current->next=prev;
                prev->next=current;
            }
            prev = (MemBlock *)n;
            found++;
        }

    }
    if (found < size) {
        //insuficiente memoria
        return NULL;
    } else {
        return start;
    }
}

void free_block(MemBlock *ptr) {
    while (1) {
        if (ptr==NULL) {
            break;
        }
        ptr->free=true;
        ptr=ptr->next;
    }
}

#endif