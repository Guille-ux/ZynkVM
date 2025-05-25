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

#ifndef ZASM_H
#define ZASM_H

#include "zcommon.h"

typedef struct {
    char *str;
    size_t len;
} String;


size_t zlen(const char *str, char sym);

double str2double(const char *text);

char zcmp(const char *a, const char *b, char sym);

char stringcmp(String a, String b);

String ztok(const char *str, char ch, size_t index);

void translate_linez(const char *line, uint8_t *code_index, Value *constants_index);

#endif

// template para añadir más elif's >>> } else if (zcmp(line, "TRUE;", ';')) { <<<
// template para añadir más adiciones de bytecode >>> code[*code_index] = OP_TRUE; <<<