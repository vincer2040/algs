#ifndef __ALGS_H__
#define __ALGS_H__

#include <stddef.h>
#include <sys/types.h>

typedef int CmpFn(void* a, void* b);

ssize_t binary_search(void* haystack, void* needle, size_t len,
                      size_t data_size, CmpFn fn);

#endif /*__ALGS_H__*/
