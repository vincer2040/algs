#include "../src/algs.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_cmp(void* a, void* b) {
    int ai = *((int*)a);
    int bi = *((int*)b);

    return ai - bi;
}

BinaryNode* tree(void) {
    int a0 = 20, a1 = 50, a2 = 100, a3 = 30, a4 = 45, a5 = 29, a6 = 10, a7 = 15,
        a8 = 5, a9 = 7;
    BinaryNode* h = binary_node_new(&a0, sizeof(int));

    h->right = binary_node_new(&a1, sizeof(int));
    h->right->right = binary_node_new(&a2, sizeof(int));
    h->right->right->right = NULL;
    h->right->right->left = NULL;
    h->right->left = binary_node_new(&a3, sizeof(int));
    h->right->left->right = binary_node_new(&a4, sizeof(int));
    h->right->left->right->right = NULL;
    h->right->left->right->left = NULL;
    h->right->left->left = binary_node_new(&a5, sizeof(int));
    h->right->left->left->right = NULL;
    h->right->left->left->left = NULL;
    h->left = binary_node_new(&a6, sizeof(int));
    h->left->right = binary_node_new(&a7, sizeof(int));
    h->left->right->right = NULL;
    h->left->right->left = NULL;
    h->left->left = binary_node_new(&a8, sizeof(int));
    h->left->left->right = binary_node_new(&a9, sizeof(int));
    h->left->left->right->right = NULL;
    h->left->left->right->left = NULL;
    h->left->left->left = NULL;

    return h;
}

START_TEST(binary_search_test) {
    int foo[] = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
    size_t foo_len = sizeof foo / sizeof foo[0];
    size_t s = sizeof(int);

    int a0 = 69;
    int a1 = 1336;
    int a2 = 69420;
    int a3 = 69421;
    int a4 = 1;
    int a5 = 0;

    ck_assert_int_eq(binary_search(foo, &a0, foo_len, s, int_cmp), 3);
    ck_assert_int_eq(binary_search(foo, &a1, foo_len, s, int_cmp), -1);
    ck_assert_int_eq(binary_search(foo, &a2, foo_len, s, int_cmp), 10);
    ck_assert_int_eq(binary_search(foo, &a3, foo_len, s, int_cmp), -1);
    ck_assert_int_eq(binary_search(foo, &a4, foo_len, s, int_cmp), 0);
    ck_assert_int_eq(binary_search(foo, &a5, foo_len, s, int_cmp), -1);
}
END_TEST

START_TEST(bubble_sort_test) {
    int arr[] = {9, 3, 7, 4, 69, 420, 42};
    size_t i, arr_len = sizeof arr / sizeof arr[0];

    bubble_sort(arr, arr_len, sizeof(int), int_cmp);

    for (i = 0; i < arr_len - 1; ++i) {
        ck_assert_int_le(arr[i], arr[i + 1]);
    }
}
END_TEST

START_TEST(queue_test) {
    Queue q = queue_new(sizeof(int));
    int a1[] = {5, 7, 9};
    size_t i, a1_len = sizeof a1 / sizeof a1[0];
    int a, b;

    for (i = 0; i < a1_len; ++i) {
        queue_enque(&q, &a1[i]);
    }

    queue_deque(&q, &a);

    ck_assert_int_eq(a, 5);
    ck_assert_uint_eq(q.len, 2);

    a = 11;
    queue_enque(&q, &a);

    queue_deque(&q, &a);
    ck_assert_uint_eq(a, 7);

    queue_deque(&q, &a);
    ck_assert_uint_eq(a, 9);

    queue_peek(&q, &a);
    ck_assert_uint_eq(a, 11);

    queue_deque(&q, &a);
    ck_assert_uint_eq(a, 11);

    ck_assert_int_eq(queue_deque(&q, &a), -1);
    ck_assert_uint_eq(q.len, 0);

    // make sure it still works after emptying it
    a = 69;
    queue_enque(&q, &a);
    queue_peek(&q, &b);
    ck_assert_int_eq(b, 69);
    ck_assert_uint_eq(q.len, 1);

    queue_free(&q, NULL);
}
END_TEST

START_TEST(stack_test) {
    Stack s = stack_new(sizeof(int));
    int a1[] = {5, 7, 9};
    size_t i, a1_len = sizeof a1 / sizeof a1[0];
    int a, b;

    for (i = 0; i < a1_len; ++i) {
        stack_push(&s, &a1[i]);
    }

    stack_pop(&s, &a);
    ck_assert_int_eq(a, 9);
    ck_assert_uint_eq(s.len, 2);

    a = 11;
    stack_push(&s, &a);

    stack_pop(&s, &b);
    ck_assert_int_eq(b, 11);

    stack_pop(&s, &b);
    ck_assert_int_eq(b, 7);

    stack_peek(&s, &b);
    ck_assert_int_eq(b, 5);

    stack_pop(&s, &b);
    ck_assert_int_eq(b, 5);

    ck_assert_int_eq(stack_pop(&s, &b), -1);
    ck_assert_uint_eq(s.len, 0);

    a = 69;

    stack_push(&s, &a);

    stack_peek(&s, &b);

    ck_assert_int_eq(b, 69);
    ck_assert_uint_eq(s.len, 1);

    stack_free(&s, NULL);
}
END_TEST

START_TEST(qs_test) {
    int arr[] = {9, 3, 7, 4, 69, 420, 42, -1};
    size_t i, len = sizeof arr / sizeof arr[0];
    quick_sort(arr, len, sizeof arr[0], int_cmp);

    for (i = 0; i < len - 1; ++i) {
        ck_assert_int_le(arr[i], arr[i + 1]);
    }
}
END_TEST

START_TEST(bt_pre_order_test) {
    BinaryNode* h = tree();
    int exp[] = {
        20, 10, 5, 7, 15, 50, 30, 29, 45, 100,
    };
    vec* v;
    size_t i, len;

    v = pre_order_search(h, sizeof(int));
    len = v->len;
    for (i = 0; i < len; ++i) {
        int* at = ((int*)(v->data + (i * v->data_size)));
        ck_assert_int_eq(*at, exp[i]);
    }

    vec_free(v, NULL);
    binary_tree_free(h, NULL);
}
END_TEST

START_TEST(bt_in_order_test) {
    BinaryNode* h = tree();
    int exp[] = {
        5, 7, 10, 15, 20, 29, 30, 45, 50, 100,
    };
    vec* v;
    size_t i, len;

    v = in_order_search(h, sizeof(int));
    len = v->len;
    for (i = 0; i < len; ++i) {
        int* at = ((int*)(v->data + (i * v->data_size)));
        ck_assert_int_eq(*at, exp[i]);
    }

    vec_free(v, NULL);
    binary_tree_free(h, NULL);
}
END_TEST

START_TEST(bt_post_order_test) {
    BinaryNode* h = tree();
    int exp[] = {
        7, 5, 15, 10, 29, 45, 30, 100, 50, 20,
    };
    vec* v;
    size_t i, len;

    v = post_order_search(h, sizeof(int));
    len = v->len;
    for (i = 0; i < len; ++i) {
        int* at = ((int*)(v->data + (i * v->data_size)));
        ck_assert_int_eq(*at, exp[i]);
    }

    vec_free(v, NULL);
    binary_tree_free(h, NULL);
}
END_TEST

START_TEST(bt_bfs_test) {
    BinaryNode* head = tree();
    int a = 45, b = 7, c = 69;

    bool at = bt_bfs(head, &a, int_cmp);
    ck_assert(at == true);

    bool bt = bt_bfs(head, &b, int_cmp);
    ck_assert(bt == true);

    bool ct = bt_bfs(head, &c, int_cmp);
    ck_assert(ct == false);

    binary_tree_free(head, NULL);
}
END_TEST

Suite* suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("algs_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, binary_search_test);
    tcase_add_test(tc_core, bubble_sort_test);
    tcase_add_test(tc_core, queue_test);
    tcase_add_test(tc_core, stack_test);
    tcase_add_test(tc_core, qs_test);
    tcase_add_test(tc_core, bt_pre_order_test);
    tcase_add_test(tc_core, bt_in_order_test);
    tcase_add_test(tc_core, bt_post_order_test);
    tcase_add_test(tc_core, bt_bfs_test);
    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    int number_failed;
    Suite* s;
    SRunner* sr;
    s = suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
