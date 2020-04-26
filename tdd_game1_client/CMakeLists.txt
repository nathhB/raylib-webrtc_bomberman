cmake_minimum_required (VERSION 3.0)

project (tdd_game1_client)

set(CMAKE_VERBOSE_MAKEFILE on)

set(LIBTDD_GAME1_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tdd_game1")

set(RAYLIB_PATH C:/raylib/raylib)
set(RAYLIB_RELEASE_PATH "${RAYLIB_PATH}/src")

set(EXPORTED_FUNCTIONS "[]")
set(EXTRA_EXPORTED_RUNTIME_METHODS "[\"ccall\", \"cwrap\"]")

set(RESOURCES_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/@")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -D TDD_CLIENT -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s BINARYEN_TRAP_MODE='clamp' -s FORCE_FILESYSTEM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=\"${EXTRA_EXPORTED_RUNTIME_METHODS}\" --profiling --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html --js-library ${CMAKE_CURRENT_SOURCE_DIR}/src/javascript/client.js --preload-file ${RESOURCES_DIRECTORY}")
set(CMAKE_EXECUTABLE_SUFFIX .html)

set(ASSET_SOURCES
    src/assets/texture.c
    src/assets/texture_atlas.c
    src/assets/font.c
)

set(SOURCES
    src/transform.c
    src/physics.c
    src/main.c
    src/network.c
    src/events.c
    src/components.c
    src/families.c
    src/bomb.c
    src/explosion.c
    src/rendering.c
    src/player.c
    src/player/state_machine.c
    src/assets_manager.c
    src/map.c
    src/animation.c
    src/bonus.c
    src/vector_utils.c
    src/aabb.c
    src/game_state_machine.c
    src/game_states/connection.c
    src/game_states/game.c
    src/game_states/lobby.c
    src/game_states/lobby/lobby_player.c
    src/game_states/score.c
    src/game_states/score/scoreboard_player.c
    src/game_states/score/scoreboard_cup.c
)

set(SYSTEMS_SOURCES
    src/systems/animate_entity_system.c
    src/systems/render_entity_system.c
    src/systems/update_transforms_entity_system.c
    src/systems/blink_rendering_entity_system.c
)

set(GAME_SYSTEMS_SOURCES
    src/game_states/game/systems/inputs_system.c
    src/game_states/game/systems/process_inputs_entity_system.c
    src/game_states/game/systems/movement_entity_system.c
    src/game_states/game/systems/collision_entity_system.c
    src/game_states/game/systems/debug_render_colliders_entity_system.c
    src/game_states/game/systems/render_map_system.c
    src/game_states/game/systems/update_player_position_system.c
    src/game_states/game/systems/position_interpolation_entity_system.c
    src/game_states/game/systems/send_local_player_position_system.c
    src/game_states/game/systems/spawn_bombs_system.c
    src/game_states/game/systems/lifetime_entity_system.c
    src/game_states/game/systems/explode_players_system.c
    src/game_states/game/systems/update_state_machines_entity_system.c
    src/game_states/game/systems/spawn_bonuses_system.c
    src/game_states/game/systems/apply_bonuses_system.c
    src/game_states/game/systems/update_bonuses_effects_system.c
    src/game_states/game/systems/render_ui_system.c
    src/game_states/game/systems/player_kick_bomb_system.c
    src/game_states/game/systems/update_bombs_colliders_system.c
)

add_subdirectory(${LIBTDD_GAME1_DIRECTORY} tdd_game1_build)

add_executable(tdd_game1_client ${SOURCES} ${SYSTEMS_SOURCES} ${GAME_SYSTEMS_SOURCES} ${ASSET_SOURCES})

target_link_libraries(tdd_game1_client "${RAYLIB_RELEASE_PATH}/libraylib.bc")
target_link_libraries(tdd_game1_client tdd_game1)

target_include_directories(tdd_game1_client PUBLIC ${LIBTDD_GAME1_DIRECTORY})
target_include_directories(tdd_game1 PUBLIC ${RAYLIB_RELEASE_PATH})
target_include_directories(tdd_game1 PUBLIC "${RAYLIB_RELEASE_PATH}/external")