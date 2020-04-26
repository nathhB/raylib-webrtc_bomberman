#include <raylib.h>
#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../network.h"
#include "../../../game_states.h"

void send_local_player_position_system(system_t* system, world_t* world, float delta)
{
    entity_t *local_player = entity_tag_manager_get_entity(world->entity_tag_manager, "LOCAL_PLAYER");

    if (local_player)
    {
        transform_t* transform = get_transform_of_entity(local_player);
        player_position_net_message_t* net_message = player_position_net_message_create();

        net_message->player_id = ((game_state_data_t*)world->user_data)->local_player_id;
        net_message->x = transform->root->position.x;
        net_message->y = transform->root->position.y;

        SEND_UNRELIABLE(net_message);
    }
}