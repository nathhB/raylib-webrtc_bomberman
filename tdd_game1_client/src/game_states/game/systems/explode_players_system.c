#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../game_states.h"
#include "../../../events.h"
#include "../../../network.h"
#include "../../../player.h"
#include "../../../bomb.h"

static void handle_player_exploded_event(world_t *, void *);

void explode_players_system(system_t *system, world_t *world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_EXPLODED, handle_player_exploded_event);
}

static void handle_player_exploded_event(world_t* world, void* ev_data)
{
    player_exploded_event_t* ev = (player_exploded_event_t*)ev_data;
    entity_t* player_entity = entity_tag_manager_get_entity(world->entity_tag_manager, get_player_entity_tag(ev->player_id));

    if (player_entity != NULL)
    {
        finite_state_machine_t* player_state_machine = get_state_machine_of_entity(player_entity)->finite_state_machine;

        if (player_is_dead(world, ev->player_id))
        {
            return;
        }

        finite_state_machine_set_current_state(player_state_machine, "dead");

        // delete the entity after 2s
        add_lifetime_to_entity(player_entity, 2000, NULL)->create_time = ((game_state_data_t *)world->user_data)->timer->elapsed_ms;

        bool is_local_player = entity_tag_manager_is_entity_tagged(world->entity_tag_manager, player_entity, LOCAL_PLAYER_TAG);

        if (is_local_player)
        {
            entity_group_manager_remove_entity(world->entity_group_manager, player_entity, DESTRUCTIBLE_ENTITIES_GROUP);
            entity_tag_manager_remove_entity(world->entity_tag_manager, LOCAL_PLAYER_TAG);
        }

        entity_tag_manager_remove_entity(world->entity_tag_manager, get_player_entity_tag(ev->player_id));
        entity_group_manager_remove_entity(world->entity_group_manager, player_entity, PLAYERS_GROUP);

        if (is_local_player)
        {
            player_exploded_net_message_t* player_exploded_net_message = player_exploded_net_message_create();

            player_exploded_net_message->player_id = ev->player_id;

            SEND_RELIABLE(player_exploded_net_message);
        }
    }
}