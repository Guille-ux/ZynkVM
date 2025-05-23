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

#ifndef ZYNK_RUNTIME
#define ZYNK_RUNTIME

#include "../common.h"
#include "opcodes.h"
#include "value.h"
#include "chunk.h"

#define FREE(manager, type, pointer) reallocate(manager, pointer, sizeof(type), 0)
#define FREE_ARR(manager, type, pointer, length) reallocate(manager, pointer, sizeof(type)*length, 0)

void load_chunk(ArenaManager *manager, uint8_t *code, Value *constants, Chunk *chunk, size_t len);

Value compareVals(Value a, Value b);

ZynkResult run(ArenaManager *manager, ZynkVM *vm);

static void reset_stack(ZynkVM *vm);

void push(ZynkVM *vm, Value value);

Value pop(ZynkVM *vm);

static void freeObj(ArenaManager *manager, Obj* obj);

void freeObjs(ArenaManager *manager, ZynkVM *vm);

void initVM(ZynkVM *vm);

void freeVM(ArenaManager *manager, ZynkVM *vm);

ZynkResult zynk_interpret(ArenaManager *manager, ZynkVM *vm, Chunk *chunk);

 #endif