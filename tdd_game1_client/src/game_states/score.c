#include <raylib.h>
#include <tdd_ecs.h>
#include <tdd_game1.h>

#include "../systems.h"
#include "../families.h"
#include "../assets.h"
#include "../game_state_machine.h"
#include "../game_states.h"
#include "../events.h"
#include "../screen.h"
#include "../vector_utils.h"

static world_t* create_scoreboard_world();
static void     render_player_score_row(int);

static Vector2 bomberman_heads_positions[] = {
    (Vector2){101, 115},
    (Vector2){101, 211},
    (Vector2){101, 307},
    (Vector2){101, 403}};

static Vector2 scoreboard_players_nicknames_positions[] = {
    (Vector2){70, SCREEN_HEIGHT - 50},
    (Vector2){262, SCREEN_HEIGHT - 50},
    (Vector2){453, SCREEN_HEIGHT - 50},
    (Vector2){641, SCREEN_HEIGHT - 50}};

static const char* bomberman_heads_regions[] = {
    "scoreboard_bomberman_head_white",
    "scoreboard_bomberman_head_black",
    "scoreboard_bomberman_head_purple",
    "scoreboard_bomberman_head_red"
};

static Vector2 cups_positions[][MAX_REQUIRED_ROUNDS] = {
    { (Vector2) { 255, 111 }, (Vector2) { 340, 111 }, (Vector2) { 425, 111 }, (Vector2) { 510, 111 }, (Vector2) { 595, 111 } },
    { (Vector2) { 255, 206 }, (Vector2) { 340, 206 }, (Vector2) { 425, 206 }, (Vector2) { 510, 206 }, (Vector2) { 595, 206 } },
    { (Vector2) { 255, 302 }, (Vector2) { 340, 302 }, (Vector2) { 425, 302 }, (Vector2) { 510, 302 }, (Vector2) { 595, 302 } },
    { (Vector2) { 255, 398 }, (Vector2) { 340, 398 }, (Vector2) { 425, 398 }, (Vector2) { 510, 398 }, (Vector2) { 595, 398 } },
};

static world_t*     scoreboard_world = NULL;
static tdd_timer_t* scoreboard_timer = NULL;

void on_score_state_entered(finite_state_machine_t* game_state_machine, state_t* score_state)
{
    TraceLog(LOG_INFO, "Entered score state");
    TraceLog(LOG_INFO, "Will re-enter game state in %d seconds...", NEXT_ROUND_DELAY_MS / 1000);

    scoreboard_timer = tdd_timer_create();
    scoreboard_world = create_scoreboard_world();

    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    for (int player_id = 0; player_id < game_state_machine_data.game_settings.players_count; player_id++)
    {
        for (int i = 0; i < game_state_machine_data.game_settings.required_rounds_to_win; i++)
        {
            unsigned int player_score = game_state_machine_data.scores[player_id];

            if (i < player_score)
            {
                create_scoreboard_cup_entity(
                    scoreboard_world,
                    cups_positions[player_id][i],
                    game_state_machine_data.last_round_winner_player_id == player_id && i == player_score - 1);
            }
        }
    }

    tdd_timer_start(scoreboard_timer);
}

void on_score_state_exited(finite_state_machine_t* game_state_machine, state_t* score_state)
{
    TraceLog(LOG_INFO, "Exited score state");

    world_destroy(scoreboard_world);
    tdd_timer_destroy(scoreboard_timer);
}

void on_score_state_updated(finite_state_machine_t* game_state_machine, state_t* score_state)
{
    atlas_region_t* background_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", "scoreboard_background");

    BeginDrawing();

    DrawTextureRec(*background_region->atlas_texture, background_region->rect, VEC2_ZERO, WHITE);

    unsigned int players_count = game_state_machine_get_data().game_settings.players_count;

    for (int player_id = 0; player_id < players_count; player_id++)
    {
        render_player_score_row(player_id);
    }

    tdd_timer_update(scoreboard_timer);
    world_update(scoreboard_world, 0);

    Font *ui_font = GET_FONT("font");
    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    // draw player nicknames
    int scoreboard_bomberman_region_width = GET_TEXTURE_ATLAS_REGION("ui_atlas", "scoreboard_bomberman_happy_white1")->rect.width;

    for (int i = 0; i <= game_state_machine_data.game_settings.players_count; i++)
    {
        int nickname_text_width = (int)MeasureTextEx(*ui_font, game_state_machine_data.nicknames[i], ui_font->baseSize, 2).x;
        Vector2 nickname_offset = (Vector2){(scoreboard_bomberman_region_width / 2) - nickname_text_width / 2, 0};

        DrawTextEx(*ui_font, game_state_machine_data.nicknames[i], Vector2Add(scoreboard_players_nicknames_positions[i], nickname_offset), ui_font->baseSize, 2, WHITE);
    }

    EndDrawing();
}

void score_state_handle_event(finite_state_machine_t* game_state_machine, state_t* game_state, int event_type, void* event_data)
{
    if (event_type == EVENT_ROUND_START)
    {
        TraceLog(LOG_INFO, "game_state_machine: score state received EVENT_ROUND_START event");

        ((game_state_machine_data_t*)game_state_machine->data)->is_round_playing = true;
    }

    free(event_data);
}

static world_t* create_scoreboard_world()
{
    world_t* world = world_create();
    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    for (int player_id = 0; player_id < game_state_machine_data.game_settings.players_count; player_id++)
    {
        create_scoreboard_player_entity(world, player_id, game_state_machine_data.last_round_winner_player_id == player_id);
    }

    world_register_entity_system(world, get_transform_family(), update_transforms_entity_system);
    world_register_animate_entity_system(world, scoreboard_timer);
    world_register_blink_rendering_entity_system(world, scoreboard_timer);
    world_register_entity_system(world, get_render_family(), render_entity_system);

    return world;
}

static void render_player_score_row(int player_id)
{
    atlas_region_t* bomberman_head_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", bomberman_heads_regions[player_id]);
    atlas_region_t* empty_cup_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", "cup_empty");

    DrawTextureRec(*bomberman_head_region->atlas_texture, bomberman_head_region->rect, bomberman_heads_positions[player_id], WHITE);

    for (int i = 0; i < game_state_machine_get_data().game_settings.required_rounds_to_win; i++)
    {
        DrawTextureRec(*empty_cup_region->atlas_texture, empty_cup_region->rect, cups_positions[player_id][i], WHITE);
    }
}