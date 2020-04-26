cmake_minimum_required (VERSION 3.0)
project (libtdd_network)

enable_testing()

set(LIBTDD_DIRECTORY ../tdd/src)
set(SRC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_subdirectory(${LIBTDD_DIRECTORY} tdd_build)

add_subdirectory(${SRC_DIRECTORY})
add_subdirectory(${TESTS_DIRECTORY})

target_include_directories(tdd PUBLIC ${LIBTDD_DIRECTORY})