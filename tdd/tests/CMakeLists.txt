cmake_minimum_required (VERSION 3.0)

set(LIBTDD_UTESTS_DIRECTORY ../../tdd_utests)

add_subdirectory(${LIBTDD_UTESTS_DIRECTORY} tdd_utests_build)

add_executable(test_bag bag_tests.c)
add_executable(test_list list_tests.c)
add_executable(test_htable htable_tests.c)
add_executable(test_circular_buffer circular_buffer_tests.c)

target_link_libraries(test_bag tdd tdd_utests)
target_link_libraries(test_list tdd tdd_utests)
target_link_libraries(test_htable tdd tdd_utests)
target_link_libraries(test_circular_buffer tdd tdd_utests)

target_include_directories(tdd PUBLIC ../src)
target_include_directories(tdd_utests PUBLIC ${LIBTDD_UTESTS_DIRECTORY})

add_test(bag test_bag)
add_test(list test_list)
add_test(htable test_htable)
add_test(circular_buffer test_circular_buffer)