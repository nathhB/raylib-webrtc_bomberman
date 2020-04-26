#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"
#include "../../../player.h"
#include "../../../game_states.h"

void inputs_system(system_t* system, world_t* world, float delta)
{
    entity_t* player_entity = entity_tag_manager_get_entity(world->entity_tag_manager, LOCAL_PLAYER_TAG);

    if (player_entity == NULL)
    {
        return;
    }
    
    inputs_controlled_t* inputs_controlled = get_inputs_controlled_of_entity(player_entity);
    int h_axis = 0;
    int v_axis = 0;

    if (IsKeyDown(KEY_RIGHT))
    {
        h_axis = 1;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        h_axis = -1;
    }

    if (IsKeyDown(KEY_UP))
    {
        v_axis = -1;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        v_axis = 1;
    }

    inputs_controlled->inputs->player_id = ((game_state_data_t*)world->user_data)->local_player_id;
    inputs_controlled->inputs->h_axis = h_axis;
    inputs_controlled->inputs->v_axis = v_axis;
    inputs_controlled->inputs->space_pressed = IsKeyDown(KEY_SPACE);
}