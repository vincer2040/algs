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
