#include <raylib.h>
#include <tdd.h>

#include "../game_state_machine.h"
#include "../game_states.h"
#include "../events.h"
#include "../player.h"
#include "../families.h"
#include "../map.h"
#include "../screen.h"
#include "../systems.h"
#include "../vector_utils.h"
#include "game/systems.h"

#define BACKGROUND_MAP_COLOR (Color){ 16, 88, 48, 255 }

static Camera2D camera = {
    .target = VEC2_ZERO,
    .offset = (Vector2) { 0, UI_HEADER_SIZE },
    .rotation = 0.0f,
    .zoom = (float)GAME_AREA_WIDTH / (TILE_SIZE * MAP_WIDTH)
};

static game_state_data_t    game_state_data = { 0 };
static world_t*             game_world = NULL;
static tdd_timer_t*         game_timer = NULL;

static world_t*             create_game_world();
static void                 create_players_entities(int, int);
static void                 update_scores(game_state_machine_data_t*, unsigned int*);

void on_game_state_entered(finite_state_machine_t* game_state_machine, state_t* state)
{
    TraceLog(LOG_INFO, "Entered game state");

    game_state_machine_data_t* game_state_machine_data = (game_state_machine_data_t*)game_state_machine->data;

    game_timer = tdd_timer_create();

    game_state_data.local_player_id = game_state_machine_data->local_player_id;
    game_state_data.timer = game_timer;

    game_world = create_game_world();

    map_save();
    create_players_entities(game_state_machine_data->game_settings.players_count, game_state_machine_data->local_player_id);
    tdd_timer_start(game_timer);
}

void on_game_state_exited(finite_state_machine_t* game_state_machine, state_t* state)
{
    TraceLog(LOG_INFO, "Exited game state");

    world_destroy(game_world);

    // destroy_tdd_game1_families();
    // destroy_tdd_game1_client_families();

    map_reset();
    tdd_timer_destroy(game_timer);
}

void on_game_state_updated(finite_state_machine_t* game_state_machine, state_t* state)
{
    BeginMode2D(camera);

    ClearBackground(BACKGROUND_MAP_COLOR);

    tdd_timer_update(game_timer);
    world_update(game_world, 0);

    EndMode2D();
}

void game_state_handle_event(finite_state_machine_t* game_state_machine, state_t* game_state, int event_type, void* event_data)
{
    if (event_type == EVENT_ROUND_OVER)
    {
        game_state_machine_data_t* game_state_machine_data = (game_state_machine_data_t*)game_state_machine->data;
        round_over_event_t* ev = (round_over_event_t*)event_data;

        update_scores(game_state_machine_data, ev->scores);

        game_state_machine_data->is_round_playing = false;
        game_state_machine_data->last_round_winner_player_id = ev->winner_player_id;

        free(event_data);
    }
    else
    {
        world_push_event(game_world, event_type, event_data);
    }
}

static world_t* create_game_world()
{
    world_t* world = world_create();

    map_create_physics(world);
    map_create_destructible_walls_entities(world);

    world_register_system(world, inputs_system);
    world_register_entity_system(world, get_inputs_controlled_family(), process_inputs_entity_system);
    world_register_entity_system(world, get_movement_family(), movement_entity_system);
    world_register_entity_system(world, get_transform_family(), update_transforms_entity_system);
    world_register_collision_entity_system(world);
    world_register_system(world, update_bombs_colliders_system);
    world_register_entity_system(world, get_state_machine_family(), update_state_machines_entity_system);
    world_register_system(world, spawn_bombs_system);
    world_register_system(world, spawn_bonuses_system);
    world_register_system(world, apply_bonuses_system);
    world_register_system(world, player_kick_bomb_system);
    world_register_update_bonuses_effects_entity_system(world);
    world_register_entity_system(world, get_lifetime_family(), lifetime_entity_system);
    world_register_system(world, update_player_position_system);
    world_register_entity_system(world, get_position_interpolated_family(), position_interpolation_entity_system);
    // world_register_system(world, update_players_map_positions_system);
    world_register_system(world, explode_players_system);
    world_register_system(world, send_local_player_position_system);
    world_register_system(world, render_map_system);
    world_register_animate_entity_system(world, game_timer);
    world_register_entity_system(world, get_render_family(), render_entity_system);
    world_register_blink_rendering_entity_system(world, game_timer);

    system_t* debug_render_colliders_system = (system_t*)world_register_entity_system(
        world, get_physics_family(), debug_render_colliders_entity_system);
    debug_render_colliders_system->is_enabled = false;

    world_register_render_ui_system(world);

    world->user_data = &game_state_data;

    return world;
}

static void create_players_entities(int players_count, int local_player_id)
{
    for (int player_id = 0; player_id < players_count; player_id++)
    {
        create_player_entity(game_world, player_id, player_id == local_player_id);
    }
}

static void update_scores(game_state_machine_data_t* game_state_machine_data, unsigned int* scores)
{
    TraceLog(LOG_INFO, "Update players scores:");

    for (int player_id = 0; player_id < game_state_machine_data->game_settings.players_count; player_id++)
    {
        TraceLog(LOG_INFO, "Player %d: %d", player_id, scores[player_id]);

        game_state_machine_data->scores[player_id] = scores[player_id];
    }
}