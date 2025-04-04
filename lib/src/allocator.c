#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "../include/allocator.h"

const uint32_t MEMINCR = 4;

struct Arena {
    struct Arena* next;
    char* avail; /* points to first available memory location */
    char* limit; /* points to first unavailable memory location */
};

struct ArenaList* allocator_create() {
    struct Arena* arena = malloc(sizeof(struct Arena));
    arena->next = NULL;
    arena->avail = (void*) arena + sizeof(struct Arena);
    arena->limit = arena->avail;

    struct ArenaList* arena_list = malloc(sizeof(struct ArenaList));
    arena_list->head = arena;
    arena_list->tail = arena;

    return arena_list;
}

void* allocator_alloc(struct ArenaList* arena_list, uint32_t size) {
    assert(size != 0);

    struct Arena* arena = arena_list->tail;

    for (; arena->avail + size > arena->limit; arena = arena->next) {
        if (arena->next != NULL) {
            arena->next->avail = (void*) arena->next + sizeof(struct Arena);
        }
        else {
            uint32_t final_size = ((size + 7) & ~3) + MEMINCR * 1024 + sizeof(struct Arena);

            struct Arena* new_arena = malloc(final_size);
            new_arena->next = NULL;
            new_arena->avail = (void*) new_arena + sizeof(struct Arena);
            new_arena->limit = (void*) new_arena + final_size;

            arena->next = new_arena;
        }
    }

    arena_list->tail = arena;
    void* p = arena->avail;
    arena->avail += size;

    return p;
}

void allocator_dealloc(struct ArenaList* arena_list) {
    if ((arena_list->tail = arena_list->head->next)) {
        arena_list->tail->avail = (void*) arena_list->tail + sizeof(struct Arena);
    }
    else {
        arena_list->tail = arena_list->head;
    }
}
