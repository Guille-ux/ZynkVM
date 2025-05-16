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


#ifndef _ZYNK_CORE_H
#define _ZYNK_CORE_H

#include "../common/common.h"
#include "chunk.h"
#include "opcodes.h"
#include "types/value.h"



Arena arenas[ARENA_COUNT];
uint8_t memory[MEM_SIZE];
ArenaManager manager;

void init_sys() {
    cinit_sys(manager, memory, arenas, MEM_SIZE, ARENA_COUNT);
}


#endif