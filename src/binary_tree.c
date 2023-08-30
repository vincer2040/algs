#include "algs.h"
#include <memory.h>
#include <stdlib.h>

BinaryNode* binary_node_new(void* data, size_t data_size) {
    BinaryNode* bn;
    size_t size = sizeof(BinaryNode) + data_size;

    bn = malloc(size);
    if (bn == NULL) {
        return NULL;
    }
    memset(bn, 0, data_size);
    memcpy(bn->data, data, data_size);
    bn->right = NULL;
    bn->left = NULL;

    return bn;
}

static void free_walk(BinaryNode* cur, FreeFn* fn) {
    if (!cur) {
        return;
    }

    free_walk(cur->left, fn);
    free_walk(cur->right, fn);
    if (fn) {
        fn(cur->data);
    }

    free(cur);
}

void binary_tree_free(BinaryNode* head, FreeFn* fn) {
    free_walk(head, fn);
}
