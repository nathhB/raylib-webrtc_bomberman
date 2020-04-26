cmake_minimum_required (VERSION 3.0)
project (tdd_game1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(RAYLIB_PATH C:/raylib/raylib)
set(RAYLIB_RELEASE_PATH "${RAYLIB_PATH}/src")

set(LIBTDD_DIRECTORY ../tdd/src)
set(LIBTDDECS_DIRECTORY ../tdd_ecs/src)
set(LIBTDD_NETWORK_DIRECTORY ../tdd_network/src)

set(SOURCES src/network.c src/map.c src/network/net_messages_factory.c src/network/net_messages.c tdd_game1.h)

add_subdirectory(${LIBTDD_DIRECTORY} tdd_build)
add_subdirectory(${LIBTDDECS_DIRECTORY} tdd_ecs_build)
add_subdirectory(${LIBTDD_NETWORK_DIRECTORY} tdd_network_build)

add_library(tdd_game1 ${SOURCES})

target_link_libraries(tdd_game1 tdd)
target_link_libraries(tdd_game1 tddecs)
target_link_libraries(tdd_game1 tdd_network)
target_link_libraries(tdd_game1 "${RAYLIB_RELEASE_PATH}/libraylib.a")

target_include_directories(tdd PUBLIC ${LIBTDD_DIRECTORY})
target_include_directories(tddecs PUBLIC ${LIBTDDECS_DIRECTORY})
target_include_directories(tdd_network PUBLIC ${LIBTDD_NETWORK_DIRECTORY})
target_include_directories(tdd_game1 PUBLIC "${RAYLIB_RELEASE_PATH}/external")