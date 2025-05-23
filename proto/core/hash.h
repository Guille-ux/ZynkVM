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

#ifndef ZYNK_HASH
#define ZYNK_HASH

#include "../common/common.h"
#include "types/value.h"
#include "map.h"

typedef struct {
    bool free;
    uint32_t hash; // el hash, para hacer comprobaciones
    Value value;
} Entry;

typedef struct {
    Entry* entries;
    uint32_t count;
    uint32_t capacity;
} Table;

void initTable(ArenaManager *manager, Table* table);
void freeTable(ArenaManager *manager, Table *table);
bool tableSet(ArenaManager *manager, Table *table, Value a, const char name[8]);
bool tableGet(ArenaManager *manager, Table *table, const char so[8], Value *value);
bool tableDelete(ArenaManager *manager, Table *table, const char so[8]);
void reAdjustTableCapacity(ArenaManager *manager, Table *table, uint32_t capacity);
void initTableCapacity(ArenaManager *manager, Table *table, uint32_t capacity);
uint32_t hash8Str(char *str);

#endif