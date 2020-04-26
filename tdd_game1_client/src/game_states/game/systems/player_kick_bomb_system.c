#include <tdd_ecs.h>
#include <tdd_game1.h>

#include "../../../components.h"
#include "../../../physics.h"
#include "../../../bomb.h"
#include "../../../events.h"

static void handle_player_kick_bomb_event(world_t*, void*);

void player_kick_bomb_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_KICK_BOMB, handle_player_kick_bomb_event);
}

static void handle_player_kick_bomb_event(world_t* world, void* ev_data)
{
    player_kick_bomb_event_t* ev = (player_kick_bomb_event_t*)ev_data;

    entity_t* bomb_entity = find_bomb_entity_at(world, ev->bomb_position);
    movement_t* bomb_movement = add_movement_to_entity(bomb_entity);
    Vector2 bomb_position = get_transform_of_entity(bomb_entity)->root->position;
    physics_t* physics = get_physics_of_entity(bomb_entity);

    map_set_cell((int)bomb_position.x / TILE_SIZE, (int)bomb_position.y / TILE_SIZE, MAP_GROUND);

    physics->is_kinetic = false;
    ((collider_t*)list_get_at(physics->colliders, 0))->collision_mask = COLLIDER_WALL | COLLIDER_DESTRUCTIBLE_WALL | COLLIDER_EXPLOSION_BLAST;

    bomb_movement->x_velocity = ev->kick_direction.x * KICKED_BOMB_SPEED;
    bomb_movement->y_velocity = ev->kick_direction.y * KICKED_BOMB_SPEED;
}