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

#include "vm.h"
#include "value.h"

#ifndef STANDALONE
#include <stdio.h>
#include "debug.h"
#endif


void load_chunk(ArenaManager *manager, uint8_t *code, Value *constants, Chunk *chunk, size_t len) {
    init_chunk(chunk);
    size_t constants_idx = 0;
    for (size_t i = 0; i < len;) {
#ifndef STANDALONE
        printf("[LOADING INSTRUCTION NUMBER %ld...]\n", i);
        printf("[DATA] [CONSTANT INDEX %d] [INSTRUCTION CODE %d]\n", chunk->constants.count, code[i]);
#endif
        writeChunk(manager, chunk, code[i], i);
        if (code[i] == OP_CONSTANT) {
            // Add the constant value and get its index
            writeArray(manager, &chunk->constants, constants[constants_idx]);
            size_t const_idx = chunk->constants.count - 1;
            constants_idx++;
            
            // Write the index bytes
            uint8_t tmp[sizeof(common_size)];
#ifndef BIG_ENDIAN
            storeSizeIn8LEndian((common_size *)&const_idx, tmp);
#else
            storeSizeIn8BEndian((common_size *)&const_idx, tmp);
#endif

#ifndef STANDALONE
            printf("[Conversion Successful]\n");
#endif
            for (size_t z = 0; z < sizeof(common_size); z++) {
                i++;
                writeChunk(manager, chunk, tmp[z], i);
            }
        }
        i++;
    }
}

Value compareVals(Value a, Value b) {
    if (SAME_TYPE(a, b)) {
        if (IS_BOOL(a)) {
            return BOOL_VAL(AS_BOOL(a) == AS_BOOL(b));
        } else if (IS_NUMBER(a)) {
            return BOOL_VAL(AS_NUMBER(a) == AS_NUMBER(b));
        } else if (IS_STRING(a)) {
            return BOOL_VAL(fmemcmp((uint8_t *)AS_CSTRING(a), 
                                  (uint8_t *)AS_CSTRING(b), 
                                  AS_STRING(a)->length, 
                                  AS_STRING(b)->length));
        } else if (IS_NULL(a)) {
            return BOOL_VAL(true); // NULL == NULL
        }
        return NULL_VAL; // Unknown type comparison
    }
    return BOOL_VAL(false); // Different types
}

static Value read_constant(ZynkVM *vm, bool *error) {
    uint8_t *index_init = vm->ip;
    vm->ip += sizeof(common_size);
    common_size ret;
#ifndef BIG_ENDIAN
    store8InSizeLEndian(index_init, &ret);
#else
    store8InSizeBEndian(index_init, &ret);
#endif
    if (ret >= vm->chunk->constants.count) {
        *error = true;
        return NULL_VAL;
    }
    *error = false;
    return vm->chunk->constants.values[ret];
}

ZynkResult run(ArenaManager *manager, ZynkVM *vm) {
#define READ() (*vm->ip++)
#ifndef BIG_ENDIAN
#define RCONSTANT() ({ \
    uint8_t *index_init = vm->ip; \
    vm->ip += sizeof(common_size); \
    common_size ret; \
    store8InSizeLEndian(index_init, &ret); \
    if (ret >= vm->chunk->constants.count) { \
        vm->ip = vm->chunk->code + vm->chunk->count; /* Force exit */ \
        Value error_val = {.type = ZYNK_NULL}; \
        return error_val; \
    } \
    vm->chunk->constants.values[ret]; \
})
#else
#define RCONSTANT() ({ \
    uint8_t *index_init = vm->ip; \
    vm->ip += sizeof(common_size); \
    common_size ret; \
    store8InSizeBEndian(index_init, &ret); \
    if (ret >= vm->chunk->constants.count) { \
        vm->ip = vm->chunk->code + vm->chunk->count; /* Force exit */ \
        Value error_val = {.type = ZYNK_NULL}; \
        return error_val; \
    } \
    vm->chunk->constants.values[ret]; \
})
#endif

#define BINARY_ADD 1
#define BINARY_SUB 2
#define BINARY_MUL 3
#define BINARY_DIV 4

#define BINARY(optype) \
    do { \
            Value b = pop(vm); \
            Value a = pop(vm); \
            if (IS_NULL(a) || IS_NULL(b)) { \
                push(vm, NULL_VAL); \
            } else if (a.type != b.type) { \
                return ZYNK_RUNTIME_ERROR; \
            } else if (IS_BOOL(a)) { \
                return ZYNK_RUNTIME_ERROR; \
            } else if (IS_NUMBER(a)) { \
                double result; \
                switch(optype) { \
                    case BINARY_ADD: result = AS_NUMBER(a) + AS_NUMBER(b); break; \
                    case BINARY_SUB: result = AS_NUMBER(a) - AS_NUMBER(b); break; \
                    case BINARY_MUL: result = AS_NUMBER(a) * AS_NUMBER(b); break; \
                    case BINARY_DIV: \
                        if (AS_NUMBER(b) == 0) return ZYNK_RUNTIME_ERROR; \
                        result = AS_NUMBER(a) / AS_NUMBER(b); \
                        break; \
                } \
                push(vm, NUMBER_VAL(result)); \
            } else if (IS_STRING(a) && optype == BINARY_ADD) { \
                return ZYNK_RUNTIME_ERROR; \
            } else { \
                return ZYNK_RUNTIME_ERROR; \
            } \
    } while (false);

    for (;;) { //anything useful
#ifdef DEBUG
#include <stdio.h>
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
            case OP_RETURN: {
                // temporal
                printVal(pop(vm));
                printf("\n");
                //end of temporal
                return ZYNK_OK;
            }
            case OP_CONSTANT: {
                bool error;
                Value constant = read_constant(vm, &error);
                if (error) return ZYNK_RUNTIME_ERROR;
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
            case OP_ADD: {
                BINARY(BINARY_ADD)
                break;
            }
            case OP_SUBSTRACT: {
                BINARY(BINARY_SUB);
                break;
            }
            case OP_MULTIPLY: {
                BINARY(BINARY_MUL);
                break;
            }
            case OP_DIVIDE: {
                BINARY(BINARY_DIV);
                break;
            }
            case OP_NULL: {
                push(vm, NULL_VAL);
                break;
            }
            case OP_TRUE: {
                push(vm, BOOL_VAL(true));
                break;
            }
            case OP_FALSE: {
                push(vm, BOOL_VAL(false));
                break;
            }
            case OP_EQUAL: {
                push(vm, compareVals(pop(vm), pop(vm)));
                break;
            }
            case OP_GREATER: {
                Value a=pop(vm);
                Value b=pop(vm);
                if (SAME_TYPE(a, b) && IS_NUMBER(a)) {
                    push(vm, BOOL_VAL((AS_NUMBER(a) > AS_NUMBER(b))));
                }
                break;
            }
            case OP_LESS: {
                Value a=pop(vm);
                Value b=pop(vm);
                if (SAME_TYPE(a, b) && IS_NUMBER(a)) {
                    push(vm, BOOL_VAL((AS_NUMBER(a) < AS_NUMBER(b))));
                }
                break;
            }
            case OP_BYTE: {
                Value a=BYTE_VAL(READ());
                push(vm, a);
                break;
            }
            case OP_STRING: {
                uint32_t length = (uint32_t)(AS_NUMBER(pop(vm)));
                Value str = OBJ_VAL(cpyString(vm, manager, (const char *)READ(), length));
                push(vm, str);
                for (uint32_t i=1;i<length;i++) {
                    READ();
                }
                break;
            }
            case OP_DEL: {
                pop(vm);
                break;
            }
            case OP_DUPE: {
                Value a=pop(vm);
                push(vm, a);
                push(vm, a);
                break;
            }
            case OP_BACKDEL: {
                Value a=pop(vm);
                pop(vm);
                push(vm, a);
                break;
            }
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
    if ((size_t)(vm->stackTop - vm->stack) >= STACK_MAX) {
#ifndef STANDALONE
        fprintf(stderr, "Error: Stack overflow\n");
#endif
        return;
    }
    *vm->stackTop = value;
    vm->stackTop++;
}

Value pop(ZynkVM *vm) {
    if (vm->stackTop - vm->stack <= 0) {
        // Return NULL value for empty stack
        return NULL_VAL;
    }
    vm->stackTop--;
    return *vm->stackTop;
}

void freeObj(ArenaManager *manager, Obj* obj) {
    switch (obj->type) {
        case ZYNK_OBJ_STRING: {
            ObjString* string = (ObjString *)obj;
            FREE_ARR(manager, char, string->chars, string->length+1);
            FREE(manager, ObjString, obj);
            break;
        }
    }
}

void freeObjs(ArenaManager *manager, ZynkVM *vm) {
    Obj *cobj = vm->objects;
    while (cobj!=NULL) {
        Obj *next = cobj->next;
        freeObj(manager, cobj);
        cobj=next;
    }
}

void initVM(ZynkVM *vm) {
    vm->chunk = NULL;
    vm->ip = NULL;
    reset_stack(vm);
    vm->objects = NULL;
    
    // Inicializar el stack con valores NULL
    for (int i = 0; i < STACK_MAX; i++) {
        vm->stack[i] = NULL_VAL;
    }
}

void freeVM(ArenaManager *manager, ZynkVM *vm) {
    freeObjs(manager, vm);
}

ZynkResult zynk_interpret(ArenaManager *manager, ZynkVM *vm, Chunk *chunk) {
    vm->chunk=chunk;
    vm->ip=vm->chunk->code;
    return run(manager, vm);
}
