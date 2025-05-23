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

#ifndef _TMEM_H
#define _TMEM_H

#include "../common.h"

void tmemcpy(uint8_t *recv, uint8_t *sender, size_t large);

uint32_t tlen(char *chars);

bool tmemcmp(uint8_t *a, uint8_t *b);

bool fmemcmp(uint8_t *a, uint8_t *b, uint32_t la, uint32_t lb);

//void blocpy(MemBlock *recv, MemBlock *sender, uint32_t large) {
//    uint32_t i = 0;
//    while (i < large) {
//        recv->value=sender->value;
//        sender=sender->next;
//        recv=recv->next;
//        if (sender->next==(MemBlock *)NULL || recv->next==(MemBlock *)NULL) {
//            //en el futuro dar error
//            break;
//        }
//        i++;
//    }
//}

#endif