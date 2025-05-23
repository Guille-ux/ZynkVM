/* 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 Copyright (c) 2025 Guillermo Leira Temes
*/

#ifndef _SYSARENA_H
#define _SYSARENA_H

#include "../include/types.h" // incluir types.h ya no es rebundante

// definiciones de estructuras
typedef struct Arena {
    size_t size;
    ptr_t base;
    size_t used;
    bool in_use;
} Arena;

// definiciones de funciones para arenas simples

void poor_arena_init(Arena *arena); // inicializa una arena con solo un bool in_use=false;

void arena_init(Arena *arena, size_t size, ptr_t base); // inicializar arena

void* arena_alloc(Arena *arena, size_t size); // reservar memoria

void arena_free(Arena *arena); // liberar toda la arena

// estructuras de arenas complejas (arrays de arenas)
typedef struct ArenaManager {
    Arena* arenas;
    size_t max_arenas;
    uint8_t *memory;
    size_t size;
} ArenaManager;

// funciones de arenas complejas
void sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t size, size_t num_arenas); // inicializa el sistema de arenas

bool arena_can_merge(Arena *a, Arena *b); // comprueba si 2 arenas se pueden fusionar

bool arena_is_void(Arena *a);

void arena_merge(Arena *dest, Arena *src); // une 2 arenas

void sysarena_defragment(ArenaManager *manager); // recorre todas las arenas e intenta fusionarlas

void* sysarena_alloc(ArenaManager *manager, size_t size); // reserva memoria en el sistema de multiples arenas

void sysarena_free(ArenaManager *manager, void *ptr); // libera memoria

void sysarena_split(ArenaManager *manager,  size_t arena_index, size_t size); // dividir una arena

void sysarena_displacement(ArenaManager *manager, size_t from_wheres); //desplazar todas las arenas uno a la derecha

//utilidades creadas para solucionar errores
void copy_arena(Arena *dest, Arena *src); // copia una arena


#endif