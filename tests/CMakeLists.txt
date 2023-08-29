add_executable(algs_test algs_test.c)

target_link_libraries(algs_test PUBLIC algs check pthread)

target_include_directories(algs_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME algs_test COMMAND algs_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(algs_test PROPERTIES TIMEOUT 30)
