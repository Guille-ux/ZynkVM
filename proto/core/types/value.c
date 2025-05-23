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

#include "value.h"

static Obj* allocateObj(ZynkVM *vm, ArenaManager *manager, size_t size, ObjType ot) {
    Obj* object = (Obj *)reallocate(manager, null, 0, size);
    object->type = ot;
    object->next = vm->objects;
    vm->objects = object;
    return object;
}

static ObjString* allocateStr(ZynkVM *vm, ArenaManager *manager, char *chars, uint32_t length) {
    ObjString* string = ALLOC_OBJ(vm, manager, ObjString, ZYNK_OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString* cpyString(ZynkVM *vm, ArenaManager *manager, const char *chars, uint32_t length) {
    char *heapChars = (char *)reallocate(manager, null, 0, sizeof(char)*(length+1));
    tmemcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateStr(vm, manager, heapChars, length);
}

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

void initArray(ValueArray *array) {
    array->values=(Value *)null;
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
    printf("%g", AS_NUMBER(val));
}

void printZynkObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case ZYNK_OBJ_STRING:
      printf("%s", AS_CSTRING(value));
      break;
  }
}

#endif