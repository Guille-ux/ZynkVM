/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
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
#ifndef STANDALONE

#ifndef DEBUG_H
#define DEBUG_H

#include "../common/common.h"
#include "chunk.h"
#include "opcodes.h"
#include <stdio.h>

static int simple_instruction(const char *name, int offset) {
    printf(" : %s\n", name);
    return offset + 1;
}


int disassemble_instruction(Chunk* chunk, int offset) {
    printf("%04d", offset);
    uint8_t instruction=chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("Unknown Instruction OpCode '%d'\n", instruction);
            return offset + 1;
    }
}

void disassemble_chunk(Chunk *chunk, const char * name) {
    printf("[+] == %s == [+] \n", name);
    printf("%d Instruction/s\n", chunk->count);
    for (int offset=0;offset<chunk->count;) {
        offset=disassemble_instruction(chunk, offset);
    }
}

#endif
#else
#ifndef DEBUG_H
#define DEBUG_H
#include "../common/common.h"
#include "chunk.h"
#include "opcodes.h"

#include "../common/common.h"
#include "chunk.h"
#include "opcodes.h"

//here will be an implementation made for Munix using it's standard library

#endif
#endif