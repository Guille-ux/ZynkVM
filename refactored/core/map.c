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

#include "map.h"

char zynkMapToASCII(char zynk) {
    switch (zynk) {
        case 0: return 'a';
        case 1: return 'b';
        case 2: return 'c';
        case 3: return 'd';
        case 4: return 'e';
        case 5: return 'f';
        case 6: return 'g';
        case 7: return 'h';
        case 8: return 'i';
        case 9: return 'j';
        case 10: return 'k';
        case 11: return 'l';
        case 12: return 'm';
        case 13: return 'n';
        case 14: return 'o';
        case 15: return 'p';
        case 16: return 'q';
        case 17: return 'r';
        case 18: return 's';
        case 19: return 't';
        case 20: return 'u';
        case 21: return 'v';
        case 22: return 'w';
        case 23: return 'x';
        case 24: return 'y';
        case 25: return 'z';
        case 26: return 'A';
        case 27: return 'B';
        case 28: return 'C';
        case 29: return 'D';
        case 30: return 'E';
        case 31: return 'F';
        case 32: return 'G';
        case 33: return 'H';
        case 34: return 'I';
        case 35: return 'J';
        case 36: return 'K';
        case 37: return 'L';
        case 38: return 'M';
        case 39: return 'N';
        case 40: return 'O';
        case 41: return 'P';
        case 42: return 'Q';
        case 43: return 'R';
        case 44: return 'S';
        case 45: return 'T';
        case 46: return 'U';
        case 47: return 'V';
        case 48: return 'W';
        case 49: return 'X';
        case 50: return 'Y';
        case 51: return 'Z';
        case 52: return '_';
    }
}

char ASCIIMapToZynk(char ascii) {
    switch (ascii) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        case 'i': return 8;
        case 'j': return 9;
        case 'k': return 10;
        case 'l': return 11;
        case 'm': return 12;
        case 'n': return 13;
        case 'o': return 14;
        case 'p': return 15;
        case 'q': return 16;
        case 'r': return 17;
        case 's': return 18;
        case 't': return 19;
        case 'u': return 20;
        case 'v': return 21;
        case 'w': return 22;
        case 'x': return 23;
        case 'y': return 24;
        case 'z': return 25;
        case 'A': return 26;
        case 'B': return 27;
        case 'C': return 28;
        case 'D': return 29;
        case 'E': return 30;
        case 'F': return 31;
        case 'G': return 32;
        case 'H': return 33;
        case 'I': return 34;
        case 'J': return 35;
        case 'K': return 36;
        case 'L': return 37;
        case 'M': return 38;
        case 'N': return 39;
        case 'O': return 40;
        case 'P': return 41;
        case 'Q': return 42;
        case 'R': return 43;
        case 'S': return 44;
        case 'T': return 45;
        case 'U': return 46;
        case 'V': return 47;
        case 'W': return 48;
        case 'X': return 49;
        case 'Y': return 50;
        case 'Z': return 51;
        case '_': return 52;
    }
}
