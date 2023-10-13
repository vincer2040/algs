#include "algs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool bt_bfs(BinaryNode* head, void* needle, CmpFn* fn) {
    Queue q;
    q = queue_new(sizeof(BinaryNode*));

    // we have to pass head by pointer because it
    // has a flexible array member and breaks
    // on deque when dequing into a non ptr
    queue_enque(&q, &head);

    while (q.len) {
        BinaryNode* cur;
        int cmp;
        queue_deque(&q, &cur);
        cmp = fn(cur->data, needle);
        if (cmp == 0) {
            queue_free(&q, NULL);
            return true;
        }

        if (cur->left) {
            queue_enque(&q, &(cur->left));
        }
        if (cur->right) {
            queue_enque(&q, &(cur->right));
        }
    }

    queue_free(&q, NULL);
    return false;
}
