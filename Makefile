# --- Compiler and Flags ---
CC = gcc
CFLAGS = -g -I. -std=c11

LIB_DIR = src
LDFLAGS = -L$(LIB_DIR) -lsysarena -lm # Added -lm for math functions if your main app uses them

# --- Directories ---
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
ZASM_DIR = $(SRC_DIR)/zasm
BUILD_DIR = build
BUILD_CORE_DIR = $(BUILD_DIR)/$(CORE_DIR)
BUILD_SRC_DIR = $(BUILD_DIR)/$(SRC_DIR)
BUILD_ZASM_DIR = $(BUILD_DIR)/$(ZASM_DIR)

# --- Main Application Source Files ---
MAIN_FILE ?= main.c
MAIN_PATH = $(SRC_DIR)/$(MAIN_FILE)

CORE_SRC = $(shell find $(CORE_DIR) -name "*.c")
COMMON_SRC = $(filter-out $(MAIN_PATH), $(wildcard $(SRC_DIR)/*.c))
ZASM_SRC = $(shell find $(ZASM_DIR) -name "*.c")

# All application source files
APP_SOURCES = $(CORE_SRC) $(COMMON_SRC) $(MAIN_PATH) $(ZASM_SRC)

# --- Main Application Object Files ---
CORE_OBJ = $(patsubst $(CORE_DIR)/%.c,$(BUILD_CORE_DIR)/%.o,$(CORE_SRC))
COMMON_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_SRC_DIR)/%.o,$(COMMON_SRC))
MAIN_OBJ = $(BUILD_SRC_DIR)/$(basename $(notdir $(MAIN_FILE))).o
ZASM_OBJ = $(patsubst $(ZASM_DIR)/%.c,$(BUILD_ZASM_DIR)/%.o,$(ZASM_SRC))

# All object files for the main VM executable
APP_OBJ = $(CORE_OBJ) $(COMMON_OBJ) $(MAIN_OBJ) $(ZASM_OBJ)

# --- Binaries ---
MAIN_EXE = $(BUILD_DIR)/zynkvm

# --- Phony Targets ---
.PHONY: all clean debug dirs

# --- Main Targets ---
all: $(MAIN_EXE)

# Rule for the main VM executable
$(MAIN_EXE): $(APP_OBJ) | $(BUILD_DIR)
	@echo "Linking $@ ..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Specific rule for main.c
$(BUILD_DIR)/src/main.o: src/main.c | $(BUILD_DIR)/src
	@echo "Compiling main file: $<..."
	$(CC) $(CFLAGS) -DENTRY_POINT -c $< -o $@

# Pattern rule for other source files in src/ (e.g., common.c if you have it directly in src)
$(BUILD_SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_SRC_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for source files in src/core/
$(BUILD_CORE_DIR)/%.o: $(CORE_DIR)/%.c | $(BUILD_CORE_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for source files in src/zasm/
$(BUILD_ZASM_DIR)/%.o: $(ZASM_DIR)/%.c | $(BUILD_ZASM_DIR)
	@echo "Compiling ZASM file: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

# Create subdirectories, ensuring their parent exists first
$(BUILD_SRC_DIR): $(BUILD_DIR)
	@mkdir -p $@

$(BUILD_CORE_DIR): $(BUILD_SRC_DIR)
	@mkdir -p $@

$(BUILD_ZASM_DIR): $(BUILD_SRC_DIR) # ZASM build dir is under build/src
	@mkdir -p $@