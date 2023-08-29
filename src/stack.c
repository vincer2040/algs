#include "algs.h"
#include <memory.h>
#include <stdlib.h>

Stack stack_new(size_t data_size) {
    Stack s = {0};
    s.data_size = data_size;
    s.head = NULL;

    return s;
}

int stack_push(Stack* s, void* data) {
    Node* n;
    Node* h;
    size_t size = sizeof(Node) + s->data_size;

    n = malloc(size);
    if (n == NULL) {
        return -1;
    }

    memset(n, 0, size);

    memcpy(n->data, data, s->data_size);

    if (s->len == 0) {
        s->head = n;
        s->len++;
        return 0;
    }

    h = s->head;
    n->next = h;
    s->head = n;

    s->len++;
    return 0;
}

int stack_pop(Stack* s, void* out) {
    Node* head;
    if (s->len == 0) {
        return -1;
    }

    head = s->head;
    memcpy(out, head->data, s->data_size);
    s->head = head->next;

    free(head);
    s->len--;
    return 0;
}

int stack_peek(Stack* s, void* out) {
    if (s->len == 0) {
        return -1;
    }

    memcpy(out, s->head->data, s->data_size);

    return 0;
}

void stack_free(Stack* s, FreeFn* fn) {
    Node* cur = s->head;

    while (cur != NULL) {
        Node* at = cur;
        cur = at->next;
        if (fn) {
            fn(at->data);
        }
        free(at);
    }
}
