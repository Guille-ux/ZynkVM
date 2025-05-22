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

#include "hash.h"

void initTable(ArenaManager *manager, Table *table) {
    table->count=0;
    table->capacity=0;
    table->entries=null;
}

void freeTable(ArenaManager *manager, Table *table) {
    reallocate(manager, table->entries, table->capacity, 0);
    table->count=0;
    table->capacity=0;
}

bool tableSet(ArenaManager *manager, Table *table, Value a, const char name[8]) {

}

bool tableGet(ArenaManager *manager, Table *table, const char so[8]) {

}

bool tableDelete(ArenaManager *manager, Table *table, const char so[8]) {

}

void reAdjustTableCapacity(ArenaManager *manager, Table *table, uint32_t capacity) {
    table->entries=(Entry *)reallocate(manager, table->entries, table->capacity, capacity);
    for (uint32_t i=table->capacity;i<capacity;i++) {
        table->entries[i].hash=(uint32_t)null;
        table->entries[i].free=true;
        table->entries[i].value=NULL_VAL;
    }
    table->capacity=capacity;
}

void initTableCapacity(ArenaManager *manager, Table *table, uint32_t capacity) {
    table->entries=(Entry *)reallocate(manager, table->entries, table->capacity, capacity);
    for (uint32_t i=0;i<capacity;i++) {
        table->entries[i].hash=(uint32_t)null;
        table->entries[i].free=true;
        table->entries[i].value=NULL_VAL;
    }
    table->capacity=capacity;
}