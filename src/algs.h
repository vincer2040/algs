#ifndef __ALGS_H__
#define __ALGS_H__

#include <stdbool.h>
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

typedef struct BinaryNode {
    struct BinaryNode* left;
    struct BinaryNode* right;
    unsigned char data[];
} BinaryNode;

typedef struct {
    size_t len;
    size_t data_size;
    Node* head;
    Node* tail;
} Queue;

typedef struct {
    size_t len;
    size_t data_size;
    Node* head;
} Stack;

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char data[];
} vec;

ssize_t binary_search(void* haystack, void* needle, size_t len,
                      size_t data_size, CmpFn fn);

void bubble_sort(void* arr, size_t len, size_t data_size, CmpFn fn);
void quick_sort(void* arr, size_t arr_len, size_t data_size, CmpFn* fn);

Queue queue_new(size_t data_size);
int queue_peek(Queue* q, void* out);
int queue_deque(Queue* q, void* out);
int queue_enque(Queue* q, void* data);
void queue_free(Queue* q, FreeFn* fn);

Stack stack_new(size_t data_size);
int stack_push(Stack* s, void* data);
int stack_pop(Stack* s, void* out);
int stack_peek(Stack* s, void* out);
void stack_free(Stack* s, FreeFn* fn);

BinaryNode* binary_node_new(void* data, size_t data_size);

vec* vec_new(size_t data_size, size_t initial_cap);
int vec_push(vec** vec, void* data);
void vec_free(vec* vec, FreeFn* fn);

vec* pre_order_search(BinaryNode* head, size_t data_size);
vec* in_order_search(BinaryNode* head, size_t data_size);
vec* post_order_search(BinaryNode* head, size_t data_size);
void binary_tree_free(BinaryNode* head, FreeFn* fn);

bool bt_bfs(BinaryNode* head, void* needle, CmpFn* fn);

bool bt_compare(BinaryNode* a, BinaryNode* b, CmpFn* fn);

#endif /*__ALGS_H__*/
