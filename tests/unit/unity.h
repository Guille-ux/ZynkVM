/*
 * Unity Test Framework for ZynkVM
 * Copyright (c) 2025 Guillermo Leira Temes
 * GPLv3 License
 */

#ifndef UNITY_H
#define UNITY_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <string.h> // Added for strcmp, based on your previous output


// Tipos de datos para los tests
typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;
typedef signed long long INT64;
typedef unsigned long long UINT64;
typedef float FLOAT;
typedef double DOUBLE;

// Estructura para mantener el estado de Unity
typedef struct {
    const char* TestFile;
    const char* CurrentTestName;
    UINT32 CurrentTestLineNumber;
    UINT32 NumberOfTests;
    UINT32 TestFailures;
    UINT32 TestIgnores;
    UINT32 CurrentTestFailed;
    UINT32 CurrentTestIgnored;
} UNITY_STORAGE_T;

extern UNITY_STORAGE_T Unity;

// Macros principales para tests
#define TEST_ASSERT(condition)                   do { if (!(condition)) { Unity.CurrentTestFailed = 1; printf("\nTEST FAILED at %s:%d\n", __FILE__, __LINE__); } } while(0)
#define TEST_ASSERT_TRUE(condition)             TEST_ASSERT(condition)
#define TEST_ASSERT_FALSE(condition)            TEST_ASSERT(!(condition))
#define TEST_ASSERT_NULL(pointer)               TEST_ASSERT(pointer == NULL)
#define TEST_ASSERT_NOT_NULL(pointer)           TEST_ASSERT(pointer != NULL)
#define TEST_ASSERT_EQUAL(expected, actual)     TEST_ASSERT((expected) == (actual))
#define TEST_ASSERT_NOT_EQUAL(expected, actual) TEST_ASSERT((expected) != (actual))
// ADDED: Macro for greater than assertion
#define TEST_ASSERT_GREATER_THAN(expected, actual) TEST_ASSERT((actual) > (expected))

// Macros para números de punto flotante
#define TEST_ASSERT_FLOAT_EQUAL(expected, actual)    TEST_ASSERT(fabs((expected) - (actual)) < 0.00001f)
#define TEST_ASSERT_DOUBLE_EQUAL(expected, actual)   TEST_ASSERT(fabs((expected) - (actual)) < 0.00001)

// Macros para strings
#define TEST_ASSERT_EQUAL_STRING(expected, actual)   TEST_ASSERT(strcmp(expected, actual) == 0)

// Framework functions
void UnityBegin(const char* filename);
int UnityEnd(void);
void UnityDefaultTestRun(void);

// Macros para el framework
#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()

// Macro para ejecutar un test individual
#define RUN_TEST(test) do {\
    Unity.CurrentTestName = #test;\
    Unity.CurrentTestLineNumber = __LINE__;\
    Unity.CurrentTestFailed = 0;\
    setUp(); /* Call setUp before each test */ \
    test();\
    if (!Unity.CurrentTestFailed) { /* Only call tearDown if test didn't fail in setUp */ \
        tearDown(); /* Call tearDown after each test */ \
    }\
    Unity.NumberOfTests++;\
    if (Unity.CurrentTestFailed) Unity.TestFailures++;\
} while(0)

#endif /* UNITY_H */