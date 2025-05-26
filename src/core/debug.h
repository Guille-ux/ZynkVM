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

#ifndef DEBUG_H
#define DEBUG_H

#include "../common.h"
#include "chunk.h"
#include "opcodes.h"
#include <stdio.h>

#ifndef STANDALONE
size_t simple_instruction(const char *name, size_t offset);
size_t constant_instruction(const char *name, Chunk *chunk, size_t offset);
int disassemble_instruction(Chunk* chunk, size_t offset);
void disassemble_chunk(Chunk *chunk, const char * name);
#else
// Declaraciones alternativas para Munix aquí si las necesitas
#endif

#endif

//template para añadir más cosas:         case OP_DIVIDE: return simple_instruction("OP_DIVIDE", offset);
