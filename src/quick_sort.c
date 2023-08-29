#include "algs.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

ssize_t partition(void* arr, ssize_t lo, ssize_t hi, size_t data_size, CmpFn* fn) {
    unsigned char* uarr = ((unsigned char*)arr);
    void *x, *pivot = uarr + (hi * data_size);
    size_t i;
    ssize_t idx = lo - 1;

    for (i = lo; i < hi; ++i) {
        void* at = uarr + (i * data_size);
        int cmp = fn(at, pivot);
        if (cmp <= 0) {
            void *a, *b;
            idx++;
            a = uarr + (i * data_size);
            b = uarr + (idx * data_size);
            swap(a, b, data_size);
        }
    }

    idx++;

    x = uarr + (idx * data_size);
    swap(pivot, x, data_size);
    return idx;
}

void qs(void* arr, ssize_t lo, ssize_t hi, size_t data_size, CmpFn* fn) {
    ssize_t pivot;
    if (lo >= hi) {
        return;
    }

    pivot = partition(arr, lo, hi, data_size, fn);
    qs(arr, lo, pivot - 1, data_size, fn);
    qs(arr, pivot + 1, hi, data_size, fn);
}

void quick_sort(void* arr, size_t arr_len, size_t data_size, CmpFn* fn) {
    qs(arr, 0, arr_len - 1, data_size, fn);
}
