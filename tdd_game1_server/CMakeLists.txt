cmake_minimum_required (VERSION 3.0)
project (tdd_game1_server)

set(CMAKE_VERBOSE_MAKEFILE on)

set(LIBTDD_GAME1_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tdd_game1")
set(LIBTDD_NETWORK_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tdd_network")

set(EXPORTED_FUNCTIONS "[]")
set(EXTRA_EXPORTED_RUNTIME_METHODS "[\"ccall\", \"cwrap\"]")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -D TDD_SERVER -s EXIT_RUNTIME=1 -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=\"${EXTRA_EXPORTED_RUNTIME_METHODS}\" --profiling --shell-file ${LIBTDD_GAME1_DIRECTORY}/shell.html --js-library ${CMAKE_CURRENT_SOURCE_DIR}/src/javascript/server.js")

set(SYSTEMS_SOURCES
    src/systems/spawn_players_system.c
    src/systems/explode_players_system.c
    src/systems/broadcast_players_positions_system.c
)

set(SOURCES
    src/main.c
    src/network.c
    src/events.c
    src/map.c
    src/game_manager.c
    src/game_manager/game_over.c
    src/game_manager/game_waiting.c
    src/game_manager/next_round_waiting.c
    src/game_manager/round_playing.c
)

add_subdirectory(${LIBTDD_GAME1_DIRECTORY} tdd_game1_build)

add_executable(tdd_game1_server ${SOURCES} ${SYSTEMS_SOURCES})

target_link_libraries(tdd_game1_server tdd_game1)

target_include_directories(tdd_game1_server PUBLIC ${LIBTDD_GAME1_DIRECTORY})
target_include_directories(tdd_game1_server PUBLIC ${LIBTDD_NETWORK_DIRECTORY})

target_include_directories(tdd_game1 PUBLIC ${RAYLIB_RELEASE_PATH})
target_include_directories(tdd_game1 PUBLIC "${RAYLIB_RELEASE_PATH}/external")
