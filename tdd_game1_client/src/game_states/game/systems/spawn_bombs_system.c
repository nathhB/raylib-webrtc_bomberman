#include <raylib.h>
#include <tdd_ecs.h>
#include <tdd_game1.h>

#include "../../../components.h"
#include "../../../bomb.h"
#include "../../../events.h"
#include "../../../player.h"
#include "../../../game_states.h"

static void handle_player_drop_bomb_event(world_t*, void*);

void spawn_bombs_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_DROP_BOMB, handle_player_drop_bomb_event);
}

static void handle_player_drop_bomb_event(world_t* world, void* ev_data)
{
    player_drop_bomb_event_t* ev = (player_drop_bomb_event_t*)ev_data;
    bool from_local_player = ((game_state_data_t*)world->user_data)->local_player_id == ev->player_id;
    player_gear_t* player_gear = get_player_gear_of_entity(
        entity_tag_manager_get_entity(world->entity_tag_manager, from_local_player ? LOCAL_PLAYER_TAG : get_player_entity_tag(ev->player_id))
    );

    printf("Bomb spawned at (%d, %d), player_id = %d !\n", (int)ev->bomb_position.x, (int)ev->bomb_position.y, ev->player_id);

    create_bomb_entity(world, ev->bomb_position, from_local_player, player_gear->explosion_range);
    map_set_cell((int)ev->bomb_position.x, (int)ev->bomb_position.y, MAP_BOMB);

    // update ui
    if (from_local_player)
    {
        unsigned int remaining_bombs_count = player_gear->bombs_count - entity_group_manager_get_group(world->entity_group_manager, LOCAL_PLAYER_BOMBS_GROUP)->count;

        create_ui_update_event(UI_REMAINING_BOMBS_COUNT, remaining_bombs_count);
    }
}