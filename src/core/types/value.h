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

#ifndef ZYNK_VALUE_H
#define ZYNK_VALUE_H

#include "../../common/common.h"
#define DOUBLE_SIZE 8

typedef double Value;

typedef struct {
    int capacity;
    int count;
    MemBlock* values;
} ValueArray;

Bool write_double(BlockManager *manager, MemBlock *start, double value) {
    uint8_t *value_ptr = (uint8_t *)&value;
    MemBlock *current = start;
    for (int i = 0; i < DOUBLE_SIZE; i++) {
        if (current == NULL) {
            return false;
        }
        current->value = value_ptr[i];
        current = current->next;
    }
    return true;
}

double read_double(MemBlock *start) {
    uint8_t temp[DOUBLE_SIZE];
    MemBlock *current = start;
    for (int i = 0; i < DOUBLE_SIZE; i++) {
        if (current == NULL) {

            return 0.0;
        }
        temp[i] = current->value;
        current = current->next;
    }
    double ret;
    uint64_t bits;
    for (int i=0; i< DOUBLE_SIZE;i++) {
        bits |= ((uint64_t)temp[i] << (i * 8));

    }
    ret = *(double *)bits;
    return ret;
}


#endif