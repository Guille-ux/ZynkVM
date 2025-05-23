/*
 * Test Suite for Chunk Module - ZynkVM
 * Copyright (c) 2025 Guillermo Leira Temes
 * GPLv3 License
 */

#include "unity.h"
#include "../../src/core/chunk.h"
#include "../../src/core/value.h"
#include "../../src/common.h"
#include "../../src/core/opcodes.h"
// ADDED: Include for init_sys declaration
#include "../../.conduitpkg/sysarena/src/include/sysarena.h"

// REMOVED: static Chunk chunk; and static ArenaManager manager;
// REMOVED: setUp() and tearDown() functions from here.
// These are managed by test_value.c's main setUp/tearDown for the unified runner.
// The 'manager' variable will be accessible if it's truly global (defined in test_value.c).

extern Arena arenas[];
extern uint8_t memory[];
extern ArenaManager manager;

void test_chunk_initialization(void) {
    Chunk test_chunk; // Use a local chunk for isolation
    init_chunk(&test_chunk);

    TEST_ASSERT_EQUAL(0, test_chunk.count);
    TEST_ASSERT_EQUAL(0, test_chunk.capacity);
    TEST_ASSERT_NULL(test_chunk.code);
    TEST_ASSERT_NULL(test_chunk.lines);
    TEST_ASSERT_EQUAL(0, test_chunk.constants.count);
    TEST_ASSERT_EQUAL(0, test_chunk.constants.capacity);
    TEST_ASSERT_NULL(test_chunk.constants.values);

    // Assuming 'manager' is a global or passed from test_value.c's setUp
    // You might need to ensure 'manager' is defined as 'extern' in common.h
    // if it's defined in test_value.c and used here.
    free_chunk(&manager, &test_chunk); // Cleanup for local chunk
}

void test_write_chunk(void) {
    Chunk test_chunk;
    init_chunk(&test_chunk);
    writeChunk(&manager, &test_chunk, OP_RETURN, 123);

    TEST_ASSERT_EQUAL(1, test_chunk.count);
    TEST_ASSERT_GREATER_THAN(0, test_chunk.capacity); // UNCOMMENTED
    TEST_ASSERT_NOT_NULL(test_chunk.code);
    TEST_ASSERT_NOT_NULL(test_chunk.lines);
    TEST_ASSERT_EQUAL(OP_RETURN, test_chunk.code[0]);
    TEST_ASSERT_EQUAL(123, test_chunk.lines[0]);
    free_chunk(&manager, &test_chunk);
}

void test_add_constant(void) {
    Chunk test_chunk;
    init_chunk(&test_chunk);
    Value constant = NUMBER_VAL(3.14);
    int index = addConstant(&manager, &test_chunk, constant);

    TEST_ASSERT_EQUAL(0, index);
    TEST_ASSERT_EQUAL(1, test_chunk.constants.count);
    TEST_ASSERT_GREATER_THAN(0, test_chunk.constants.capacity); // UNCOMMENTED
    TEST_ASSERT_NOT_NULL(test_chunk.constants.values);
    TEST_ASSERT_DOUBLE_EQUAL(3.14, AS_NUMBER(test_chunk.constants.values[0]));
    free_chunk(&manager, &test_chunk);
}

void test_multiple_chunks(void) {
    Chunk test_chunk;
    init_chunk(&test_chunk);
    writeChunk(&manager, &test_chunk, OP_CONSTANT, 1);
    writeChunk(&manager, &test_chunk, 0, 1);  // índice constante
    writeChunk(&manager, &test_chunk, OP_RETURN, 1);

    TEST_ASSERT_EQUAL(3, test_chunk.count);
    TEST_ASSERT_EQUAL(OP_CONSTANT, test_chunk.code[0]);
    TEST_ASSERT_EQUAL(0, test_chunk.code[1]);
    TEST_ASSERT_EQUAL(OP_RETURN, test_chunk.code[2]);
    free_chunk(&manager, &test_chunk);
}

void test_chunk_capacity_growth(void) {
    Chunk test_chunk;
    init_chunk(&test_chunk);
    int initial_capacity;
    writeChunk(&manager, &test_chunk, OP_RETURN, 1);
    initial_capacity = test_chunk.capacity;

    // Escribir más chunks para forzar el crecimiento
    for (int i = 0; i < initial_capacity + 1; i++) {
        writeChunk(&manager, &test_chunk, OP_RETURN, 1);
    }

    TEST_ASSERT_GREATER_THAN(initial_capacity, test_chunk.capacity); // UNCOMMENTED
    free_chunk(&manager, &test_chunk);
}

// REMOVED: int main(void) function from here.