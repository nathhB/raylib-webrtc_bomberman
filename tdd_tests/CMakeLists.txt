cmake_minimum_required (VERSION 3.0)
project (tdd_tests)

set(CMAKE_VERBOSE_MAKEFILE on)

set(LIBTDD_DIRECTORY ../tdd)

add_subdirectory(${LIBTDD_DIRECTORY} tdd_build)

set(SOURCES CuTest.c all_tests.c bag_tests.c list_tests.c htable_tests.c)
set(HEADERS CuTest.h)

add_executable(tdd_tests ${SOURCES} ${HEADERS})

target_link_libraries(tdd_tests tdd)
target_include_directories(tdd PUBLIC ${LIBTDD_DIRECTORY})