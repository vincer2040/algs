#include "algs.h"
#include "sha256.h"
#include "siphash.h"
#include <stdint.h>
#include <stdlib.h>

#define HT_INITIAL_CAP 32
#define BUCKET_INITAL_CAP 5

#define ht_padding(size)                                                       \
    ((sizeof(void*) - ((size + 8) % sizeof(void*))) & (sizeof(void*) - 1))

static inline void entry_free(HtEntry* entry, FreeFn* free_fn);

Ht* ht_new(size_t data_size) {
    Ht* ht;
    ht = calloc(1, sizeof *ht);
    if (ht == NULL) {
        return NULL;
    }
    ht->data_size = data_size;
    ht->len = 0;
    ht->buckets = calloc(HT_INITIAL_CAP, sizeof(HtBucket));
    if (ht->buckets == NULL) {
        free(ht);
        return NULL;
    }
    ht->cap = HT_INITIAL_CAP;
    get_random_bytes(ht->seed, sizeof ht->seed);
    return ht;
}

static inline uint64_t ht_hash(Ht* ht, unsigned char* key, size_t key_len) {
    return siphash(key, key_len, ht->seed) % ht->cap;
}

static HtEntry* ht_entry_new(unsigned char* key, size_t key_len, void* value,
                             size_t data_size) {
    HtEntry* entry;
    size_t offset, needed;
    offset = key_len + ht_padding(key_len);
    needed = sizeof *entry + offset + data_size;
    entry = malloc(needed);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, needed);
    entry->key_len = key_len;
    memcpy(entry->data, key, key_len);
    memcpy(entry->data + offset, value, data_size);
    return entry;
}

static int ht_init_bucket(HtBucket* bucket) {
    bucket->entries = calloc(BUCKET_INITAL_CAP, sizeof(HtEntry*));
    if (bucket->entries == NULL) {
        return -1;
    }
    bucket->cap = BUCKET_INITAL_CAP;
    return 0;
}

static int ht_realloc_bucket(HtBucket* bucket) {
    size_t new_cap = bucket->cap << 1;
    void* tmp = realloc(bucket->entries, new_cap * sizeof(HtEntry*));
    if (tmp == NULL) {
        return -1;
    }
    bucket->cap = new_cap;
    bucket->entries = tmp;
    memset(&(bucket->entries[bucket->len]), 0,
           (bucket->cap - bucket->len) * sizeof(HtEntry*));
    return 0;
}

static inline void bucket_remove(HtBucket* bucket, size_t idx, FreeFn* free_fn) {
    size_t bucket_len = bucket->len;
    HtEntry* entry = bucket->entries[idx];
    if (free_fn) {
        size_t entry_key_len = entry->key_len;
        size_t offset = entry_key_len + ht_padding(entry_key_len);
        void* ptr = entry->data + offset;
        free_fn(ptr);
    }
    free(entry);
    bucket->len--;
    if (bucket->len == 0) {
        memset(bucket->entries, 0, sizeof(HtEntry*));
        return;
    }

    memmove(&(bucket->entries[idx]), &(bucket->entries[idx + 1]),
            (bucket_len - idx - 1) * sizeof(HtEntry*));

    memset(&(bucket->entries[bucket_len - 1]), 0, sizeof(HtEntry*));
}

int ht_resize(Ht* ht) {
    size_t i, cap = ht->cap;
    size_t new_cap = cap << 1;
    void* tmp = realloc(ht->buckets, new_cap * sizeof(HtBucket));
    if (tmp == NULL) {
        return -1;
    }

    ht->cap = new_cap;
    ht->buckets = tmp;
    memset(&(ht->buckets[cap]), 0, (new_cap - cap) * sizeof(HtBucket));

    for (i = 0; i < cap; ++i) {
        HtBucket* bucket = &(ht->buckets[i]);
        size_t j, len = bucket->len;

        for (j = 0; j < len; ++j) {
            HtEntry* entry = bucket->entries[j];
            size_t key_len = entry->key_len;
            unsigned char* key = entry->data;
            uint64_t hash = ht_hash(ht, key, key_len);
            if (hash == i) {
                size_t k;

                for (k = 0; k < j; ++k) {
                    HtEntry* cur = bucket->entries[k];
                    if (!cur) {
                        memmove(&(bucket->entries[k]), &(bucket->entries[j]), sizeof(HtEntry*));
                        memset(&(bucket->entries[j]), 0, sizeof(HtEntry*));
                        goto next;
                    }
                }
            } else {
                HtBucket* new_bucket = &(ht->buckets[hash]);
                size_t new_bucket_cap = new_bucket->cap, new_bucket_len = new_bucket->len;
                if (new_bucket_cap == 0) {
                    if (ht_init_bucket(bucket) == -1) {
                        return -1;
                    }
                }
                if (new_bucket_len == 0) {
                    new_bucket->entries[0] = entry;
                    memset(&(bucket->entries[j]), 0, sizeof(HtEntry*));
                    new_bucket->len++;
                    bucket->len--;
                    goto next;
                }
                if (new_bucket_len == new_bucket_cap) {
                    if (ht_realloc_bucket(new_bucket) == -1) {
                        return -1;
                    }
                }
                memmove(&(new_bucket->entries[new_bucket_len]), &(bucket->entries[j]), sizeof(HtEntry*));
                memset(&(bucket->entries[j]), 0, sizeof(HtEntry*));
                new_bucket->len++;
                bucket->len--;
            }

next:
            continue;
        }
    }

    return 0;
}

int ht_insert(Ht* ht, unsigned char* key, size_t key_len, void* value,
              FreeFn* free_fn) {
    uint64_t hash = ht_hash(ht, key, key_len);
    HtBucket* bucket = &(ht->buckets[hash]);
    size_t i, len = bucket->len, cap = bucket->cap;
    HtEntry* entry;

    if (cap == 0) {
        if (ht_init_bucket(bucket) == -1) {
            return -1;
        }
        entry = ht_entry_new(key, key_len, value, ht->data_size);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        ht->len++;
        return 0;
    }

    if (len == 0) {
        entry = ht_entry_new(key, key_len, value, ht->data_size);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        ht->len++;
        return 0;
    }

    for (i = 0; i < len; ++i) {
        HtEntry* cur = bucket->entries[i];
        size_t cur_key_len = cur->key_len;
        unsigned char* cur_key = cur->data;
        if ((cur_key_len == key_len) && (memcmp(key, cur_key, key_len) == 0)) {
            /* we have found equal keys */
            size_t offset = key_len + ht_padding(key_len);
            void* ptr = cur->data + offset;
            if (free_fn) {
                free_fn(ptr);
            }
            memcpy(cur->data + offset, value, ht->data_size);
            return 0;
        }
    }

    if (len == cap) {
        if (ht_realloc_bucket(bucket) == -1) {
            return -1;
        }
    }

    entry = ht_entry_new(key, key_len, value, ht->data_size);
    if (entry == NULL) {
        return -1;
    }

    bucket->entries[len] = entry;
    bucket->len++;
    ht->len++;
    return 0;
}

void* ht_get(Ht* ht, unsigned char* key, size_t key_len) {
    uint64_t hash = ht_hash(ht, key, key_len);
    HtBucket* bucket = &(ht->buckets[hash]);
    size_t i, len = bucket->len;
    if (len == 0) {
        return NULL;
    }
    for (i = 0; i < len; ++i) {
        HtEntry* entry = bucket->entries[i];
        size_t cur_key_len = entry->key_len;
        unsigned char* cur_key = entry->data;
        if ((cur_key_len == key_len) && (memcmp(key, cur_key, key_len) == 0)) {
            size_t offset = key_len + ht_padding(key_len);
            void* ptr = entry->data + offset;
            return ptr;
        }
    }
    return NULL;
}

int ht_delete(Ht* ht, unsigned char* key, size_t key_len, FreeFn* free_fn) {
    uint64_t hash = ht_hash(ht, key, key_len);
    HtBucket* bucket = &(ht->buckets[hash]);
    size_t i, len = bucket->len;
    if (len == 0) {
        return -1;
    }
    for (i = 0; i < len; ++i) {
        HtEntry* cur = bucket->entries[i];
        size_t cur_key_len = cur->key_len;
        unsigned char* cur_key = cur->data;
        if ((key_len == cur_key_len) && (memcmp(key, cur_key, key_len) == 0)) {
            bucket_remove(bucket, i, free_fn);
            ht->len--;
            return 0;
        }
    }
    return -1;
}

static inline void entry_free(HtEntry* entry, FreeFn* free_fn) {
    if (free_fn) {
        size_t e_key_len = entry->key_len;
        size_t padding = ht_padding(e_key_len);
        size_t offset = e_key_len + padding;
        void* ptr = entry->data + offset;
        free_fn(ptr);
    }
    free(entry);
}

static void ht_bucket_free(HtBucket* bucket, FreeFn* free_fn) {
    size_t i, len = bucket->len, cap = bucket->cap;
    if (cap == 0) {
        return;
    }
    for (i = 0; i < len; ++i) {
        HtEntry* entry = bucket->entries[i];
        entry_free(entry, free_fn);
    }
    free(bucket->entries);
}

void ht_free(Ht* ht, FreeFn* free_fn) {
    size_t i, len = ht->cap;
    for (i = 0; i < len; ++i) {
        HtBucket bucket = ht->buckets[i];
        ht_bucket_free(&bucket, free_fn);
    }
    free(ht->buckets);
    free(ht);
}
