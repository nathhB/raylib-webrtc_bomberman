#include <string.h>

#include "bomb.h"
#include "components.h"
#include "network.h"
#include "game_states.h"
#include "physics.h"
#include "player.h"
#include "events.h"
#include "explosion.h"

static void on_bomb_hit_by_explosion(world_t*, entity_t*, entity_t*, collider_t*);
static void on_bomb_trigger_enter(world_t*, collision_t*);
static void on_bomb_trigger_exit(world_t*, collision_t*);
static void on_bomb_collision_enter(world_t*, collision_t*);

void create_bomb_entity(world_t* world, Vector2 bomb_position, bool from_local_player, unsigned int explosion_range)
{
    entity_t* bomb_entity = world_create_entity(world);
    transform_t* transform = add_transform_to_entity(bomb_entity, (int)bomb_position.x * TILE_SIZE, (int)bomb_position.y * TILE_SIZE);
    physics_t* physics = add_physics_to_entity(bomb_entity, true);
    // collider_t* collider = collider_create(TILE_SIZE, TILE_SIZE, from_local_player, COLLIDER_BOMB);
    collider_t* collider = collider_create(TILE_SIZE, TILE_SIZE, true, COLLIDER_BOMB);

    // TODO: create a second collide that always collides with other bombs

    physics_add_collider(physics, transform->root, collider);

    collider->on_trigger_enter_func = on_bomb_trigger_enter;
    collider->on_trigger_exit_func = on_bomb_trigger_exit;
    collider->on_collision_enter_func = on_bomb_collision_enter;

    render_t* render = add_render_to_entity(bomb_entity);
    sprite_data_t* sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "bomb1"));

    render_add_renderable(render, transform->root, (renderable_t*)sprite);

    animated_t *animated = add_animated_to_entity(bomb_entity);
    animated_sprite_t* animated_sprite = animated_create_animated_sprite(animated, sprite);

    animated_add_animation_to_sprite(animated_sprite, "bomb", animation_create("props_atlas", 4, true, 4, "bomb1", "bomb2", "bomb3", "bomb4"));
    animated_set_sprite_current_animation(animated_sprite, "bomb");

    add_bomb_to_entity(bomb_entity, explosion_range);
    add_destructible_to_entity(bomb_entity, on_bomb_hit_by_explosion);
    add_lifetime_to_entity(bomb_entity, BOMB_LIFETIME, bomb_explode)->create_time = ((game_state_data_t*)world->user_data)->timer->elapsed_ms;

    entity_group_manager_add_entity(world->entity_group_manager, bomb_entity, DESTRUCTIBLE_ENTITIES_GROUP);

    if (from_local_player)
    {
        entity_group_manager_add_entity(world->entity_group_manager, bomb_entity, LOCAL_PLAYER_BOMBS_GROUP);
    }

    entity_group_manager_add_entity(world->entity_group_manager, bomb_entity, BOMBS_GROUP);
}

void bomb_explode(world_t* world, entity_t* bomb_entity)
{
    transform_t* transform = get_transform_of_entity(bomb_entity);
    Vector2 bomb_map_position = (Vector2){ (int)transform->root->position.x / TILE_SIZE, (int)transform->root->position.y / TILE_SIZE };
    Vector2 bomb_position = Vector2Scale(bomb_map_position, TILE_SIZE);

    transform->root->position = bomb_position;

    create_explosion_entity(world, bomb_position, get_bomb_of_entity(bomb_entity)->explosion_range);
    map_set_cell((int)bomb_map_position.x, (int)bomb_map_position.y, MAP_GROUND);
    entity_group_manager_remove_entity(world->entity_group_manager, bomb_entity, DESTRUCTIBLE_ENTITIES_GROUP);
    entity_group_manager_remove_entity(world->entity_group_manager, bomb_entity, BOMBS_GROUP);

    if (entity_group_manager_is_entity_in_group(world->entity_group_manager, bomb_entity, LOCAL_PLAYER_BOMBS_GROUP))
    {
        entity_group_manager_remove_entity(world->entity_group_manager, bomb_entity, LOCAL_PLAYER_BOMBS_GROUP);

        // update ui
        unsigned int bombs_count = get_player_gear_of_entity(entity_tag_manager_get_entity(world->entity_tag_manager, LOCAL_PLAYER_TAG))->bombs_count;
        unsigned int remaining_bombs_count = bombs_count - entity_group_manager_get_group(world->entity_group_manager, LOCAL_PLAYER_BOMBS_GROUP)->count;

        create_ui_update_event(UI_REMAINING_BOMBS_COUNT, remaining_bombs_count);
    }
}

static void on_bomb_hit_by_explosion(world_t* world, entity_t* bomb_entity, entity_t* explosion_entity, collider_t* blast_collider)
{
    if (!bomb_entity->marked_for_deletion)
    {
        Vector2 bomb_position = get_transform_of_entity(bomb_entity)->root->position;
        Vector2 explosion_center = blast_collider->transform_node->parent->position;
        unsigned int explosion_range = get_explosion_of_entity(explosion_entity)->range;
        bool explode = false;

        if (strcmp(blast_collider->transform_node->name, "h_blast_collider") == 0)
        {
            explode = ((int)blast_collider->transform_node->position.y == (int)bomb_position.y) &&
                       ABS((int)bomb_position.x / TILE_SIZE - (int)explosion_center.x / TILE_SIZE) <= explosion_range;
        }
        else if (strcmp(blast_collider->transform_node->name, "v_blast_collider") == 0)
        {
            explode = (int)blast_collider->transform_node->position.x == (int)bomb_position.x &&
                       ABS((int)bomb_position.y / TILE_SIZE - (int)explosion_center.y / TILE_SIZE) <= explosion_range;
        }

        if (explode)
        {
            bomb_explode(world, bomb_entity);

            world_mark_entity_for_deletion(world, bomb_entity);
        }
    }
}

static void on_bomb_trigger_enter(world_t* world, collision_t* collision)
{
    if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, collision->entity_a, LOCAL_PLAYER_TAG))
    {
        get_bomb_of_entity(collision->entity_b)->has_player_inside = true;
    }
}

static void on_bomb_trigger_exit(world_t* world, collision_t* collision)
{
    if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, collision->entity_a, LOCAL_PLAYER_TAG))
    {
        get_bomb_of_entity(collision->entity_b)->has_player_inside = false;
    }
}

static void on_bomb_collision_enter(world_t* world, collision_t* collision)
{
    entity_t* player_entity = collision->entity_a;
    entity_t *bomb_entity = collision->entity_b;
    bomb_t* bomb = get_bomb_of_entity(bomb_entity);

    if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, player_entity, LOCAL_PLAYER_TAG) &&
        get_player_gear_of_entity(player_entity)->can_kick &&
        !bomb->is_kicked)
    {
        Vector2 bomb_position = get_transform_of_entity(bomb_entity)->root->position;
        Vector2 kick_direction = Vector2Normalize(collision->velocity);

        create_player_kick_bomb_event((Vector2) { (int)bomb_position.x / TILE_SIZE, (int)bomb_position.y / TILE_SIZE }, kick_direction);

        bomb->is_kicked = true;

        player_kick_bomb_net_message_t* kick_bomb_net_message = player_kick_bomb_net_message_create();

        kick_bomb_net_message->player_id = ((game_state_data_t*)world->user_data)->local_player_id;
        kick_bomb_net_message->bomb_x = (int)bomb_position.x / TILE_SIZE;
        kick_bomb_net_message->bomb_y = (int)bomb_position.y / TILE_SIZE;
        kick_bomb_net_message->dir_x = (int)kick_direction.x;
        kick_bomb_net_message->dir_y = (int)kick_direction.y;

        SEND_RELIABLE(kick_bomb_net_message);
    }
}

entity_t* find_bomb_entity_at(world_t* world, Vector2 position)
{
    list_t* bombs = entity_group_manager_get_group(world->entity_group_manager, BOMBS_GROUP);
    list_node_t* current_bomb_node = bombs->head;

    while (current_bomb_node)
    {
        entity_t* bomb_entity = (entity_t*)current_bomb_node->data;
        Vector2 bomb_position = get_transform_of_entity(bomb_entity)->root->position;

        if ((int)position.x == (int)bomb_position.x / TILE_SIZE && (int)position.y == (int)bomb_position.y / TILE_SIZE)
        {
            return bomb_entity;
        }

        current_bomb_node = current_bomb_node->next;
    }

    return NULL;
}