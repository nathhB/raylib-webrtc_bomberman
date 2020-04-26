#include <raylib.h>
#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../player.h"
#include "../../../screen.h"
#include "../../../network.h"
#include "../../../game_state_machine.h"
#include "../../../game_states.h"
#include "../../../events.h"

static void render_ui_system(system_t*, world_t*, float);
static void handle_update_ui_event(world_t*, void*);
static void render_header(tdd_timer_t*, Font*);
static void render_header_scores(unsigned int, unsigned int*, Font*);
static void render_game_timer(tdd_timer_t*, Font*);
static void render_footer(Font*);
static void render_bombs_count(Vector2);
static void render_explosion_range(Vector2, Font*);

static unsigned int max_bombs_count = 0;
static unsigned int remaining_bombs_count = 0;
static unsigned int explosion_range = 0;

static Vector2 header_2players_scores_positions[] = {
    (Vector2){138, 9},
    (Vector2){565, 9}};

static Vector2 header_3players_scores_positions[] = {
    (Vector2){100, 9},
    (Vector2){212, 9},
    (Vector2){565, 9}};

static Vector2 header_4players_scores_positions[] = {
    (Vector2){100, 9},
    (Vector2){212, 9},
    (Vector2){494, 9},
    (Vector2){606, 9}};

static const char* header_players_atlas_regions[] = {
    "header_bomberman_white",
    "header_bomberman_black",
    "header_bomberman_purple",
    "header_bomberman_red"};

void world_register_render_ui_system(world_t* world)
{
    max_bombs_count = PLAYER_BASE_BOMBS_COUNT;
    remaining_bombs_count = PLAYER_BASE_BOMBS_COUNT;
    explosion_range = PLAYER_BASE_EXPLOSION_RANGE;

    world_register_system(world, render_ui_system);
}

static void render_ui_system(system_t* system, world_t* world, float delta)
{
    Camera2D ui_camera = { .target = VEC2_ZERO, .offset = VEC2_ZERO, .rotation = 0.f, .zoom = 1.f };
    Font* ui_font = GET_FONT("font");
    tdd_timer_t* timer = ((game_state_data_t*)world->user_data)->timer;

    world_consume_events(world, EVENT_UPDATE_UI, handle_update_ui_event);

    EndMode2D(); // end gameplay rendering

    BeginMode2D(ui_camera);

    render_header(timer, ui_font);
    render_footer(ui_font);

    EndMode2D();
}

static void handle_update_ui_event(world_t *world, void *ev_data)
{
    update_ui_event_t *ev = (update_ui_event_t *)ev_data;

    switch (ev->type)
    {
    case UI_MAX_BOMBS_COUNT:
        max_bombs_count = ev->max_bombs_count;

        TraceLog(LOG_INFO, "Update UI max bombs count: %d", max_bombs_count);
        break;

    case UI_REMAINING_BOMBS_COUNT:
        remaining_bombs_count = ev->remaining_bombs_count;

        TraceLog(LOG_INFO, "Update UI remaining bombs count: %d", remaining_bombs_count);
        break;

    case UI_EXPLOSION_RANGE:
        explosion_range = ev->explosion_range;

        TraceLog(LOG_INFO, "Update UI explosion range: %d", explosion_range);
        break;
    }
}

static void render_header(tdd_timer_t *timer, Font *font)
{
    atlas_region_t *header_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", "header");

    DrawTextureRec(*header_region->atlas_texture, header_region->rect, VEC2_ZERO, WHITE);

    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    render_header_scores(game_state_machine_data.game_settings.players_count, game_state_machine_data.scores, font);
    render_game_timer(timer, font);
}

static void render_header_scores(unsigned int players_count, unsigned int *scores, Font *font)
{
    Vector2 *scores_positions = NULL;

    if (players_count == 2)
    {
        scores_positions = header_2players_scores_positions;
    }
    else if (players_count == 3)
    {
        scores_positions = header_3players_scores_positions;
    }
    else if (players_count == 4)
    {
        scores_positions = header_4players_scores_positions;
    }

    if (scores_positions == NULL)
    {
        TraceLog(LOG_WARNING, "Cannot render players scores");
    }

    for (int i = 0; i < players_count; i++)
    {
        atlas_region_t *score_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", header_players_atlas_regions[i]);
        char score_text[2];

        snprintf(score_text, 2, "%d", scores[i]);

        Vector2 text_size = MeasureTextEx(*font, score_text, font->baseSize, 2);

        DrawTextureRec(*score_region->atlas_texture, score_region->rect, scores_positions[i], WHITE);
        DrawTextEx(*font, score_text, Vector2Add(scores_positions[i], (Vector2){73 - text_size.x / 2 - 2, 20 - text_size.y / 2 + 2}), font->baseSize, 2, WHITE);
    }
}

static void render_game_timer(tdd_timer_t *timer, Font *font)
{
    int seconds = game_state_machine_get_data().game_settings.round_time - timer->elapsed_ms / 1000;
    int remaining_seconds = MAX(0, seconds);
    char timer_text[10];

    snprintf(timer_text, 10, "%d : %02d", remaining_seconds / 60, remaining_seconds % 60);

    Vector2 text_size = MeasureTextEx(*font, timer_text, font->baseSize, 2);

    DrawTextEx(*font, timer_text, (Vector2){SCREEN_WIDTH / 2 - text_size.x / 2, 15}, font->baseSize, 2, WHITE);
}

static void render_footer(Font *font)
{
    Vector2 footer_position = (Vector2){0, GAME_AREA_HEIGHT + (UI_OFFSET / 2) - 2};

    DrawRectangle(footer_position.x, footer_position.y, SCREEN_WIDTH, UI_HEADER_SIZE, ORANGE);
    render_bombs_count(footer_position);
    // render_explosion_range(footer_position, font);

    // draw ping
    char ping_text[16];

    snprintf(ping_text, 16, "Ping: %lu", network_get_ping());

    DrawTextEx(*font, ping_text, Vector2Add(footer_position, (Vector2){SCREEN_WIDTH - 160, 10}), font->baseSize, 2, WHITE);
}

static void render_bombs_count(Vector2 footer_position)
{
    atlas_region_t *bomb_region = GET_TEXTURE_ATLAS_REGION("props_atlas", "bomb3");
    atlas_region_t *used_bomb_region = GET_TEXTURE_ATLAS_REGION("props_atlas", "bomb_transparent");

    for (int i = 0; i < max_bombs_count; i++)
    {
        Vector2 offset = {(i * 37) + 10, 10};
        atlas_region_t *region = (i < remaining_bombs_count) ? bomb_region : used_bomb_region;

        DrawTextureRec(*region->atlas_texture, region->rect, Vector2Add(footer_position, offset), WHITE);
    }
}

static void render_explosion_range(Vector2 footer_position, Font *font)
{
    char explosion_range_text[255] = {0};

    snprintf(explosion_range_text, 255, "Range: %d", explosion_range);

    Vector2 text_size = MeasureTextEx(*font, explosion_range_text, font->baseSize, 2);

    DrawTextEx(*font, explosion_range_text, Vector2Add(footer_position, (Vector2){SCREEN_WIDTH - text_size.x - 10, 10}), font->baseSize, 2, BLACK);
}