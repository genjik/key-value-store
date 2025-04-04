#include <stdint.h>
#include "../include/allocator.h"

#ifndef RBT
#define RBT

struct Rbt {
    struct RbtNode* root;
    struct RbtNode* nil;
    struct ArenaList* arena_list;
};
struct RbtNode {
    struct RbtNode* p;
    struct RbtNode* l;
    struct RbtNode* r;
    char* k; /* NULL terminated string */
    void* v;
    uint8_t c; /* Red = 0, Black = 1 */
};

struct Rbt*     rbt_create(struct ArenaList* arena_list);
void            rbt_insert(struct Rbt* rbt, char* key, void* value);
void            rbt_delete(struct Rbt* rbt, char* key);
struct RbtNode* rbt_get(struct Rbt* rbt, char* key);
void            rbt_print(struct Rbt* rbt, struct RbtNode* node);

#endif
