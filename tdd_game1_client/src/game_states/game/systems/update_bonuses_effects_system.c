#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../game_states.h"
#include "../../../physics.h"
#include "../../../player.h"
#include "../../../families.h"

static void update_bonuses_effects_entity_system(entity_system_t*, world_t*, float, entity_t*);

static bool is_local_player_walking_through_walls = false;

void world_register_update_bonuses_effects_entity_system(world_t* world)
{
    is_local_player_walking_through_walls = false;

    world_register_entity_system(world, get_player_gear_family(), update_bonuses_effects_entity_system);
}

static void update_bonuses_effects_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    player_gear_t* player_gear = get_player_gear_of_entity(entity);

    if (player_gear->is_indestructible)
    {
        if (((game_state_data_t*)world->user_data)->timer->elapsed_ms - player_gear->armor_equip_time > PLAYER_ARMOR_TIME)
        {
            player_gear->is_indestructible = false;

            remove_blink_rendering_from_entity(entity);

            list_node_t* current_node = get_render_of_entity(entity)->renderables->head;

            while (current_node)
            {
                ((renderable_t *)current_node->data)->is_rendering_enabled = true;

                current_node = current_node->next;
            }
        }
    }

    if (player_gear->can_pass_through_walls)
    {
        if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, entity, LOCAL_PLAYER_TAG))
        {
            Vector2 player_position = Vector2Add(Vector2Add(get_transform_of_entity(entity)->root->position, (Vector2){ (TILE_SIZE - 5) / 2, (TILE_SIZE - 6) / 2 }), PLAYER_TRANSFORM_OFFSET);
            Vector2 player_map_position = (Vector2){(int)player_position.x / TILE_SIZE, (int)player_position.y / TILE_SIZE};

            if (is_local_player_walking_through_walls)
            {
                if (map_get_cell((int)player_map_position.x, (int)player_map_position.y) != MAP_DEST_WALL)
                {
                    ((collider_t*)list_get_at(get_physics_of_entity(entity)->colliders, 0))->collision_mask |= COLLIDER_DESTRUCTIBLE_WALL;

                    player_gear->can_pass_through_walls = false;
                    is_local_player_walking_through_walls = false;
                }
            }
            else
            {
                if (map_get_cell((int)player_map_position.x, (int)player_map_position.y) == MAP_DEST_WALL)
                {
                    is_local_player_walking_through_walls = true;
                }
            }
        }
    }
}