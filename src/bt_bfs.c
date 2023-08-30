#include "algs.h"
#include <stdbool.h>
#include <stdio.h>

bool bt_bfs(BinaryNode* head, void* needle, CmpFn* fn) {
    Queue q;
    q = queue_new(sizeof(BinaryNode*));

    // we have to pass head by pointer because it
    // has a flexible array membor and breaks
    // on deque when dequing into a non ptr
    queue_enque(&q, &head);

    while (q.len) {
        BinaryNode* cur;
        int cmp;
        queue_deque(&q, &cur);
        cmp = fn(cur->data, needle);
        if (cmp == 0) {
            return true;
        }

        if (cur->left) {
            queue_enque(&q, &(cur->left));
        }
        if (cur->right) {
            queue_enque(&q, &(cur->right));
        }
    }

    return false;
}
