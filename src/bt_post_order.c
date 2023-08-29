#include "algs.h"

static void post_walk(BinaryNode* cur, vec* vec) {
    if (!cur) {
        return;
    }

    post_walk(cur->left, vec);
    post_walk(cur->right, vec);
    vec_push(&vec, cur->data);
}

vec* post_order_search(BinaryNode* head, size_t data_size) {
    vec* vec = vec_new(data_size, 32);
    if (vec == NULL) {
        return NULL;
    }
    post_walk(head, vec);
    return vec;
}
