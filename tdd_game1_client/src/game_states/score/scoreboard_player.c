#include <raylib.h>
#include <tdd_ecs.h>

#include "../../components.h"

static Vector2 scoreboard_players_positions[] = {
    (Vector2){70, 514},
    (Vector2){262, 514},
    (Vector2){453, 514},
    (Vector2){641, 514}};

static const char* scoreboard_players_happy_region_names[][5] = {
    {"scoreboard_bomberman_happy_white1", "scoreboard_bomberman_happy_white2"},
    {"scoreboard_bomberman_happy_black1", "scoreboard_bomberman_happy_black2"},
    {"scoreboard_bomberman_happy_purple1", "scoreboard_bomberman_happy_purple2"},
    {"scoreboard_bomberman_happy_red1", "scoreboard_bomberman_happy_red2"}};

static const char* scoreboard_players_sad_region_names[][5] = {
    {"scoreboard_bomberman_sad_white1", "scoreboard_bomberman_sad_white2"},
    {"scoreboard_bomberman_sad_black1", "scoreboard_bomberman_sad_black2"},
    {"scoreboard_bomberman_sad_purple1", "scoreboard_bomberman_sad_purple2"},
    {"scoreboard_bomberman_sad_red1", "scoreboard_bomberman_sad_red2"}};

void create_scoreboard_player_entity(world_t* world, int player_id, bool is_round_winner)
{
    TraceLog(LOG_INFO, "Create scoreboard player entity (player_id: %d, is_round_winner: %d)", player_id, is_round_winner);

    entity_t* scoreboard_player_entity = world_create_entity(world);
    Vector2 position = scoreboard_players_positions[player_id];

    transform_t* transform = add_transform_to_entity(scoreboard_player_entity, (int)position.x, (int)position.y);
    render_t* render = add_render_to_entity(scoreboard_player_entity);

    sprite_data_t* sprite = create_sprite_from_atlas(
        GET_TEXTURE_ATLAS_REGION("ui_atlas", (is_round_winner) ? scoreboard_players_happy_region_names[player_id][0] : scoreboard_players_sad_region_names[player_id][0]));

    render_add_renderable(render, transform->root, (renderable_t*)sprite);

    animated_t* animated = add_animated_to_entity(scoreboard_player_entity);
    animated_sprite_t* animated_sprite = animated_create_animated_sprite(animated, sprite);

    if (is_round_winner)
    {
        const char** regions = scoreboard_players_happy_region_names[player_id];

        animated_add_animation_to_sprite(animated_sprite, "dance", animation_create("ui_atlas", 2, true, 2, regions[0], regions[1]));

        animated_set_sprite_current_animation(animated_sprite, "dance");
    }
    else
    {
        const char **regions = scoreboard_players_sad_region_names[player_id];

        animated_add_animation_to_sprite(animated_sprite, "cry", animation_create("ui_atlas", 2, true, 2, regions[0], regions[1]));

        animated_set_sprite_current_animation(animated_sprite, "cry");
    }
}