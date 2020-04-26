#include <raylib.h>
#include <tdd_ecs.h>

#include "../../components.h"
#include "../../player.h"

static Vector2 lobby_players_positions[] = {
    (Vector2){70, 514},
    (Vector2){262, 514},
    (Vector2){453, 514},
    (Vector2){641, 514}};

static const char *lobby_players_region_names[][5] = {
    {"lobby_bomberman_white1", "lobby_bomberman_white2", "lobby_bomberman_white3", "lobby_bomberman_white4", "lobby_bomberman_white5"},
    {"lobby_bomberman_black1", "lobby_bomberman_black2", "lobby_bomberman_black3", "lobby_bomberman_black4", "lobby_bomberman_black5"},
    {"lobby_bomberman_purple1", "lobby_bomberman_purple2", "lobby_bomberman_purple3", "lobby_bomberman_purple4", "lobby_bomberman_purple5"},
    {"lobby_bomberman_red1", "lobby_bomberman_red2", "lobby_bomberman_red3", "lobby_bomberman_red4", "lobby_bomberman_red5"}};

void create_lobby_player_entity(world_t* world, int player_id, bool is_ready)
{
    TraceLog(LOG_INFO, "Create lobby player entity (player_id: %d, is_ready: %d)", player_id, is_ready);

    entity_t* lobby_player_entity = world_create_entity(world);
    Vector2 position = lobby_players_positions[player_id];

    transform_t* transform = add_transform_to_entity(lobby_player_entity, (int)position.x, (int)position.y);
    render_t* render = add_render_to_entity(lobby_player_entity);

    sprite_data_t* sprite = create_sprite_from_atlas(
        GET_TEXTURE_ATLAS_REGION("ui_atlas", (is_ready) ? lobby_players_region_names[player_id][4] : lobby_players_region_names[player_id][0]));

    render_add_renderable(render, transform->root, (renderable_t*)sprite);

    if (!is_ready)
    {
        animated_t* animated = add_animated_to_entity(lobby_player_entity);
        animated_sprite_t *animated_sprite = animated_create_animated_sprite(animated, sprite);

        const char** regions = lobby_players_region_names[player_id];

        animated_add_animation_to_sprite(animated_sprite, "idle", animation_create("ui_atlas", 1, false, 1, regions[0]));
        animated_add_animation_to_sprite(animated_sprite, "ready", animation_create("ui_atlas", 8, false, 5, regions[0], regions[1], regions[2], regions[3], regions[4]));

        animated_set_sprite_current_animation(animated_sprite, "idle");

        entity_tag_manager_tag_entity(world->entity_tag_manager, lobby_player_entity, get_player_entity_tag(player_id));
    }
}