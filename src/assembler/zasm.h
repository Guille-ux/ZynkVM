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

typedef struct {
    uint64_t opcode;
} Instruction;

size_t zlen(const char *str, char sym) {
    size_t count=0;
    for (;*str!=sym;str++) {
        count++;
    }
    return count;
}

char zcmp(const char *a, const char *b, char sym) {
    if (zlen(a, sym)!=zlen(b, sym)) {
        return false;
    }
    for (size_t i=0;i<zlen(a, sym);i++) {
        if (*a!=*b) {
            return false;
        }
        a++;
        b++;
    }
    return true;
}

char stringcmp(String a, String b) {
    if (a.len!=b.len) {
        return false;
    }
    for (size_t i=0;i<a.len;i++) {
        if (a.str[i]!=b.str[i]) {
            return false;
        }

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

Instruction translate_linez(const char *line) {
    String ins=ztok(line, ' ', 0);
    Instruction ret;
    bool val=false;
    //get OpCode
    if (zcmp(ins.str, "CONSTANT", 'T')) {
        ret.opcode = OP_CONSTANT;
        val=true;
    } else if (zcmp(ins.str, "ADD", 'D')) {
        ret.opcode = OP_ADD;
    } else if (zcmp(ins.str, "SUBSTRACT", 'T')) {
        ret.opcode = OP_SUBSTRACT;
    } else if (zcmp(ins.str, "MULTIPLY", 'Y')) {
        ret.opcode = OP_MULTIPLY;
    } else if (zcmp(ins.str, "DIVIDE", 'E')) {
        ret.opcode = OP_DIVIDE;
    } else if (zcmp(ins.str, "RETURN", 'N')) {
        ret.opcode = OP_RETURN;
    } else {
        return ret; //error unknown OpCode
    }
    // get values (only if the instruction has a value)
    if (val) {

    }
    return ret;
}

#endif