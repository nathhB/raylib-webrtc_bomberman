cmake_minimum_required (VERSION 3.0)
project (libtdd)

enable_testing()

set(SRC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_subdirectory(${SRC_DIRECTORY})
add_subdirectory(${TESTS_DIRECTORY})