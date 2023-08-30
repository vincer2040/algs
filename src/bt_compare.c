#include "algs.h"

bool bt_compare(BinaryNode* a, BinaryNode* b, CmpFn* fn) {
    int cmp;

    if ((a == NULL) && (b == NULL)) {
        return true;
    }

    if ((a == NULL) || (b == NULL)) {
        return false;
    }

    cmp = fn(a->data, b->data);
    if (cmp != 0) {
        return false;
    }

    return bt_compare(a->left, b->left, fn) &&
           bt_compare(a->right, b->right, fn);
}
