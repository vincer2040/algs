#include "algs.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

MinHeap* minheap_new(size_t data_size, size_t initial_cap) {
    MinHeap* heap;
    size_t needed;

    if (initial_cap == 0) {
        fprintf(stderr, "cannot pass 0 as initial capacity to heap_new\n");
        return NULL;
    }

    needed = sizeof(MinHeap) + (data_size * initial_cap);

    heap = malloc(needed);

    if (heap == NULL) {
        fprintf(stderr, "failed to allocate memory for new heap\n");
        return NULL;
    }

    memset(heap, 0, needed);

    heap->cap = initial_cap;
    heap->data_size = data_size;

    return heap;
}

static inline size_t minheap_parent(size_t idx) { return (idx - 1) / 2; }

static inline size_t minheap_left_child(size_t idx) { return (idx * 2) + 1; }

static inline size_t minheap_right_child(size_t idx) { return (idx * 2) + 2; }

static void minheapify_up(MinHeap** heap, size_t idx, CmpFn* cmp) {
    size_t p, data_size;
    void *pv, *v;

    data_size = (*heap)->data_size;

    if (idx == 0) {
        return;
    }

    for (; idx > 0; idx--) {

        p = minheap_parent(idx);
        pv = (*heap)->data + (p * data_size);
        v = (*heap)->data + (idx * data_size);

        if (cmp(pv, v) > 0) {
            swap(pv, v, data_size);
        }
    }
}

static void minheapify_down(MinHeap** heap, size_t idx, CmpFn* cmp) {
    size_t lidx, ridx, len, data_size;
    void *lv, *rv, *v;

    len = (*heap)->len;
    data_size = (*heap)->data_size;

    if (idx >= len) {
        return;
    }

    while (idx < len) {

        lidx = minheap_left_child(idx);
        if (lidx >= len) {
            return;
        }

        ridx = minheap_right_child(idx);
        if (ridx >= len) {
            return;
        }

        lv = (*heap)->data + (lidx * data_size);
        rv = (*heap)->data + (ridx * data_size);
        v = (*heap)->data + (idx * data_size);

        if ((cmp(lv, rv) > 0) && (cmp(v, rv) > 0)) { // right is smallest
            swap(rv, v, data_size);
            idx = ridx;
        } else if ((cmp(rv, lv) > 0) && (cmp(v, lv) > 0)) { // left is smallest
            swap(lv, v, data_size);
            idx = lidx;
        } else {
            break;
        }
    }
}

int minheap_insert(MinHeap** heap, void* value, CmpFn* cmp) {
    size_t ins, cap, data_size;
    ins = (*heap)->len;
    cap = (*heap)->cap;
    data_size = (*heap)->data_size;
    if (ins == cap) {
        size_t needed;
        void* tmp;
        cap <<= 1;
        needed = sizeof(MinHeap) + (cap * data_size);

        tmp = realloc(*heap, needed);
        if (tmp == NULL) {
            return -1;
        }
        *heap = tmp;

        memset((*heap)->data + (ins * data_size), 0, (cap - ins) * data_size);

        (*heap)->cap = cap;
    }

    memcpy((*heap)->data + (ins * data_size), value, data_size);
    minheapify_up(heap, ins, cmp);
    (*heap)->len++;
    return 0;
}

int minheap_delete(MinHeap** heap, void* out, CmpFn* cmp) {
    size_t data_size, len;
    len = (*heap)->len;
    data_size = (*heap)->data_size;
    if (len == 0) {
        return -1;
    }

    memcpy(out, (*heap)->data, data_size);
    if (len == 1) {
        memset((*heap)->data, 0, data_size);
        (*heap)->len--;
        return 0;
    }
    (*heap)->len--;
    swap((*heap)->data, (*heap)->data + ((*heap)->len * data_size), data_size);
    memset((*heap)->data + (len * data_size), 0, data_size);
    minheapify_down(heap, 0, cmp);

    return 0;
}

void minheap_free(MinHeap* heap, FreeFn* cb) {
    size_t i, len, data_size;
    data_size = heap->data_size;
    len = heap->len * data_size;

    if (cb) {
        for (i = 0; i < len; i += data_size) {
            void* at = heap->data + i;
            cb(at);
        }
    }

    free(heap);
    heap = NULL;
}
