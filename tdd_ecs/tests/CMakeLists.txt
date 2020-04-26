cmake_minimum_required (VERSION 3.0)

set(LIBTDD_UTESTS_DIRECTORY ../../tdd_utests)

add_subdirectory(${LIBTDD_UTESTS_DIRECTORY} tdd_utests_build)

add_executable(test_entity entity_tests.c test_components.c test_components.h)
add_executable(test_family family_tests.c test_components.c test_components.h)
add_executable(test_system system_tests.c test_components.c test_components.h)
add_executable(test_world_events world_events_tests.c)

target_link_libraries(test_entity tddecs tdd_utests)
target_link_libraries(test_family tddecs tdd_utests)
target_link_libraries(test_system tddecs tdd_utests)
target_link_libraries(test_world_events tddecs tdd_utests)

target_include_directories(tddecs PUBLIC ../src)
target_include_directories(tdd_utests PUBLIC ${LIBTDD_UTESTS_DIRECTORY})

add_test(entity test_entity)
add_test(family test_family)
add_test(system test_system)
add_test(world_events test_world_events)