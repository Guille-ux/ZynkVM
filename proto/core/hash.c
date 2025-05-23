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
    uint32_t key_hash=hash8Str(name);
    uint32_t index = key_hash % table->capacity;
    if (table->capacity <= table->count) {
        return false; //thre isn't enough memory free
    }
    for (;;index = (index + 1) % table->capacity) {
        if (table->entries[index].free) {
            table->entries[index].hash=key_hash;
            table->entries[index].free=false;
            table->entries[index].value = a;
            table->count++;
            break;
        } if (((index+1)%table->capacity)==index-1) {
            return false;
        }
    }
    return true;
}

bool tableGet(ArenaManager *manager, Table *table, const char so[8], Value *value) {
    if (table->capacity <= table->count) {
        return false; //thre isn't enough memory free
    }
    uint32_t key_hash=hash8Str(so);
    uint32_t index = key_hash % table->capacity;
    for (;;index = (index + 1) % table->capacity) {
        if (table->entries[index].hash==key_hash) {
            value = &table->entries[index].value;
            break;
        } if (((index+1)%table->capacity)==index-1) {
            return false;
        }
    }
    return true;
}

bool tableDelete(ArenaManager *manager, Table *table, const char so[8]) {
    uint32_t key_hash=hash8Str(so);
    uint32_t index = key_hash % table->capacity;
    if (table->entries[index].free) {
        return false;
    }
    table->entries[index].hash=(uint32_t)null;
    table->entries[index].free=true;
    table->entries[index].value = NULL_VAL;
    table->count--;
    return true;
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

uint32_t hash8Str(char *str) {
    uint32_t hash=0;
    for (char i=0;i<8;i++) {
        hash ^= (uint8_t)str[i];
        hash *= ZYNK_MAP_RANGE;
    }
    return hash;
}