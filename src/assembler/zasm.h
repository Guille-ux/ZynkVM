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

#include "../common/common.h"
#include "../core/opcodes.h"

typedef struct {
    char *str;
    size_t len;
} String;

size_t zlen(const char *str, char sym) {
    size_t count=0;
    for (;*str!=sym;str++) {
        count++;
    }
    return count;
}

char zcmp(const char *a, const char *b) {
    if (zlen(a, '\0')!=zlen(b, '\0')) {
        return false;
    }
    for (size_t i=0;i<zlen(a, '\0');i++) {
        if (*a!=*b) {
            return false;
        }
        a++;
        b++;
    }
    return true;
}

String ztok(const char *str, char ch, size_t index) {
    size_t counter=0;
    String ret;
    for (size_t i=0;i<zlen(str, '\0');i++) {
        if (str[i]==ch) {
            counter++;
        }
        if (counter==index) {
            ret.str=(char *)&str[i];
            ret.len=zlen((char *)str[i], ch);
            break;
        }
    }
    return ret;
}

OpCode translate_linez(const char *line) {

}

#endif