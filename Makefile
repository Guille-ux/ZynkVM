# Makefile for ZynkVM
# Copyright (c) 2025 Guillermo Leira Temes
# GPLv3 License

# --- Compiler and Flags ---
CC = gcc
CFLAGS = -g -I. -std=c11 # Added -Wall -Wextra for more warnings
# Add this if you want to use the sysarena library from a specific directory
# Adjust the path to where your libsysarena.a file is located
LIB_DIR = src # Assuming libsysarena.a is in the root project directory (ZynkVM/)
# If it's in a 'lib' subdirectory, change to: LIB_DIR = lib

# LDFLAGS: link with math library (-lm) and the sysarena library (-lsysarena)
# -L$(LIB_DIR) tells the linker to look for libraries in LIB_DIR
LDFLAGS = -lm -L$(LIB_DIR) -lsysarena

# --- Directories ---
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
TEST_DIR = tests/unit
BUILD_DIR = build
BUILD_CORE_DIR = $(BUILD_DIR)/$(CORE_DIR)
BUILD_SRC_DIR = $(BUILD_DIR)/$(SRC_DIR)
BUILD_TEST_DIR = $(BUILD_DIR)/$(TEST_DIR)

# --- Source Files ---
# Find all .c files in core and its subdirectories
CORE_SRC = $(shell find $(CORE_DIR) -name "*.c")
# Find .c files directly in src/ (e.g., main.c, common.c, sysarena.c if they are there)
COMMON_SRC = $(wildcard $(SRC_DIR)/*.c)
# Find all .c files in the test directory
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

# Combine all application source files
APP_SOURCES = $(CORE_SRC) $(COMMON_SRC)

# --- Object Files ---
# Transform .c paths to .o paths within BUILD_DIR, maintaining structure
# E.g., src/core/foo.c -> build/src/core/foo.o
CORE_OBJ = $(patsubst $(CORE_DIR)/%.c,$(BUILD_CORE_DIR)/%.o,$(CORE_SRC))
COMMON_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_SRC_DIR)/%.o,$(COMMON_SRC))
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c,$(BUILD_TEST_DIR)/%.o,$(TEST_SRC))

# All object files for the main VM executable
MAIN_OBJ = $(CORE_OBJ) $(COMMON_OBJ)
# All object files for the test runner executable
TEST_RUNNER_OBJ = $(CORE_OBJ) $(COMMON_OBJ) $(TEST_OBJ) # Test runner needs all VM objects + test objects

# --- Binaries ---
MAIN_EXE = $(BUILD_DIR)/zynkvm
TEST_RUNNER_EXE = $(BUILD_DIR)/test_runner

# --- Phony Targets ---
.PHONY: all clean test debug dirs test_value test_chunk

# --- Main Targets ---
all: $(MAIN_EXE)

# Target for running all tests
test: $(TEST_RUNNER_EXE)
	@echo "Running all unit tests..."
	./$(TEST_RUNNER_EXE)

# --- Build Rules ---

# Rule for the main VM executable
$(MAIN_EXE): $(MAIN_OBJ)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule for the test runner executable
$(TEST_RUNNER_EXE): $(TEST_RUNNER_OBJ)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# --- Pattern Rule for Object Files ---
# This rule handles compilation of any .c file into its corresponding .o file
# within the BUILD_DIR, maintaining the directory structure.
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(SRC_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.c | $(BUILD_DIR)/$(CORE_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)/$(TEST_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# --- Directory Creation Rules ---
$(BUILD_DIR)/$(SRC_DIR):
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)

$(BUILD_DIR)/$(CORE_DIR):
	@mkdir -p $(BUILD_DIR)/$(CORE_DIR)

$(BUILD_DIR)/$(TEST_DIR):
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)

# You can keep a 'dirs' target for manual creation, but the above
# pattern rules implicitly create them via the '|' prerequisite.
dirs: $(BUILD_DIR)/$(SRC_DIR) $(BUILD_DIR)/$(CORE_DIR) $(BUILD_DIR)/$(TEST_DIR)
	@echo "Ensured build directories exist."

# --- Clean Target ---
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

# --- Individual Test Targets ---
# These rules now use the common object file list for core dependencies
# and link against the math library.
# It's better to explicitly list the object files the test needs
test_value: $(BUILD_DIR)/$(TEST_DIR)/test_value.o $(MAIN_OBJ)
	@echo "Linking test_value executable..."
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/test_value $(LDFLAGS) # Changed target name for clarity
	@echo "Running test_value..."
	./$(BUILD_DIR)/test_value

test_chunk: $(BUILD_DIR)/$(TEST_DIR)/test_chunk.o $(MAIN_OBJ)
	@echo "Linking test_chunk executable..."
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/test_chunk $(LDFLAGS) # Changed target name for clarity
	@echo "Running test_chunk..."
	./$(BUILD_DIR)/test_chunk

# --- Debug Info ---
debug:
	@echo "--- Makefile Debug Info ---"
	@echo "CORE_SRC: $(CORE_SRC)"
	@echo "COMMON_SRC: $(COMMON_SRC)"
	@echo "TEST_SRC: $(TEST_SRC)"
	@echo "APP_SOURCES: $(APP_SOURCES)"
	@echo "CORE_OBJ: $(CORE_OBJ)"
	@echo "COMMON_OBJ: $(COMMON_OBJ)"
	@echo "TEST_OBJ: $(TEST_OBJ)"
	@echo "MAIN_OBJ: $(MAIN_OBJ)"
	@echo "TEST_RUNNER_OBJ: $(TEST_RUNNER_OBJ)"
	@echo "MAIN_EXE: $(MAIN_EXE)"
	@echo "TEST_RUNNER_EXE: $(TEST_RUNNER_EXE)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "---------------------------"