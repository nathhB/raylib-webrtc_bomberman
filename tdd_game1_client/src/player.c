#include <string.h>
#include <tdd_game1.h>

#include "player.h"
#include "components.h"
#include "bonus.h"
#include "network.h"
#include "game_states.h"
#include "bomb.h"
#include "events.h"
#include "physics.h"

static const char* player_tags[] = { "PLAYER_1", "PLAYER_2", "PLAYER_3", "PLAYER_4" };

static char* players_move_down_sprites[MAX_PLAYERS][3] = {
    { "move_down1-white", "move_down2-white", "move_down3-white" },
    { "move_down1-black", "move_down2-black", "move_down3-black" },
    { "move_down1-purple", "move_down2-purple", "move_down3-purple" },
    { "move_down1-red", "move_down2-red", "move_down3-red" }
};

static char* players_move_up_sprites[MAX_PLAYERS][3] = {
    { "move_up1-white", "move_up2-white", "move_up3-white" },
    { "move_up1-black", "move_up2-black", "move_up3-black" },
    { "move_up1-purple", "move_up2-purple", "move_up3-purple" },
    { "move_up1-red", "move_up2-red", "move_up3-red" }
};

static char* players_move_right_sprites[MAX_PLAYERS][3] = {
    { "move_right1-white", "move_right2-white", "move_right3-white" },
    { "move_right1-black", "move_right2-black", "move_right3-black" },
    { "move_right1-purple", "move_right2-purple", "move_right3-purple" },
    { "move_right1-red", "move_right2-red", "move_right3-red" }
};

static char* players_move_left_sprites[MAX_PLAYERS][3] = {
    { "move_left1-white", "move_left2-white", "move_left3-white" },
    { "move_left1-black", "move_left2-black", "move_left3-black" },
    { "move_left1-purple", "move_left2-purple", "move_left3-purple" },
    { "move_left1-red", "move_left2-red", "move_left3-red" }
};

static char* players_dead_sprites[MAX_PLAYERS][6] = {
    { "dead1-white", "dead2-white", "dead3-white", "dead4-white", "dead5-white", "dead6-white" },
    { "dead1-black", "dead2-black", "dead3-black", "dead4-black", "dead5-black", "dead6-black" },
    { "dead1-purple", "dead2-purple", "dead3-purple", "dead4-purple", "dead5-purple", "dead6-purple" },
    { "dead1-red", "dead2-red", "dead3-red", "dead4-red", "dead5-red", "dead6-red" }
};

static Vector2 players_spawns_positions[MAX_PLAYERS] = {
    (Vector2) { 1, 1 },
    (Vector2) { 13, 1 },
    (Vector2) { 1, 9 },
    (Vector2) { 13, 9 }
};

static void     setup_player_animations(entity_t*, sprite_data_t*, int);
static void     on_player_hit_by_explosion(world_t*, entity_t*, entity_t*, collider_t*);
static bool     is_player_dead(entity_t*);
static void     on_player_trigger_enter(world_t*, collision_t*);
static void     handle_bonus_pickup(world_t*, entity_t*, entity_t*);

void create_player_entity(world_t* world, int player_id, bool is_local)
{
    Vector2 map_position = players_spawns_positions[player_id];

    printf("Player entity created at (%d, %d), player_id: %d, is_local: %d !\n", (int)map_position.x, (int)map_position.y, player_id, is_local);

    entity_t* player_entity = world_create_entity(world);

    transform_t* transform = add_transform_to_entity(player_entity, (int)map_position.x * TILE_SIZE, (int)map_position.y * TILE_SIZE);
    add_movement_to_entity(player_entity);
    
    render_t* render = add_render_to_entity(player_entity);
    sprite_data_t* sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("bomberman_atlas", players_move_down_sprites[player_id][0]));

    render_add_renderable(render, transform->root, (renderable_t*)sprite);

    add_player_gear_to_entity(player_entity);

    setup_player_animations(player_entity, sprite, player_id);
    create_player_state_machine(world, player_entity);

    physics_t* physics = add_physics_to_entity(player_entity, false);
    collider_t* player_collider = collider_create(TILE_SIZE - 5, TILE_SIZE - 6, false, COLLIDER_PLAYER);
    transform_node_t* collider_transform = transform_node_attach_child(transform->root, NULL, PLAYER_TRANSFORM_OFFSET);

    physics_add_collider(physics, collider_transform, player_collider);

    player_collider->on_trigger_enter_func = on_player_trigger_enter;

    if (is_local)
    {
        add_inputs_controlled_to_entity(player_entity, 0);
        add_destructible_to_entity(player_entity, on_player_hit_by_explosion);

        entity_tag_manager_tag_entity(world->entity_tag_manager, player_entity, LOCAL_PLAYER_TAG);
        entity_group_manager_add_entity(world->entity_group_manager, player_entity, DESTRUCTIBLE_ENTITIES_GROUP);
    }
    else
    {
        add_remotely_controlled_to_entity(player_entity, player_id);
        add_interpolated_position_to_entity(player_entity, 100);

        player_collider->collision_mask = COLLIDER_BONUS;
    }

    entity_tag_manager_tag_entity(world->entity_tag_manager, player_entity, get_player_entity_tag(player_id));
    entity_group_manager_add_entity(world->entity_group_manager, player_entity, PLAYERS_GROUP);
}

static void setup_player_animations(entity_t* player_entity, sprite_data_t* sprite, int player_id)
{
    animated_t* animated = add_animated_to_entity(player_entity);
    animated_sprite_t* animated_sprite = animated_create_animated_sprite(animated, sprite);

    char** move_down_sprites = players_move_down_sprites[player_id];
    char** move_up_sprites = players_move_up_sprites[player_id];
    char** move_right_sprites = players_move_right_sprites[player_id];
    char** move_left_sprites = players_move_left_sprites[player_id];
    char** dead_sprites = players_dead_sprites[player_id];

    animated_add_animation_to_sprite(
        animated_sprite, "move_down", animation_create("bomberman_atlas", 6, true, 4, move_down_sprites[0], move_down_sprites[1], move_down_sprites[0], move_down_sprites[2]));
    animated_add_animation_to_sprite(
        animated_sprite, "move_up", animation_create("bomberman_atlas", 6, true, 4, move_up_sprites[0], move_up_sprites[1], move_up_sprites[0], move_up_sprites[2]));
    animated_add_animation_to_sprite(
        animated_sprite, "move_right", animation_create("bomberman_atlas", 6, true, 4, move_right_sprites[0], move_right_sprites[1], move_right_sprites[0], move_right_sprites[2]));
    animated_add_animation_to_sprite(
        animated_sprite, "move_left", animation_create("bomberman_atlas", 6, true, 4, move_left_sprites[0], move_left_sprites[1], move_left_sprites[0], move_left_sprites[2]));
    animated_add_animation_to_sprite(animated_sprite, "idle_down", animation_create("bomberman_atlas", 1, true, 1, move_down_sprites[0]));
    animated_add_animation_to_sprite(animated_sprite, "idle_up", animation_create("bomberman_atlas", 1, true, 1, move_up_sprites[0]));
    animated_add_animation_to_sprite(animated_sprite, "idle_right", animation_create("bomberman_atlas", 1, true, 1, move_right_sprites[0]));
    animated_add_animation_to_sprite(animated_sprite, "idle_left", animation_create("bomberman_atlas", 1, true, 1, move_left_sprites[0]));
    animated_add_animation_to_sprite(animated_sprite, "dead", animation_create("bomberman_atlas", 6, false, 6, dead_sprites[0], dead_sprites[1], dead_sprites[2], dead_sprites[3], dead_sprites[4], dead_sprites[5]));

    animated_set_sprite_current_animation(animated_sprite, "idle_down");
}

const char* get_player_entity_tag(int player_id)
{
    return player_tags[player_id];
}

bool player_is_dead(world_t* world, int player_id)
{
    return is_player_dead(entity_tag_manager_get_entity(world->entity_tag_manager, get_player_entity_tag(player_id)));
}

static void on_player_hit_by_explosion(world_t* world, entity_t* player_entity, entity_t* explosion_entity, collider_t* blast_collider)
{
    if (get_player_gear_of_entity(player_entity)->is_indestructible || is_player_dead(player_entity)) return;

    int player_id = ((game_state_data_t*)world->user_data)->local_player_id;

    create_player_exploded_event(player_id);
}

static bool is_player_dead(entity_t* player_entity)
{
    finite_state_machine_t* player_state_machine = get_state_machine_of_entity(player_entity)->finite_state_machine;

    return strcmp(player_state_machine->current_state->name, "dead") == 0;
}

static void on_player_trigger_enter(world_t* world, collision_t* collision)
{
    if (entity_group_manager_is_entity_in_group(world->entity_group_manager, collision->entity_b, BONUS_ENTITIES_GROUP))
    {
        handle_bonus_pickup(world, collision->entity_a, collision->entity_b);
    }
}

static void handle_bonus_pickup(world_t* world, entity_t* player_entity, entity_t* bonus_entity)
{
    if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, player_entity, LOCAL_PLAYER_TAG))
    {
        player_pickup_bonus_net_message_t* net_message = player_pickup_bonus_net_message_create();
        Vector2 bonus_position = get_transform_of_entity(bonus_entity)->root->position;

        net_message->player_id = ((game_state_data_t*)world->user_data)->local_player_id;
        net_message->x = (int)bonus_position.x / TILE_SIZE;
        net_message->y = (int)bonus_position.y / TILE_SIZE;

        SEND_RELIABLE(net_message)
    }

    world_mark_entity_for_deletion(world, bonus_entity);
    entity_group_manager_remove_entity(world->entity_group_manager, bonus_entity, BONUS_ENTITIES_GROUP);
}