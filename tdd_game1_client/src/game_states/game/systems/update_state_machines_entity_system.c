#include <tdd_ecs.h>

#include "../../../components.h"

void update_state_machines_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    finite_state_machine_update(get_state_machine_of_entity(entity)->finite_state_machine);
}