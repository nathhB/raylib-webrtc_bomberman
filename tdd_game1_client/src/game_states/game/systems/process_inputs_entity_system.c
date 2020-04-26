#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"
#include "../../../player.h"
#include "../../../game_states.h"
#include "../../../events.h"
#include "../../../network.h"
#include "../../../bomb.h"

static void handle_drop_bombs(world_t*, entity_t*, inputs_t*);
static void drop_bomb(world_t*, int, Vector2);

static unsigned int next_bomb_id = 0;

void process_inputs_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* player_entity)
{
    inputs_t* inputs = get_inputs_controlled_of_entity(player_entity)->inputs;
    movement_t* movement = get_movement_of_entity(player_entity);
    player_gear_t *player_gear = get_player_gear_of_entity(player_entity);

    movement->x_velocity = inputs->h_axis * (int)player_gear->speed;
    movement->y_velocity = inputs->v_axis * (int)player_gear->speed;

    handle_drop_bombs(world, player_entity, inputs);
}

static void handle_drop_bombs(world_t* world, entity_t* player_entity, inputs_t* inputs)
{
    if (inputs->space_pressed)
    {
        Vector2 player_position = Vector2Add(get_transform_of_entity(player_entity)->root->position, PLAYER_TRANSFORM_OFFSET);
        player_gear_t* player_gear = get_player_gear_of_entity(player_entity);

        if (entity_group_manager_get_group(world->entity_group_manager, LOCAL_PLAYER_BOMBS_GROUP)->count < player_gear->bombs_count)
        {
            Vector2 bomb_position = (Vector2){
                ((int)player_position.x + TILE_SIZE / 2) / TILE_SIZE,
                ((int)player_position.y + TILE_SIZE / 2) / TILE_SIZE};

            // don't drop a new bomb only if we are on the ground
            if (map_get_cell(bomb_position.x, bomb_position.y) == MAP_GROUND)
            {
                drop_bomb(world, inputs->player_id, bomb_position);
            }
        }
    }
}

static void drop_bomb(world_t* world, int player_id, Vector2 bomb_position)
{
    unsigned int bomb_id = next_bomb_id++;

    create_player_drop_bomb_event(player_id, bomb_id, bomb_position);

    player_drop_bomb_net_message_t* drop_bomb_net_message = player_drop_bomb_net_message_create();

    drop_bomb_net_message->player_id = player_id;
    drop_bomb_net_message->bomb_id = bomb_id;
    drop_bomb_net_message->x = bomb_position.x;
    drop_bomb_net_message->y = bomb_position.y;

    SEND_RELIABLE(drop_bomb_net_message);
}