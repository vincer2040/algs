#include "algs.h"
#include <stddef.h>
#include <sys/types.h>

ssize_t binary_search(void* haystack, void* needle, size_t len,
                      size_t data_size, CmpFn* fn) {
    size_t lo = 0;
    size_t hi = len;
    unsigned char* h = ((unsigned char*)haystack);

    do {
        size_t m = (lo + (hi - lo) / 2);
        void* v = h + (m * data_size);
        int cmp = fn(v, needle);
        if (cmp == 0) {
            return m;
        } else if (cmp > 0) {
            hi = m;
        } else {
            lo = m + 1;
        }
    } while (lo < hi);

    return -1;
}
