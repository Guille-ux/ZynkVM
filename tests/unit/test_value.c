/*
 * Test Suite for Value Module - ZynkVM
 * Copyright (c) 2025 Guillermo Leira Temes
 * GPLv3 License
 */

#include "unity.h"
#include "../../src/core/value.h"
#include "../../src/core/chunk.h"
#include "../../src/core/vm.h"
#include "../../src/common.h"
// ADDED: Include for init_sys declaration
#include "../../.conduitpkg/sysarena/src/include/sysarena.h"

// Static global for ArenaManager, initialized in setUp
Arena arenas[ARENA_COUNT];
uint8_t memory[MEM_SIZE];
ArenaManager manager;

void setUp(void) {
    init_sys(&manager);
}

void tearDown(void) {
    // Add any cleanup for the ArenaManager here if sysarena provides it,
    // e.g., sys_arena_deinit(&manager);
    // Otherwise, it implicitly cleans up when the process exits.
}

void test_number_value(void) {
    Value v = NUMBER_VAL(3.14159);
    TEST_ASSERT_TRUE(IS_NUMBER(v));
    TEST_ASSERT_FALSE(IS_BOOL(v));
    TEST_ASSERT_FALSE(IS_NULL(v));
    TEST_ASSERT_DOUBLE_EQUAL(3.14159, AS_NUMBER(v));
}

void test_boolean_value(void) {
    Value v1 = BOOL_VAL(TRUE);
    Value v2 = BOOL_VAL(FALSE);

    TEST_ASSERT_TRUE(IS_BOOL(v1));
    TEST_ASSERT_TRUE(IS_BOOL(v2));
    TEST_ASSERT_TRUE(AS_BOOL(v1));
    TEST_ASSERT_FALSE(AS_BOOL(v2));
}

void test_null_value(void) {
    Value v = NULL_VAL;
    TEST_ASSERT_TRUE(IS_NULL(v));
    TEST_ASSERT_FALSE(IS_NUMBER(v));
    TEST_ASSERT_FALSE(IS_BOOL(v));
}

void test_value_array(void) {
    ValueArray array;
    initArray(&array); // manager is initialized in setUp

    TEST_ASSERT_EQUAL(0, array.count);
    TEST_ASSERT_EQUAL(0, array.capacity);
    TEST_ASSERT_NULL(array.values);

    writeArray(&manager, &array, NUMBER_VAL(1.0));
    writeArray(&manager, &array, BOOL_VAL(TRUE));
    writeArray(&manager, &array, NULL_VAL);

    TEST_ASSERT_EQUAL(3, array.count);
    TEST_ASSERT_GREATER_THAN(0, array.capacity); // UNCOMMENTED
    TEST_ASSERT_NOT_NULL(array.values);

    TEST_ASSERT_TRUE(IS_NUMBER(array.values[0]));
    TEST_ASSERT_TRUE(IS_BOOL(array.values[1]));
    TEST_ASSERT_TRUE(IS_NULL(array.values[2]));

    TEST_ASSERT_DOUBLE_EQUAL(1.0, AS_NUMBER(array.values[0]));
    TEST_ASSERT_TRUE(AS_BOOL(array.values[1]));

    freeArray(&manager, &array);
}

void test_value_array_growth(void) {
    ValueArray array;
    initArray(&array); // manager is initialized in setUp

    int initial_capacity;
    writeArray(&manager, &array, NUMBER_VAL(1.0));
    initial_capacity = array.capacity;

    // Forzar crecimiento del array
    for (int i = 0; i < initial_capacity + 1; i++) {
        writeArray(&manager, &array, NUMBER_VAL(i));
    }

    TEST_ASSERT_GREATER_THAN(initial_capacity, array.capacity); // UNCOMMENTED

    freeArray(&manager, &array);
}

void test_value_equality(void) {
    Value n1 = NUMBER_VAL(1.0);
    Value n2 = NUMBER_VAL(1.0);
    Value n3 = NUMBER_VAL(2.0);
    Value b1 = BOOL_VAL(TRUE);
    Value null1 = NULL_VAL;
    Value null2 = NULL_VAL;

    TEST_ASSERT_TRUE(AS_BOOL(compareVals(n1, n2)));
    TEST_ASSERT_FALSE(AS_BOOL(compareVals(n1, n3)));
    TEST_ASSERT_FALSE(AS_BOOL(compareVals(n1, b1)));
    TEST_ASSERT_TRUE(AS_BOOL(compareVals(null1, null2)));
    TEST_ASSERT_FALSE(AS_BOOL(compareVals(null1, n1)));
}

// Main test runner function
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_number_value);
    RUN_TEST(test_boolean_value);
    RUN_TEST(test_null_value);
    RUN_TEST(test_value_array);
    RUN_TEST(test_value_array_growth);
    RUN_TEST(test_value_equality);
    return UNITY_END();
}