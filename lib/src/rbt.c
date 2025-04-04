#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "../include/rbt.h"
#include "../include/allocator.h"

#define RED 0
#define BLACK 1

/* Helper functions */
void _rbt_rotate_left(struct Rbt *rbt, struct RbtNode *x) {
    assert(rbt != NULL && x != NULL);

    struct RbtNode* y = x->r;
    struct RbtNode* z = y->l;

    if (x->p == rbt->nil) {
        rbt->root = y;
    }
    else if (x == x->p->l) {
        x->p->l = y;
    }
    else if (x == x->p->r) {
        x->p->r = y;
    }
    else {
        abort();
    }

    y->p = x->p;
    y->l = x;

    x->p = y;
    x->r = z;

    z->p = x;
}
void _rbt_rotate_right(struct Rbt *rbt, struct RbtNode *x) {
    assert(rbt != NULL && x != NULL);

    struct RbtNode* y = x->l;
    struct RbtNode* z = y->r;

    if (x->p == rbt->nil) {
        rbt->root = y;
    }
    else if (x == x->p->l) {
        x->p->l = y;
    }
    else if (x == x->p->r) {
        x->p->r = y;
    }
    else {
        abort();
    }

    y->p = x->p;
    y->r = x;

    x->p = y;
    x->l = z;

    z->p = x;
}
void _rbt_transplant(struct Rbt* rbt, struct RbtNode* n, struct RbtNode* m) {
    assert(rbt != NULL && n != NULL && m != NULL);

    if (n->p == rbt->nil) {
        rbt->root = m;
    }
    else if (n == n->p->l) {
        n->p->l = m;
    }
    else if (n == n->p->r) {
        n->p->r = m;
    }
    else {
        abort();
    }
    m->p = n->p;
}
struct RbtNode* _rbt_min(struct Rbt* rbt, struct RbtNode* x) {
    assert(rbt != NULL && x != NULL && x != rbt->nil);

    while(x->l != rbt->nil) {
        x = x->l;
    }

    return x;
}


/* Create Red Black Tree */
struct Rbt* rbt_create(struct ArenaList* arena_list) {
    assert(arena_list != NULL);

    struct Rbt* rbt = allocator_alloc(arena_list, sizeof(struct Rbt));
    struct RbtNode* nil = allocator_alloc(arena_list, sizeof(struct RbtNode));

    nil->c = BLACK;
    nil->k = "";
    nil->v = NULL;
    nil->l = NULL;
    nil->r = NULL;
    nil->p = NULL;

    rbt->arena_list = arena_list;
    rbt->nil = nil;
    rbt->root = nil;

    return rbt;
}

/* Insert functions */
void _rbt_insert_fixup(struct Rbt* rbt, struct RbtNode* z) {
    assert(rbt != NULL && z != NULL && z->c == RED);

    while (z->p->c == RED) {
        if (z->p == z->p->p->l) {
            struct RbtNode* y = z->p->p->r;
            if (y->c == RED) {
                y->c = BLACK;
                z->p->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->r) {
                    z = z->p;
                    _rbt_rotate_left(rbt, z);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                _rbt_rotate_right(rbt, z->p->p);
            }
        }
        else {
            struct RbtNode* y = z->p->p->l;
            if (y->c == RED) {
                y->c = BLACK;
                z->p->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->l) {
                    z = z->p;
                    _rbt_rotate_right(rbt, z);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                _rbt_rotate_left(rbt, z->p->p);
            }
        }
    }

    rbt->root->c = BLACK;
}

void rbt_insert(struct Rbt* rbt, char* key, void* value) {
    assert(rbt != NULL && key != NULL && value != NULL);

    struct RbtNode* curr = rbt->root;
    struct RbtNode* y = curr;

    int cmp;
    while (curr != rbt->nil) {
        y = curr;
        cmp = strcmp(key, curr->k);
        if (cmp == 0) {
            curr->v = value;
            return;
        }
        else if (cmp < 0) {
            curr = curr->l;
        }
        else {
            curr = curr->r;
        }
    }

    struct RbtNode* z = allocator_alloc(rbt->arena_list, sizeof(struct RbtNode));
    z->p = y;
    z->l = rbt->nil;
    z->r = rbt->nil;
    z->k = key;
    z->v = value;
    z->c = RED;

    if (y == rbt->nil) {
        rbt->root = z;
    }
    else if (cmp < 0) {
        y->l = z;
    }
    else {
        y->r = z;
    }

    _rbt_insert_fixup(rbt, z);
}

/* Delete functions */
void _rbt_delete_fixup(struct Rbt* rbt, struct RbtNode* x) {
    assert(rbt != NULL && x != NULL);

    while (x != rbt->root && x->c == BLACK) {
        if (x == x->p->l) {
            struct RbtNode* w = x->p->r;
            if (w->c == RED) {
                w->p->c = RED;
                w->c = BLACK;
                _rbt_rotate_left(rbt, w->p);
                w = x->p->r;
            }
            if (w->l->c == BLACK && w->r->c == BLACK) {
                w->c = RED;
                x = x->p;
            }
            else {
                if (w->r->c == BLACK) {
                    w->l->c = BLACK;
                    w->c = RED;
                    _rbt_rotate_right(rbt, w);
                    w = x->p->r;
                }
                w->c = w->p->c;
                w->p->c = BLACK;
                w->r->c = BLACK;
                _rbt_rotate_left(rbt, w->p);
                x = rbt->root;
            }
        }
        else {
            struct RbtNode* w = x->p->l;
            if (w->c == RED) {
                w->p->c = RED;
                w->c = BLACK;
                _rbt_rotate_right(rbt, w->p);
                w = x->p->l;
            }
            if (w->l->c == BLACK && w->r->c == BLACK) {
                w->c = RED;
                x = x->p;
            }
            else {
                if (w->l->c == BLACK) {
                    w->r->c = BLACK;
                    w->c = RED;
                    _rbt_rotate_left(rbt, w);
                    w = x->p->l;
                }
                w->c = w->p->c;
                w->p->c = BLACK;
                w->l->c = BLACK;
                _rbt_rotate_right(rbt, w->p);
                x = rbt->root;
            }
        }
    }

    x->c = BLACK;
}

void rbt_delete(struct Rbt* rbt, char* key) {
    assert(rbt != NULL && key != NULL);

    struct RbtNode* z = rbt_get(rbt, key);
    if (z == rbt->nil) {
        return;
    }

    struct RbtNode* y = z;
    int y_original_color = y->c;
    struct RbtNode* x;

    if (y->l == rbt->nil) {
        x = y->r;
        _rbt_transplant(rbt, y, x);
    }
    else if (y->r == rbt->nil) {
        x = y->l;
        _rbt_transplant(rbt, y, x);
    }
    else {
        y = _rbt_min(rbt, y->r);
        y_original_color = y->c;
        x = y->r;

        if (y == z->r) {
            x->p = y;
        }
        else {
            _rbt_transplant(rbt, y, x);
            y->r = z->r;
            y->r->p = y;
        }
        _rbt_transplant(rbt, z, y);
        y->l = z->l;
        y->l->p = y;
        y->c = z->c;
    }

    if (y_original_color == BLACK) {
        _rbt_delete_fixup(rbt, x);
    }
}

/* Get function */
struct RbtNode* rbt_get(struct Rbt* rbt, char* key) {
    assert(rbt != NULL && key != NULL);

    struct RbtNode* curr = rbt->root;
    while (curr != rbt->nil) {
        int cmp = strcmp(key, curr->k);
        if (cmp == 0) {
            return curr;
        }
        else if (cmp < 0) {
            curr = curr->l;
        }
        else {
            curr = curr->r;
        }
    }
    
    return rbt->nil;
}

/* Print function */
void rbt_print(struct Rbt* rbt, struct RbtNode* x) {
    assert(rbt != NULL && x != NULL);

    if (x == rbt->nil) {
        return;
    }

    rbt_print(rbt, x->l);
    printf("\tkey=%s; value=%s\n", x->k, x->v);
    rbt_print(rbt, x->r);
}
