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

#ifndef ZYNK_VALUE_H
#define ZYNK_VALUE_H

#include "../../common/common.h"
#define DOUBLE_SIZE 8

typedef enum {
    ZYNK_BOOL,
    ZYNK_NUM,
    ZYNK_NULL,
    ZYNK_BYTE,
} ZynkType;

typedef struct {
    ZynkType type;
    union {
        bool boolean;
        double number;
        ptr_t pointer;
        char byte;
    } as;
} Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

#define BOOL_VAL(value)   ((Value){ZYNK_BOOL, {.boolean = value}})
#define NULL_VAL          ((Value){ZYNK_NULL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){ZYNK_NUM, {.number = value}})
#define BYTE_VAL(value)   ((Value){ZYNK_BYTE, {.byte = value}})
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)
#define AS_BYTE(value)    ((value).as.byte)
#define IS_BOOL(value)    ((value).type == ZYNK_BOOL)
#define IS_NULL(value)    ((value).type == ZYNK_NULL)
#define IS_NUMBER(value)  ((value).type == ZYNK_NUM)
#define IS_BYTE(value)    ((value).type == ZYNK_BYTE)
#define SAME_TYPE(a, b)   ((a).type==(b).type)

void initArray(ValueArray *array) {
    array->values=(uint8_t *)null;
    array->count=0;
    array->capacity=0;
}

void writeArray(ArenaManager *manager, ValueArray *array, Value value) {
    if (array->capacity < array->count + 1) {
        size_t old_cap = array->capacity;
        if (old_cap==0) {
            array->values=reallocate(manager, array->values, old_cap, INITIAL);
            array->capacity=INITIAL;
        } else {
            size_t new_cap = old_cap*GROW_FACTOR;
            array->values=reallocate(manager, array->values, old_cap, new_cap);
        }
    }
    array->values[array->count] = value;
    array->count++;
}

void freeArray(ArenaManager *manager, ValueArray *array) {
    sysarena_free(manager, array->values);
    initArray(array);
}


#ifndef STANDALONE
void printVal(Value val) {
    printf("%g", val);
}
#endif

#endif

