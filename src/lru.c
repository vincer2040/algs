#include "algs.h"
#include "vstring.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#define lru_node_size(size) (sizeof(ListNode) + size);

static void lru_reverse_lookup_free_fn(void* ptr);
static ListNode* list_node_new(void* data, size_t data_size);
static void lru_detach(LRU* lru, ListNode* node);
static void lru_prepend(LRU* lru, ListNode* node);
static void lru_trim_cache(LRU* lru);

LRU* lru_new(size_t capacity, size_t data_size) {
    LRU* lru;
    // size_t node_size;
    lru = calloc(1, sizeof *lru);
    if (lru == NULL) {
        return NULL;
    }

    lru->len = 0;
    lru->cap = capacity;
    lru->data_size = data_size;
    lru->head = lru->tail = NULL;

    // node_size = lru_node_size(data_size);
    lru->lookup = ht_new(sizeof(ListNode*), HT_NOT_RESIZABLE, capacity);
    if (lru->lookup == NULL) {
        return NULL;
    }
    lru->reverse_lookup = ht_new(sizeof(vstr), HT_NOT_RESIZABLE, capacity);
    if (lru->reverse_lookup == NULL) {
        ht_free(lru->lookup, NULL, NULL);
        free(lru);
        return NULL;
    }
    return lru;
}

int lru_update(LRU* lru, unsigned char* key, size_t key_len, void* data, FreeFn* free_fn) {
    ListNode** node = ht_get(lru->lookup, key, key_len);
    if (node == NULL) {
        ListNode* new_node = list_node_new(data, lru->data_size);
        vstr vstr_key = vstr_new_len(key_len);
        int insert_res;
        vstr_key = vstr_push_string_len(vstr_key, (char*)key, key_len);
        if (new_node == NULL) {
            return -1;
        }
        lru->len++;
        lru_prepend(lru, new_node);
        lru_trim_cache(lru);
        insert_res = ht_insert(lru->lookup, key, key_len, &new_node, NULL);
        if (insert_res != 0) {
            return insert_res;
        }
        insert_res = ht_insert(lru->reverse_lookup, (unsigned char*)(&new_node), sizeof(ListNode*), &vstr_key, NULL);
        return insert_res;
    } else {
        lru_detach(lru, *node);
        lru_prepend(lru, *node);
        if (free_fn) {
            free_fn((*node)->data);
        }
        memcpy((*node)->data, data, lru->data_size);
        return 0;
    }
    return 0;
}

void* lru_get(LRU* lru, unsigned char* key, size_t key_len) {
    ListNode** node = ht_get(lru->lookup, key, key_len);
    if (node == NULL) {
        return NULL;
    }
    lru_detach(lru, *node);
    lru_prepend(lru, *node);
    return (*node)->data;
}

void lru_free(LRU* lru, FreeFn* free_fn) {
    ((void)free_fn);
    ht_free(lru->lookup, NULL, NULL);
    ht_free(lru->reverse_lookup, NULL, lru_reverse_lookup_free_fn);
}

static void lru_detach(LRU* lru, ListNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (lru->head == node) {
        lru->head = lru->head->next;
    }
    if (lru->tail == node) {
        lru->tail = lru->tail->prev;
    }
}

static void lru_prepend(LRU* lru, ListNode* node) {
    if (lru->head == NULL) {
        lru->head = lru->tail = node;
        return;
    }
    lru->head->prev = node;
    node->next = lru->head;
    lru->head = node;
}

static void lru_trim_cache(LRU* lru) {
    ListNode* tail;
    vstr* key_ptr;
    size_t key_len;
    if (lru->len <= lru->cap) {
        return;
    }

    tail = lru->tail;
    lru_detach(lru, tail);

    key_ptr = ht_get(lru->reverse_lookup, (unsigned char*)(&tail), sizeof(ListNode*));
    assert(key_ptr != NULL);
    key_len = vstr_len(*key_ptr);
    ht_delete(lru->lookup, (unsigned char*)(*key_ptr), key_len, NULL, NULL);
    ht_delete(lru->reverse_lookup, (unsigned char*)(&tail), sizeof(ListNode*), NULL, NULL);
    lru->len--;
}

static void lru_reverse_lookup_free_fn(void* ptr) {
    vstr s = *((vstr*)ptr);
    vstr_delete(s);
}

static ListNode* list_node_new(void* data, size_t data_size) {
    ListNode* node;
    size_t size = lru_node_size(data_size);
    node = malloc(size);
    if (node == NULL) {
        return NULL;
    }

    memcpy(node->data, data, data_size);
    node->prev = node->next = NULL;
    return node;
}
