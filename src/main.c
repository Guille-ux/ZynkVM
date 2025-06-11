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

#define DEBUG

#include <stdio.h>
#include "common.h"
#include "core/value.h"
#include "core/vm.h"
#include "core/tmem.h"
#include "core/opcodes.h"
#include "core/chunk.h"
#include "zasm/zasm.h"

#define VERSION "0.0.1"
#define DEBUG

Arena arenas[ARENA_COUNT];
uint8_t memory[MEM_SIZE];
ArenaManager manager;

int add_test() {
    uint8_t *code_buffer=(uint8_t *)sysarena_alloc(&manager, 512);
    Value *value_buffer=(Value *)sysarena_alloc(&manager, 256*sizeof(Value));
    translate_linez("ADD;", code_buffer, value_buffer);
    translate_linez("10", code_buffer, value_buffer);
    translate_linez("CONSTANT;", code_buffer, value_buffer);
    translate_linez("10", code_buffer, value_buffer);
    translate_linez("CONSTANT;", code_buffer, value_buffer);
    Chunk chunk;
    load_chunk(&manager, code_buffer, value_buffer, &chunk, 512);
    ZynkVM machine;
    zynk_interpret(&manager, &machine, &chunk);
    return 0;
}

int main(int argc, const char argv[]) {
    sysarena_init(&manager, memory, arenas, MEM_SIZE, ARENA_COUNT);
    printf("[------------------------ ZynkVM %s -------------------------] \n", VERSION);
    printf("[-------------------------- TESTING ----------------------------] \n");
    add_test();
    return 0;
}

