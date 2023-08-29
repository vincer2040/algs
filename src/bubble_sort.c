#include "algs.h"
#include <stddef.h>

void bubble_sort(void* arr, size_t len, size_t data_size, CmpFn* fn) {
    size_t i, j;
    unsigned char* uarr = ((unsigned char*)arr);

    for (i = 0; i < len; ++i) {
        size_t jlen = len - 1 - i;
        for (j = 0; j < jlen; ++j) {
            size_t o1 = data_size * j;
            size_t o2 = data_size * (j + 1);
            void* a = uarr + o1;
            void* b = uarr + o2;
            int cmp = fn(a, b);
            if (cmp > 0) {
                swap(a, b, data_size);
            }
        }
    }
}
