cmake_minimum_required (VERSION 3.0)
project (tdd_ecs_tests)

set(LIBTDD_DIRECTORY ../tdd)
set(LIBTDD_ECS_DIRECTORY ../tdd_ecs)

add_subdirectory(${LIBTDD_ECS_DIRECTORY} tdd_ecs_build)

set(SOURCES CuTest.c all_tests.c entity_tests.c family_tests.c system_tests.c world_tests.c test_components.c)
set(HEADERS CuTest.h test_components.h)

add_executable(tdd_ecs_tests ${SOURCES} ${HEADERS})

target_link_libraries(tdd_ecs_tests tdd)
target_link_libraries(tdd_ecs_tests tddecs)

target_include_directories(tdd PUBLIC ${LIBTDD_DIRECTORY})
target_include_directories(tddecs PUBLIC ${LIBTDD_ECS_DIRECTORY})