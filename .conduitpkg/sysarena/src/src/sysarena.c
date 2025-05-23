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

#include "../include/types.h" // incluir types.h ya no es rebundante
#include "../include/sysarena.h" // incluir la arena


void poor_arena_init(Arena *arena) {
    arena->size=0;
    arena->used=0;
    arena->base=null;
    arena->in_use=false;
}

void arena_init(Arena *arena, size_t size, ptr_t base) {
    arena->size=size;
    arena->base=base;
    arena->used=0;
    arena->in_use=false;
}

void* arena_alloc(Arena *arena, size_t size) {
    if (arena->used + size > arena->size) {
        return null; // devolver null si no hay suficiente memoria
    }
    ptr_t ptr=(void *)(arena->used + arena->base);
    arena->used += size;
    return ptr;
}

void arena_free(Arena *arena) {
    arena->used=0;
    arena->in_use=false;
}

void sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t size, size_t num_arenas) {
    manager->arenas = arenas;
    manager->max_arenas=num_arenas;
    manager->size=size;
    arena_init(&manager->arenas[0], size, (ptr_t)&memory[0]);
    manager->arenas[0].in_use=true;
    for (size_t i=1; i < num_arenas; i++) {
        poor_arena_init(&manager->arenas[i]);
    }
}

bool arena_can_merge(Arena *a, Arena *b) {
    if (a->in_use==false && b->in_use==false) {
        return true;
    }
    return false;
}

bool arena_is_void(Arena *a) {
    if (a->used==0) {
        return true;
    }
    return false;
}

void arena_merge(Arena *dest, Arena *src) {
    dest->size+=src->size;
    src->in_use=false;
}

void sysarena_defragment(ArenaManager *manager) {
    for (size_t i = 0; i < manager->max_arenas; i++) {
        size_t x=i+1;
        while (true) {
            if (arena_can_merge(&manager->arenas[i], &manager->arenas[x])) {
                break;
            }
            x+=1;
        }
        if (arena_is_void(&manager->arenas[i]) && arena_is_void(&manager->arenas[x])) {
            arena_merge(&manager->arenas[i], &manager->arenas[x]);
        }
    }
}

void* sysarena_alloc(ArenaManager *manager, size_t size) {
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (manager->arenas[i].size >= size && manager->arenas[i].in_use) {
            ptr_t to_ret = arena_alloc(&manager->arenas[i], size);
            //dividir la arena
            if (manager->arenas[i].used + size < manager->arenas[i].size) {
                sysarena_split(manager, i, size);
            }
            return to_ret;
        }
    }
    return null;
}

void sysarena_split(ArenaManager *manager, size_t arena_index, size_t size) {
    if (arena_index >= manager->max_arenas || manager->arenas[arena_index].in_use) {
        return;
    }
    Arena *src = &manager->arenas[arena_index];
    if (src->size < size + sizeof(Arena)) { 
        return;
    }
    size_t new_arena_index = 0;
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (!manager->arenas[i].in_use) {
            new_arena_index = i;
            break;
        }
    }
    if (new_arena_index == 0) {
        return;
    }
    if (manager->arenas[new_arena_index].in_use==true) {
        sysarena_displacement(manager, new_arena_index);
    }
    Arena *new_arena = &manager->arenas[new_arena_index];
    new_arena->size = size;
    new_arena->base = src->base+(src->size-size);
    new_arena->used = size;
    new_arena->in_use = true;

    src->size -= size;
}

void sysarena_free(ArenaManager *manager, void *ptr) {
    for (size_t i = 0; i < manager->max_arenas; i++) {
        ptr_t base = manager->arenas[i].base;
        size_t size = manager->arenas[i].size;
        if (ptr >= base && ptr < (ptr_t)((char*)base + size)) {
            arena_free(&manager->arenas[i]);
            return;
        }
    }
}

void sysarena_displacement(ArenaManager *manager, size_t where) {
    for (size_t i=(manager->max_arenas-1); i>where; i--) {
        copy_arena(&manager->arenas[i], &manager->arenas[i-1]);
    }
}

void copy_arena(Arena *dest, Arena *src) {
    dest->in_use=src->in_use;
    dest->base=src->base;
    dest->size=src->size;
    dest->used=src->used;
}