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
#include "../vm.h"
#include "object.h"
#define DOUBLE_SIZE 8

typedef enum {
    ZYNK_BOOL,
    ZYNK_NUM,
    ZYNK_NULL,
    ZYNK_BYTE,
    ZYNK_OBJ,
    ZYNK_VAR,
} ZynkType;

typedef struct {
    ZynkType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
        char byte;
        char var[8];
    } as;
} Value;

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

static Obj* allocateObj(ZynkVM *vm, ArenaManager *manager, size_t size, ObjType ot);

#define BOOL_VAL(value)   ((Value){ZYNK_BOOL, {.boolean = value}})
#define NULL_VAL          ((Value){ZYNK_NULL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){ZYNK_NUM, {.number = value}})
#define BYTE_VAL(value)   ((Value){ZYNK_BYTE, {.byte = value}})
#define OBJ_VAL(object)   ((Value){ZYNK_OBJ, {.obj = (Obj*)object}})
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)
#define AS_BYTE(value)    ((value).as.byte)
#define AS_OBJ(value)     ((value).as.obj)
#define IS_BOOL(value)    ((value).type == ZYNK_BOOL)
#define IS_NULL(value)    ((value).type == ZYNK_NULL)
#define IS_NUMBER(value)  ((value).type == ZYNK_NUM)
#define IS_BYTE(value)    ((value).type == ZYNK_BYTE)
#define IS_OBJ(value)     ((value).type == ZYNK_OBJ)
#define IS_STRING(value)  isObjType(value, ZYNK_OBJ_STRING)
#define SAME_TYPE(a, b)   ((a).type==(b).type)
#define OBJ_TYPE(object)  (AS_OBJ(object)->type)
#define AS_STRING(value)  ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define ALLOC_OBJ(vm, manager, ty, ot) \
    (ty*)allocateObj(vm, manager, sizeof(ty), ot) \

static ObjString* allocateStr(ZynkVM *vm, ArenaManager *manager, char *chars, uint32_t length);

ObjString* cpyString(ZynkVM *vm, ArenaManager *manager, const char *chars, uint32_t length);

static inline bool isObjType(Value value, ObjType type);

void initArray(ValueArray *array);

void writeArray(ArenaManager *manager, ValueArray *array, Value value);

void freeArray(ArenaManager *manager, ValueArray *array);


#ifndef STANDALONE
void printVal(Value val);

void printZynkObject(Value value);

#endif

#endif

