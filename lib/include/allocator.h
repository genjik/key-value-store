#include <stdint.h>

#ifndef ALLOCATOR
#define ALLOCATOR

struct ArenaList {
    struct Arena* head;
    struct Arena* tail;
};

struct ArenaList* allocator_create();
void* allocator_alloc(struct ArenaList* arena_list, uint32_t size);
void allocator_dealloc(struct ArenaList* arena_list);

#endif
