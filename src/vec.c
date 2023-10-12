#include "algs.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

vec* vec_new(size_t data_size, size_t initial_cap) {
    size_t size;
    vec* v;
    assert(initial_cap > 0);

    size = sizeof(vec) + (data_size * initial_cap);
    v = malloc(size);
    if (v == NULL) {
        return NULL;
    }

    memset(v, 0, size);
    v->data_size = data_size;
    v->cap = initial_cap;

    return v;
}

int vec_push(vec** vec, void* data) {
    size_t len, cap, data_size;
    len = (*vec)->len;
    cap = (*vec)->cap;
    data_size = (*vec)->data_size;
    if (len == cap) {
        size_t size;
        void* tmp;
        cap <<= 1; // mulitply by two
        size = sizeof(vec) + (data_size * cap);
        tmp = realloc(*vec, size);
        if (tmp == NULL) {
            return -1;
        }
        *vec = tmp;
        (*vec)->cap = cap;
    }

    memcpy((*vec)->data + (len * data_size), data, data_size);
    (*vec)->len++;
    return 0;
}

ssize_t vec_find(vec* vec, void* cmp_data, CmpFn* cmp_fn) {
    size_t i, len;
    len = vec->len;
    if (len == 0) {
        return -1;
    }
    for (i = 0; i < len; ++i) {
        void* at = vec->data + (i * vec->data_size);
        int cmp = cmp_fn(cmp_data, at);
        if (cmp == 0) {
            return i;
        }
    }
    return -1;
}

int vec_set_at(vec* vec, size_t idx, void* data, FreeFn* free_fn) {
    void* ptr;
    size_t data_size = vec->data_size;
    if (vec->len >= idx) {
        return -1;
    }
    ptr = vec->data + (idx * data_size);
    if (free_fn) {
        free_fn(ptr);
    }
    memcpy(vec->data + (idx * data_size), data, data_size);
    return 0;
}

void vec_free(vec* vec, FreeFn* fn) {
    if (fn) {
        size_t i, len, data_size;
        len = vec->len;
        data_size = vec->data_size;

        for (i = 0; i < len; ++i) {
            void* at = vec->data + (i * data_size);
            fn(at);
        }
    }

    free(vec);
}
