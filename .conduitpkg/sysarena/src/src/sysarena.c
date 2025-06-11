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

#include "../include/types.h"
#include "../include/sysarena.h"

// Inicializa una arena como vacía
void poor_arena_init(Arena *arena) {
    if (!arena) return;
    arena->size = 0;
    arena->used = 0;
    arena->base = NULL;
    arena->in_use = false;
    arena->is_contiguous = false;
}

// Inicializa una arena con datos concretos
bool arena_init(Arena *arena, size_t size, ptr_t base) {
    if (!arena || !base) return false;
    arena->size = size;
    arena->base = base;
    arena->used = 0;
    arena->in_use = true;
    arena->is_contiguous = true;
    return true;
}

// Inicializa el sistema de arenas (bloque génesis)
bool sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t total_size, size_t num_arenas) {
    if (!manager || !memory || !arenas || num_arenas < 1) return false;
    manager->arenas = arenas;
    manager->max_arenas = num_arenas;
    manager->initial_memory = memory;
    manager->initial_size = total_size;
    manager->current_arena_idx = 0;

    // Bloque génesis ocupa toda la memoria
    poor_arena_init(&manager->arenas[0]);
    arena_init(&manager->arenas[0], total_size, memory);
    for (size_t i = 1; i < num_arenas; i++) {
        poor_arena_init(&manager->arenas[i]);
    }
    return true;
}

// Reserva memoria siguiendo la lógica del bloque génesis
void* sysarena_alloc(ArenaManager *manager, size_t size) {
    if (!manager || size == 0) return NULL;

    // Busca el bloque génesis (primer bloque libre y en uso)
    size_t genesis_idx = 0;
    while (genesis_idx < manager->max_arenas && (!manager->arenas[genesis_idx].in_use || manager->arenas[genesis_idx].used == manager->arenas[genesis_idx].size)) {
        genesis_idx++;
    }
    if (genesis_idx >= manager->max_arenas) return NULL; // No hay génesis válido

    Arena *genesis = &manager->arenas[genesis_idx];

    // Si el génesis es el único bloque libre con espacio, tomamos memoria de él
    if (genesis->size - genesis->used >= size) {
        // Buscar un slot libre para el nuevo bloque
        size_t new_idx = genesis_idx;
        // Si no hay espacio para desplazar, no se puede asignar
        if (manager->max_arenas - 1 < genesis_idx) return NULL;

        // Desplazar las arenas a la derecha para dejar hueco al génesis
        for (size_t i = manager->max_arenas - 1; i > genesis_idx; --i) {
            manager->arenas[i] = manager->arenas[i - 1];
        }
        // Crear arena nueva en la posición donde estaba el génesis
        Arena *new_arena = &manager->arenas[genesis_idx];
        new_arena->size = size;
        new_arena->used = size;
        new_arena->base = (uint8_t*)genesis->base + genesis->used;
        new_arena->in_use = true;
        new_arena->is_contiguous = true;

        // Actualizar el génesis (ahora está a la derecha)
        genesis = &manager->arenas[genesis_idx + 1];
        genesis->base = (uint8_t*)new_arena->base + size;
        genesis->size = genesis->size - size - genesis->used;
        genesis->used = 0;
        genesis->in_use = (genesis->size > 0);
        genesis->is_contiguous = true;

        return new_arena->base;
    }

    // Si hay otros bloques con espacio, buscar entre ellos (puedes personalizar esta parte)
    for (size_t i = 0; i < manager->max_arenas; ++i) {
        if (manager->arenas[i].in_use && manager->arenas[i].size - manager->arenas[i].used >= size) {
            Arena *arena = &manager->arenas[i];
            void* ptr = (uint8_t*)arena->base + arena->used;
            arena->used += size;
            return ptr;
        }
    }

    return NULL; // No hay espacio
}

// Libera una arena entera y fusiona si es posible
bool sysarena_free(ArenaManager *manager, void *ptr) {
    if (!manager || !ptr) return false;
    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *arena = &manager->arenas[i];
        if (arena->in_use && arena->base &&
            (uint8_t*)ptr >= (uint8_t*)arena->base && (uint8_t*)ptr < (uint8_t*)arena->base + arena->size) {
            poor_arena_init(arena);
            // Intentar fusionar con adyacentes
            sysarena_defragment(manager);
            return true;
        }
    }
    return false;
}

// Fusiona arenas vacías y adyacentes, si es posible
void sysarena_defragment(ArenaManager *manager) {
    if (!manager) return;
    for (size_t i = 0; i < manager->max_arenas - 1; i++) {
        Arena *a = &manager->arenas[i];
        Arena *b = &manager->arenas[i + 1];
        // Si ambas están libres y son contiguas
        if (!a->in_use && !b->in_use && a->size > 0 && b->size > 0 &&
            (uint8_t*)a->base + a->size == (uint8_t*)b->base) {
            a->size += b->size;
            poor_arena_init(b);
            // Desplazar arenas a la izquierda
            for (size_t j = i + 1; j < manager->max_arenas - 1; j++) {
                manager->arenas[j] = manager->arenas[j + 1];
            }
            poor_arena_init(&manager->arenas[manager->max_arenas - 1]);
            i--; // Reintentar por si hay más para fusionar
        }
    }
}

// El resto de funciones pueden quedarse igual o adaptarse a esta lógica
bool arena_can_merge(const Arena *a, const Arena *b) {
    return (!a->in_use && !b->in_use);
}
bool arena_is_void(const Arena *a) {
    return a->used == 0;
}
bool arena_merge(Arena *dest, Arena *src) {
    if (!arena_can_merge(dest, src)) return false;
    dest->size += src->size;
    poor_arena_init(src);
    return true;
}
void copy_arena(Arena *dest, const Arena *src) {
    if (!dest || !src) return;
    dest->size = src->size;
    dest->used = src->used;
    dest->base = src->base;
    dest->in_use = src->in_use;
    dest->is_contiguous = src->is_contiguous;
}
void sysarena_displacement(ArenaManager *manager, size_t where) {
    // Puedes mantener o eliminar esta función según si la usas en split/merge
}
bool sysarena_split(ArenaManager *manager, size_t index, size_t size) {
    // Puedes dejar esta función vacía o ajustarla si quieres soportar splits
    return false;
}
bool sysarena_is_fully_merged(ArenaManager *manager) {
    if (!manager) return false;
    size_t active_arenas = 0;
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (manager->arenas[i].in_use) active_arenas++;
    }
    return active_arenas == 1 &&
           manager->arenas[0].size == manager->initial_size &&
           manager->arenas[0].base == manager->initial_memory;
}
