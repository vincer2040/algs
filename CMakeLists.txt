cmake_minimum_required(VERSION 3.10)

project(algs VERSION 0.1)

enable_testing()

add_subdirectory(tests)
set(INSTALL_PREFIX "/usr/local")

set(CMAKE_C_COMPILER "gcc")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2")

set(CMAKE_C_FLAGS_RELEASE "-std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -Werror=format-security -Werror=implicit-function-declaration -pipe -O2 -s -DNDEBUG")

add_library(algs src/binary_search.c src/bubble_sort.c src/queue.c src/stack.c src/quick_sort.c src/vec.c src/binary_tree.c src/bt_pre_order.c src/bt_in_order.c src/bt_post_order.c src/bt_bfs.c src/bt_compare.c src/minheap.c)

install(FILES build/libalgs.a DESTINATION "${INSTALL_PREFIX}/lib")
install(FILES src/algs.h DESTINATION "${INSTALL_PREFIX}/include")
