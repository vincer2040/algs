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
        ck_assert_int_lt(arr[i], arr[i + 1]);
    }
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("algs_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, binary_search_test);
    tcase_add_test(tc_core, bubble_sort_test);
    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    int number_failed;
    Suite* s;
    SRunner* sr;
    s = ht_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
