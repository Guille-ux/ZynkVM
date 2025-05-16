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

#ifndef ZYNK_VM_H
#define ZYNK_VM_H

#include "chunk.h"
#include "opcodes.h"
#include "../common/common.h"


typedef struct {
    Chunk* chunk;
    uint8_t *ip; // ip means instruction pointer
    Value stack[STACK_MAX];
    Value* stackTop;
} ZynkVM;

typedef enum {
    ZYNK_OK,
    ZYNK_COMPILE_ERROR,
    ZYNK_RUNTIME_ERROR,
} ZynkResult;

static ZynkResult run(ZynkVM *vm) {
#define READ() (*vm->ip++)
#define RCONSTANT() (vm->chunk->constants.values[READ()])

    for (;;) { //anything useful
#ifdef DEBUG
        printf("\t");
        for (Value* slot=vm->stack; slot<vm->stackTop; slot++) {
            printf("[ ");
            printVal(*slot);
            printf(" ]");
        }
        printf("\n")
        disassemble_instruction(vm->chunk, size_t (size_t)(vm->ip-vm->chunk->code))
#endif
        uint8_t instruction;
        switch (instruction=READ()) {
            case OP_RETURN:
                return ZYNK_OK;
            case OP_CONSTANT:
                Value constant = RCONSTANT();
                
        }
    }
#undef RCONSTANT
#undef READ
}

static void reset_stack(ZynkVM *vm) {
    vm->stackTop=vm->stack;
}

void push(ZynkVM *vm, Value value) {
    *vm->stackTop = value;
    vm->stackTop++;
}

Value pop(ZynkVM *vm) {
    vm->stackTop--;
    return *vm->stackTop;
}

void initVM(ZynkVM *vm) {
    reset_stack(vm);
}

void freeVM(ZynkVM *vm) {

}

ZynkResult zynk_interpret(ZynkVM *vm, Chunk *chunk) {
    vm->chunk=chunk;
    vm->ip=vm->chunk->code;
    return run(vm);
}

#endif