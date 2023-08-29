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
typedef void FreeFn(void* ptr);

typedef struct Node {
    struct Node* next;
    unsigned char data[];
} Node;

typedef struct {
    size_t len;
    size_t data_size;
    Node* head;
    Node* tail;
} Queue;

ssize_t binary_search(void* haystack, void* needle, size_t len,
                      size_t data_size, CmpFn fn);

void bubble_sort(void* arr, size_t len, size_t data_size, CmpFn fn);

Queue queue_new(size_t data_size);
int queue_peek(Queue* q, void* out);
int queue_deque(Queue* q, void* out);
int queue_enque(Queue* q, void* data);
void queue_free(Queue* q, FreeFn* fn);

#endif /*__ALGS_H__*/
