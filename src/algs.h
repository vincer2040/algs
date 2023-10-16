#ifndef __ALGS_H__
#define __ALGS_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

#define HT_SEED_SIZE 16
#define HT_RESIZABLE 1
#define HT_NOT_RESIZABLE 0

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

typedef struct ListNode {
    struct ListNode* next;
    struct ListNode* prev;
    unsigned char data[];
} ListNode;

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

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char data[];
} MinHeap;

typedef struct {
    uint64_t from;
    uint64_t to;
    uint64_t weight;
} GraphEdge;

typedef struct {
    size_t key_len;
    unsigned char data[];
} HtEntry;

typedef struct {
    size_t len;
    size_t cap;
    HtEntry** entries;
} HtBucket;

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    int resizable;
    unsigned char seed[HT_SEED_SIZE];
    HtBucket* buckets;
} Ht;

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    ListNode* head;
    ListNode* tail;
    Ht* lookup;
    Ht* reverse_lookup;
} LRU;

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
ssize_t vec_find(vec* vec, void* cmp_data, CmpFn* cmp_fn);
int vec_set_at(vec* vec, size_t idx, void* data, FreeFn* free_fn);
void vec_free(vec* vec, FreeFn* fn);

vec* pre_order_search(BinaryNode* head, size_t data_size);
vec* in_order_search(BinaryNode* head, size_t data_size);
vec* post_order_search(BinaryNode* head, size_t data_size);
void binary_tree_free(BinaryNode* head, FreeFn* fn);

bool bt_bfs(BinaryNode* head, void* needle, CmpFn* fn);

bool bt_compare(BinaryNode* a, BinaryNode* b, CmpFn* fn);

MinHeap* minheap_new(size_t data_size, size_t initial_cap);
int minheap_insert(MinHeap** heap, void* value, CmpFn* cmp);
int minheap_delete(MinHeap** heap, void* out, CmpFn* cmp);
void minheap_free(MinHeap* heap, FreeFn* cb);

void get_random_bytes(uint8_t* p, size_t len);

Ht* ht_new(size_t data_size, int resizable, size_t inital_cap);
int ht_insert(Ht* ht, unsigned char* key, size_t key_len, void* value,
              FreeFn* free_fn);
int ht_has(Ht* ht, unsigned char* key, size_t key_len);
void* ht_get(Ht* ht, unsigned char* key, size_t key_len);
int ht_delete(Ht* ht, unsigned char* key, size_t key_len, FreeFn* key_free_fn,
              FreeFn* value_free_fn);
size_t ht_len(Ht* ht);
size_t ht_capacity(Ht* ht);
void ht_free(Ht* ht, FreeFn* key_free_fn, FreeFn* value_free_fn);

LRU* lru_new(size_t capacity, size_t data_size);
int lru_update(LRU* lru, unsigned char* key, size_t key_len, void* data,
               FreeFn* free_fn);
void* lru_get(LRU* lru, unsigned char* key, size_t key_len);
void lru_free(LRU* lru, FreeFn* free_fn);

#endif /*__ALGS_H__*/
