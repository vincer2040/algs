#ifndef __ALGS_H__
#define __ALGS_H__

#include <stddef.h>
#include <sys/types.h>

#define swap(a1, b1, s)                                                        \
    {                                                                          \
        size_t k;                                                              \
        unsigned char* uca = ((unsigned char*)a1);                             \
        unsigned char* ucb = ((unsigned char*)b1);                             \
        for (k = 0; k < s; ++k) {                                              \
            unsigned char tmp = uca[k];                                        \
            uca[k] = ucb[k];                                                   \
            ucb[k] = tmp;                                                      \
        }                                                                      \
    }

typedef int CmpFn(void* a, void* b);

ssize_t binary_search(void* haystack, void* needle, size_t len,
                      size_t data_size, CmpFn fn);

void bubble_sort(void* arr, size_t len, size_t data_size, CmpFn fn);

#endif /*__ALGS_H__*/
