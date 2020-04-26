#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../game_states.h"

void lifetime_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    unsigned long current_time = ((game_state_data_t*)world->user_data)->timer->elapsed_ms;
    lifetime_t* lifetime = get_lifetime_of_entity(entity);

    if (current_time - lifetime->create_time >= lifetime->lifetime)
    {
        if (!entity->marked_for_deletion)
        {
            if (lifetime->lifetime_expired_func)
            {
                lifetime->lifetime_expired_func(world, entity);
            }

            world_mark_entity_for_deletion(world, entity);
        }
    }
}