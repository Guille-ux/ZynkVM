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
#include "../ext/ext.h"
#include "types/value.h"
#include "types/object.h"

#define FREE(manager, type, pointer) reallocate(manager, pointer, sizeof(type), 0)
#define FREE_ARR(manager, type, pointer, length) reallocate(manager, pointer, sizeof(type)*length, 0)

typedef struct {
    Chunk* chunk;
    uint8_t *ip; // ip means instruction pointer
    Value stack[STACK_MAX];
    Value* stackTop;
    Obj* objects;
} ZynkVM;

typedef enum {
    ZYNK_OK,
    ZYNK_COMPILE_ERROR,
    ZYNK_RUNTIME_ERROR,
} ZynkResult;

void load_chunk(ArenaManager *manager, uint8_t *code, Value *constants, Chunk *chunk, size_t len) {
    init_chunk(chunk);
    for (size_t i=0;i<len;i++) {
        writeChunk(manager, chunk, code[i], i);
        if (code[i]==OP_CONSTANT) {
            size_t index = addConstant(manager, chunk, constants[chunk->constants.count]);
            uint8_t tmp[sizeof(common_size)];
#ifndef BIG_ENDIAN
            storeSizeIn8LEndian((common_size *)index, tmp);
#else
            storeSizeIn8BEndian((common_size *)index, tmp);
#endif
            for (char z=0;z<sizeof(common_size);z++) {
                writeChunk(manager, chunk, tmp[z], i);
            }
        }
    }
}

Value compareVals(Value a, Value b) {
    if (SAME_TYPE(a, b)) {
        if (IS_BOOL(a)) {
            if (AS_BOOL(a)==AS_BOOL(b)) {
                return BOOL_VAL(true);
            } else {
                return BOOL_VAL(false);
            }
        } else if (IS_NUMBER(a)) {
            if (AS_NUMBER(a)==AS_NUMBER(b)) {
                return BOOL_VAL(true);
            } else {
                return BOOL_VAL(false);
            }
        } else if (IS_STRING(a)) {
            return BOOL_VAL(fmemcmp((uint8_t *)AS_CSTRING(a), (uint8_t *)AS_CSTRING(b), AS_STRING(a)->length, AS_STRING(b)->length));
        } else {
            // runtimeError (Unknown Type)
        }
    } else {
        return BOOL_VAL(false);
    }
}

static ZynkResult run(ZynkVM *vm) {
#define READ() (*vm->ip++)
#ifndef BIG_ENDIAN
#define RCONSTANT() ({ \
    uint8_t *index_init = vm->ip; \
    vm->ip += sizeof(common_size); \
    common_size ret; \
    store8InSizeLEndian(index_init, &ret); \
    vm->chunk->constants.values[ret]; \
})
#else
#define RCONSTANT() ({ \
    uint8_t *index_init = vm->ip; \
    vm->ip += sizeof(common_size); \
    common_size ret; \
    store8InSizeBEndian(index_init, &ret); \
    vm->chunk->constants.values[ret]; \
})
#endif

#define BINARY(op) \
    do { \
            Value a = pop(vm); \
            Value b = pop(vm); \
            if (a.type != b.type) { \
                    /* runtimeError, they aren't the same */ \
            } else { \
                if (IS_BOOL(a)) { \
                     /* runtimeError, los booleanos no se pueden sumar */ \
                } else if (IS_NUMBER(a)) { \
                    a.as.number=AS_NUMBER(a) op AS_NUMBER(b); \
                } \
            } \
    } while (false);

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
                // temporal
                printVal(pop(vm));
                printf("\n");
                //end of temporal
                return ZYNK_OK;
            case OP_CONSTANT: {
                Value constant = RCONSTANT();
                push(vm, constant);
                break;
            }
            case OP_NEGATE: {
                Value val = pop(vm);
                if (IS_NUMBER(val)) {
                    val.as.number=-AS_NUMBER(val);
                } else if (IS_BOOL(val)) {
                    if (AS_BOOL(val)==true) {
                        val.as.boolean=false;
                    } else {
                        val.as.boolean=true;
                    }
                } else {
                    //runtimeError();
                }
                push(vm, val);
                break;
            }
            case OP_ADD:
                BINARY(+)
                break;
            case OP_SUBSTRACT:
                BINARY(-);
                break;
            case OP_DIVIDE:
                BINARY(/);
                break;
            case OP_MULTIPLY:
                BINARY(*);
                break;
            case OP_NULL:
                push(vm, NULL_VAL);
                break;
            case OP_TRUE:
                push(vm, BOOL_VAL(true));
                break;
            case OP_FALSE:
                push(vm, BOOL_VAL(false));
                break;
            case OP_EQUAL:
                push(vm, compareVals(pop(vm), pop(vm)));
                break;
            case OP_GREATER:
                Value a=pop(vm);
                Value b=pop(vm);
                if (SAME_TYPE(a, b) && IS_NUMBER(a)) {
                    push(vm, BOOL_VAL((AS_NUMBER(a) > AS_NUMBER(b))));
                }
                break;
            case OP_LESS:
                Value a=pop(vm);
                Value b=pop(vm);
                if (SAME_TYPE(a, b) && IS_NUMBER(a)) {
                    push(vm, BOOL_VAL((AS_NUMBER(a) < AS_NUMBER(b))));
                }
                break;
        }
    }
#undef BINARY
#undef RCONSTANT
#undef READ
}

static void reset_stack(ZynkVM *vm) {
    vm->stackTop=vm->stack;
}

void push(ZynkVM *vm, Value value) {
    *vm->stackTop = value;
    if ((size_t)(vm->stackTop-vm->stack)<STACK_MAX) {
        vm->stackTop++;
    } else {
        // panic(); futura función para errores
    }
}

Value pop(ZynkVM *vm) {
    if (vm->stackTop - vm->stack>0) {
        vm->stackTop--;
    } else {
        // panic(); o tambien puede ser runtimeError
    }
    return *vm->stackTop;
}

static void freeObj(ArenaManager *manager, Obj* obj) {
    switch (obj->type) {
        case ZYNK_OBJ_STRING:
            ObjString* string = (ObjString *)obj;
            FREE_ARR(manager, char, string->chars, string->length+1);
            FREE(manager, ObjString, obj);
            break;
    }
}

void freeObjs(ArenaManager *manager, ZynkVM *vm) {
    Obj *cobj = vm->objects;
    while (cobj!=null) {
        Obj *next = cobj->next;
        freeObj(manager, cobj);
        cobj=next;
    }
}

void initVM(ZynkVM *vm) {
    reset_stack(vm);
    vm->objects=(Obj *)null;
}

void freeVM(ArenaManager *manager, ZynkVM *vm) {
    freeObjs(manager, vm);
}

ZynkResult zynk_interpret(ZynkVM *vm, Chunk *chunk) {
    vm->chunk=chunk;
    vm->ip=vm->chunk->code;
    return run(vm);
}

#endif