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
#include "object.h"
#define DOUBLE_SIZE 8

typedef enum {
    ZYNK_BOOL,
    ZYNK_NUM,
    ZYNK_NULL,
    ZYNK_BYTE,
    ZYNK_OBJ,
} ZynkType;

typedef struct {
    ZynkType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
        char byte;
    } as;
} Value;

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

static Obj* allocateObj(ArenaManager *manager, size_t size, ObjType ot) {
    Obj* object = (Obj *)reallocate(manager, null, 0, size);
    object->type = ot;
    return object;
}

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
#define ALLOC_OBJ(manager, ty, ot) \
    (ty*)allocateObj(manager, sizeof(ty), ot) \

static ObjString* allocateStr(ArenaManager *manager, char *chars, uint32_t length) {
    ObjString* string = ALLOC_OBJ(manager, ObjString, ZYNK_OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString* cpyString(ArenaManager *manager, const char *chars, uint32_t length) {
    char *heapChars = (char *)reallocate(manager, null, 0, sizeof(char)*(length+1));
    tmemcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateStr(manager, heapChars, length);
}

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

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

void printZynkObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case ZYNK_OBJ_STRING:
      printf("%s", AS_CSTRING(value));
      break;
  }
}

#endif

#endif

