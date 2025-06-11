/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (c) 2025 Guillermo Leira Temes
 */

#include "../include/types.h"
#include "../include/sysarena.h"

// Initializes an arena as empty or unused.
void poor_arena_init(Arena *arena) {
    arena->size = 0;
    arena->used = 0;
    arena->base = (ptr_t)NULL;
    arena->in_use = FALSE;
}

// Initializes an arena with a specific size and memory base.
void arena_init(Arena *arena, size_t size, ptr_t base) {
    arena->size = size;
    arena->base = base;
    arena->used = 0;
    arena->in_use = TRUE;
}

// Allocates a block of 'size' bytes from an arena.
// Returns NULL if there's not enough space.
void* arena_alloc(Arena *arena, size_t size) {
    // If the allocation exceeds available size, there's no memory.
    if (arena->used + size > arena->size) {
        return (ptr_t)NULL;
    }
    // Calculates the address of the new block.
    ptr_t ptr = (ptr_t)((char*)arena->base + arena->used);
    // Updates the arena's usage.
    arena->used += size;
    return ptr;
}

// Frees an entire arena, resetting its usage counter.
void arena_free(Arena *arena) {
    arena->used = 0;
    arena->in_use = FALSE; // Marks the arena as unused.
}

// Initializes the arena manager.
void sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t total_memory_size, size_t num_arenas) {
    manager->arenas = arenas;
    manager->max_arenas = num_arenas;
    manager->current_arena_idx = 0; // Starts with the first arena.

    if (num_arenas == 0) {
        return; // No arenas to manage, exit.
    }

    // The first arena is initialized with the entire memory block.
    arena_init(&manager->arenas[0], total_memory_size, (ptr_t)memory);

    // Other arenas are initialized as empty.
    for (size_t i = 1; i < num_arenas; i++) {
        poor_arena_init(&manager->arenas[i]);
    }
}

// Checks if two arenas can be merged.
bool arena_can_merge(Arena *a, Arena *b) {
    // They can only be merged if both are not in use.
    if (a->in_use == FALSE && b->in_use == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// Checks if an arena is completely empty.
bool arena_is_void(Arena *a) {
    // It's empty if it has no bytes used.
    if (a->used == 0) {
        return TRUE;
    }
    return FALSE;
}

// Merges the 'src' arena into the 'dest' arena.
void arena_merge(Arena *dest, Arena *src) {
    dest->size += src->size; // Increases the destination's size.
    src->in_use = FALSE; // Marks the source as unused.
    poor_arena_init(src); // Resets the source arena.
}

// Defragments arenas by attempting to merge empty and adjacent ones.
void sysarena_defragment(ArenaManager *manager) {
    for (size_t i = 0; i < manager->max_arenas; i++) {
        // Only consider arenas that are not in use or are already void.
        if (manager->arenas[i].in_use || !arena_is_void(&manager->arenas[i])) {
            continue;
        }

        size_t x = i + 1;
        // Search for an adjacent arena to merge.
        while (x < manager->max_arenas) {
            // Check if we can merge and if arena 'x' is also void.
            if (arena_can_merge(&manager->arenas[i], &manager->arenas[x]) && arena_is_void(&manager->arenas[x])) {
                break; // Found a mergeable partner.
            }
            x += 1;
        }

        // If a partner was found, perform the merge.
        if (x < manager->max_arenas) {
            arena_merge(&manager->arenas[i], &manager->arenas[x]);
            // Re-check the current arena in case more merges are possible.
            i--;
        }
    }
}

// Allocates memory in the arena system.
void* sysarena_alloc(ArenaManager *manager, size_t size) {
    if (size == 0) return (ptr_t)NULL; // Cannot allocate 0 bytes.

    ptr_t allocated_ptr = (ptr_t)NULL;

    // First, try to allocate from the current arena.
    if (manager->current_arena_idx < manager->max_arenas &&
        manager->arenas[manager->current_arena_idx].in_use) { // Must be initialized.
        allocated_ptr = arena_alloc(&manager->arenas[manager->current_arena_idx], size);
        if (allocated_ptr != (ptr_t)NULL) {
            return allocated_ptr; // Allocation successful.
        }
    }

    // If the current arena fails, search in others.
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (i == manager->current_arena_idx) continue; // Skip already attempted current arena.

        if (manager->arenas[i].in_use) { // Only if the arena is initialized and in use.
            allocated_ptr = arena_alloc(&manager->arenas[i], size);
            if (allocated_ptr != (ptr_t)NULL) {
                manager->current_arena_idx = i; // Update the current arena.
                return allocated_ptr; // Allocation successful.
            }
        }
    }

    return (ptr_t)NULL; // No suitable arena found.
}

// Splits an arena into two.
void sysarena_split(ArenaManager *manager, size_t arena_index, size_t size) {
    // If index is out of range or arena is not in use, exit.
    if (arena_index >= manager->max_arenas || manager->arenas[arena_index].in_use == FALSE) {
        return;
    }

    Arena *src = &manager->arenas[arena_index];

    // Cannot split if the size is greater than what's left in the arena.
    if (src->size < size) {
        return;
    }

    size_t new_arena_idx = manager->max_arenas; // Index for the new arena.
    // Search for an empty slot for the new arena.
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (manager->arenas[i].in_use == FALSE) {
            new_arena_idx = i;
            break;
        }
    }

    // If no free slot is found, we cannot split.
    if (new_arena_idx == manager->max_arenas) {
        return;
    }

    // If the found slot for new_arena_idx is already in use (complex situation),
    // try to displace arenas.
    if (manager->arenas[new_arena_idx].in_use == TRUE) {
        sysarena_displacement(manager, new_arena_idx);
    }

    Arena *new_arena = &manager->arenas[new_arena_idx];

    // The new arena starts right after the source's used space.
    new_arena->base = (ptr_t)((char*)src->base + src->used);
    new_arena->size = size;
    new_arena->used = 0; // The new arena starts empty.
    new_arena->in_use = TRUE; // Mark it as in use.

    // Reduce the size of the source arena.
    src->size -= size;
}

// Frees memory associated with a pointer.
// This means resetting the entire arena the pointer belongs to.
// It does NOT free individual blocks.
void sysarena_free(ArenaManager *manager, void *ptr) {
    for (size_t i = 0; i < manager->max_arenas; i++) {
        ptr_t base = manager->arenas[i].base;
        size_t size = manager->arenas[i].size;

        // Check if the pointer is within this arena's memory range.
        if (base != (ptr_t)NULL && ptr >= base && ptr < (ptr_t)((char*)base + size)) {
            // Found the arena, free it (reset completely).
            arena_free(&manager->arenas[i]);
            sysarena_defragment(manager); // Attempt defragmentation after freeing.
            return;
        }
    }
}

// Copies data from one arena struct to another.
void copy_arena(Arena *dest, Arena *src) {
    dest->in_use = src->in_use;
    dest->base = src->base;
    dest->size = src->size;
    dest->used = src->used;
}

// Displaces arenas to reorder them.
void sysarena_displacement(ArenaManager *manager, size_t where) {
    if (where >= manager->max_arenas) {
        return; // Index out of bounds.
    }
    // Move arenas from right to left from the end towards 'where'.
    for (size_t i = (manager->max_arenas - 1); i > where; i--) {
        if (i > 0) { // Ensure i-1 is a valid index.
            copy_arena(&manager->arenas[i], &manager->arenas[i - 1]);
        }
    }
}