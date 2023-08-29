#include "algs.h"

void in_walk(BinaryNode* cur, vec* vec) {
    if (!cur) {
        return;
    }

    in_walk(cur->left, vec);
    vec_push(&vec, cur->data);
    in_walk(cur->right, vec);
}

vec* in_order_search(BinaryNode* head, size_t data_size) {
    vec* vec = vec_new(data_size, 32);
    if (vec == NULL) {
        return NULL;
    }
    in_walk(head, vec);
    return vec;
}
