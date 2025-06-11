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
#include <stdbool.h>
#include "tmem.h"

void tmemcpy(uint8_t *recv, uint8_t *sender, size_t large) {
    for (size_t i=0; i<large; i++) {
        recv[i]=sender[i];
    }
}

uint32_t tlen(char *chars) {
    uint32_t counter=0;
    while (chars[counter]!='\0') {
        counter++;
    }
    return counter;
}

bool tmemcmp(uint8_t *a, uint8_t *b) {
    uint32_t la = tlen(a);
    if (la != tlen(b)) {
        return false;
    } else {
        for (uint32_t i=0;i<la;i++) {
            if (a[i]!=b[i]) {
                return false;
            }
        }
    }
    return false;
}

bool fmemcmp(uint8_t *a, uint8_t *b, uint32_t la, uint32_t lb) {
    if (la!=lb) {
        return false;
    } else {
        for (uint32_t i=0;i<la;i++) {
            if (a[i]!=b[i]) {
                return false;
            }
        }
    }
    return true;
}
