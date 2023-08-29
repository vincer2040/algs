#include "algs.h"

static void pre_walk(BinaryNode* cur, vec* vec) {
    if (!cur) {
        return;
    }

    vec_push(&vec, cur->data);
    pre_walk(cur->left, vec);
    pre_walk(cur->right, vec);
}

vec* pre_order_search(BinaryNode* head, size_t data_size) {
    vec* vec = vec_new(data_size, 32);
    if (vec == NULL) {
        return NULL;
    }
    pre_walk(head, vec);
    return vec;
}
