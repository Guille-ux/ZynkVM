/*
 * Unity Test Framework for ZynkVM
 * Copyright (c) 2025 Guillermo Leira Temes
 * GPLv3 License
 */

#include "unity.h"
#include <stdio.h>
#include <string.h>

// Variable global para almacenar el estado de Unity
UNITY_STORAGE_T Unity = {
    .TestFile = NULL,
    .CurrentTestName = NULL,
    .CurrentTestLineNumber = 0,
    .NumberOfTests = 0,
    .TestFailures = 0,
    .TestIgnores = 0,
    .CurrentTestFailed = 0,
    .CurrentTestIgnored = 0
};

// Colores ANSI para la salida
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void UnityBegin(const char* filename) {
    Unity.TestFile = filename;
    Unity.NumberOfTests = 0;
    Unity.TestFailures = 0;
    Unity.TestIgnores = 0;
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
    
    printf("\n"
           "---------------------------------------\n"
           "Running Tests from %s\n"
           "---------------------------------------\n",
           filename);
}

int UnityEnd(void) {
    printf("\n"
           "---------------------------------------\n"
           "Test Summary: %s\n"
           "  Tests:    %d\n"
           "  Failures: %d\n"
           "  Ignored:  %d\n"
           "---------------------------------------\n",
           Unity.TestFile,
           Unity.NumberOfTests,
           Unity.TestFailures,
           Unity.TestIgnores);

    if (Unity.TestFailures == 0) {
        printf(ANSI_COLOR_GREEN "OK - All tests passed!" ANSI_COLOR_RESET "\n");
        return 0;
    } else {
        printf(ANSI_COLOR_RED "FAIL - %d test(s) failed" ANSI_COLOR_RESET "\n",
               Unity.TestFailures);
        return 1;
    }
}

void UnityDefaultTestRun(void) {
    const char* testName = Unity.CurrentTestName;
    if (Unity.CurrentTestFailed) {
        printf(ANSI_COLOR_RED "[FAIL]" ANSI_COLOR_RESET " %s\n", testName);
    } else if (Unity.CurrentTestIgnored) {
        printf(ANSI_COLOR_YELLOW "[IGNORED]" ANSI_COLOR_RESET " %s\n", testName);
    } else {
        printf(ANSI_COLOR_GREEN "[PASS]" ANSI_COLOR_RESET " %s\n", testName);
    }
}