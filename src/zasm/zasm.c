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

#include "zasm.h"

size_t zlen(const char *str, char sym) {
    size_t count=0;
    for (;*str!=sym&&*str!='\0';str++) {
        count++;
    }
    return count;
}

double str2double(const char *text) {
    double ret = 0.0;
    int sign = 1;
    size_t i = 0;
    int decimal_point_found = 0;
    double decimal_multiplier = 0.1;

    if (text[i] == '-') {
        sign = -1;
        i++;
    } else if (text[i] == '+') {
        i++;
    }

    while (text[i] != '\0') {
        if (text[i] >= '0' && text[i] <= '9') {
            if (!decimal_point_found) {
                ret = ret * 10.0 + (text[i] - '0');
            } else {
                ret += (text[i] - '0') * decimal_multiplier;
                decimal_multiplier *= 0.1;
            }
            i++;
        } else if (text[i] == '.') {
            if (decimal_point_found) {
                return 0.0/0.0; // NaN (Not a Number)
            }
            decimal_point_found = 1;
            i++;
        } else {
            return 0.0/0.0; // NaN (Not a Number)
        }
    }

    return ret * sign;
}

char zcmp(const char *a, const char *b, char sym) {
    if (zlen(a, sym)!=zlen(b, sym)) {
        return FALSE;
    }
    for (size_t i=0;i<zlen(a, sym);i++) {
        if (*a!=*b) {
            return FALSE;
        }
        a++;
        b++;
    }
    return TRUE;
}

char stringcmp(String a, String b) {
    if (a.len!=b.len) {
        return FALSE;
    }
    for (size_t i=0;i<a.len;i++) {
        if (a.str[i]!=b.str[i]) {
            return FALSE;
        }

    }
    return TRUE;
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

void translate_linez(const char *line, uint8_t *code_index, Value *constants_index) {
    if (zcmp(line, "CONSTANT;", ';')) {
        *code_index = OP_CONSTANT;
    } else if (zcmp(line, "ADD;", ';')) {
        *code_index = OP_ADD;
    } else if (zcmp(line, "SUBSTRACT;", ';')) {
        *code_index = OP_SUBSTRACT;
    } else if (zcmp(line, "MULTIPLY;", ';')) {
        *code_index = OP_MULTIPLY;
    } else if (zcmp(line, "DIVIDE;", ';')) {
        *code_index = OP_DIVIDE;
    } else if (zcmp(line, "RETURN;", ';')) {
        *code_index = OP_RETURN;
    } else if(zcmp(line, "NEGATE;", ';')) {
        *code_index = OP_NEGATE;
    } else if (zcmp(line, "TRUE;", ';')) {
        *code_index = OP_TRUE;
    } else if (zcmp(line, "FALSE;", ';')) {
        *code_index = OP_FALSE;
    } else if (zcmp(line, "NULL;", ';')) {
        *code_index = OP_NULL;
    } else if (zcmp(line, "EQUAL;", ';')) {
        *code_index = OP_EQUAL;
    } else if (zcmp(line, "GREATER;", ';')) {
        *code_index = OP_GREATER;
    } else if (zcmp(line, "LESS;", ';')) {
        *code_index = OP_LESS;
    } else if (zcmp(line, "BYTE;", ';')) {
        *code_index = OP_BYTE;
    } else if (zcmp(line, "DEL;", ';')) {
        *code_index = OP_DEL;
    } else if (zcmp(line, "DUPE;", ';')) {
        *code_index = OP_DUPE;
    } else if (zcmp(line, "BACKDEL;", ';')) {
        *code_index = OP_BACKDEL;
    } else if (line[0]=='b') {
        *code_index=line[1];
    } else if (line[0]=='n') {
        code_index--;
        constants_index++;
    } else {
        code_index--;
    }
    code_index++;
}