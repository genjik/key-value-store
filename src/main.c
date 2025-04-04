#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/include/rbt.h"


int main() {
    struct ArenaList* arena_list = allocator_create();
    struct Rbt* rbt              = rbt_create(arena_list);
    
    char* action = allocator_alloc(arena_list, 1024);
    for (;;) {
        printf("Enter action (insert, get, delete, print, or quit): ");
        if(!fgets(action, 1024, stdin)) {
            abort();
        }
        action[strlen(action) - 1] = '\0';

        if (strcmp(action, "insert") == 0) {
            printf("Enter key: ");
            char* key = allocator_alloc(arena_list, 1024);
            fgets(key, 1024, stdin);
            key[strlen(key) - 1] = '\0';
             
            printf("Enter value: ");
            char* value = allocator_alloc(arena_list, 1024);
            fgets(value, 1024, stdin);
            value[strlen(value) - 1] = '\0';

            rbt_insert(rbt, key, value);
        }
        else if (strcmp(action, "get") == 0) {
            printf("Enter key: ");
            char* key = allocator_alloc(arena_list, 1024);
            fgets(key, 1024, stdin);
            key[strlen(key) - 1] = '\0';

            struct RbtNode* x = rbt_get(rbt, key);
            if (x != NULL && x != rbt->nil) {
                printf("\tPair Found: key=%s | value=%s\n", x->k, (char*) x->v);
            }
            else {
                printf("\tPair Not Found\n");
            }
        }
        else if (strcmp(action, "delete") == 0) {
            printf("Enter key: ");
            char* key = allocator_alloc(arena_list, 1024);
            fgets(key, 1024, stdin);
            key[strlen(key) - 1] = '\0';

            rbt_delete(rbt, key);
            printf("Deleted key=%s\n", key);
        }
        else if (strcmp(action, "print") == 0) {
            rbt_print(rbt, rbt->root);
        }
        else if (strcmp(action, "quit") == 0) {
            allocator_dealloc(arena_list);
            exit(0);
        }
    }
    
    return 0;
}
