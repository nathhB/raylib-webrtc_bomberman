#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"
#include "../../../events.h"
#include "../../../network.h"
#include "../../../player.h"
#include "../../../game_states.h"

static void handle_player_update_position_event(world_t*, void*);

void update_player_position_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_UPDATE_POSITION, handle_player_update_position_event);
}

static void handle_player_update_position_event(world_t* world, void* ev_data)
{
    player_update_position_event_t* ev = (player_update_position_event_t*)ev_data;
    tdd_timer_t* timer = ((game_state_data_t*)world->user_data)->timer;
    entity_t* player_entity = entity_tag_manager_get_entity(world->entity_tag_manager, get_player_entity_tag(ev->player_id));

    if (player_entity == NULL)
    {
        return;
    }

    interpolated_position_t* interpolated_position = get_interpolated_position_of_entity(player_entity);
    interpolation_state_t* interpolation_state = malloc(sizeof(interpolation_state_t));

    interpolation_state->position = (Vector2){ .x = ev->x, .y = ev->y };
    interpolation_state->time = timer->elapsed_ms;

    circular_buffer_add(interpolated_position->buffer, interpolation_state);
}