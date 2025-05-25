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
BUILD_DIR = build
BUILD_CORE_DIR = $(BUILD_DIR)/$(CORE_DIR)
BUILD_SRC_DIR = $(BUILD_DIR)/$(SRC_DIR)

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

# All object files for the main VM executable
MAIN_OBJ = $(CORE_OBJ) $(COMMON_OBJ)
# All object files for the test runner executable
TEST_RUNNER_OBJ = $(CORE_OBJ) $(COMMON_OBJ) $(TEST_OBJ) # Test runner needs all VM objects + test objects

# --- Binaries ---
MAIN_EXE = $(BUILD_DIR)/zynkvm

# --- Phony Targets ---
.PHONY: all clean test debug dirs test_value test_chunk

# --- Main Targets ---
all: $(MAIN_EXE)

# --- Build Rules ---

# Rule for the main VM executable
$(MAIN_EXE): $(MAIN_OBJ)
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



# --- Directory Creation Rules ---
$(BUILD_DIR)/$(SRC_DIR):
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)

$(BUILD_DIR)/$(CORE_DIR):
	@mkdir -p $(BUILD_DIR)/$(CORE_DIR)


# You can keep a 'dirs' target for manual creation, but the above
# pattern rules implicitly create them via the '|' prerequisite.
dirs: $(BUILD_DIR)/$(SRC_DIR) $(BUILD_DIR)/$(CORE_DIR)
	@echo "Ensured build directories exist."

# --- Clean Target ---
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

