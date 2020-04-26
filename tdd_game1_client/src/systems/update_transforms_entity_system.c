#include <tdd_ecs.h>
#include <tdd.h>

#include "../components.h"

void update_transforms_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    transform_node_update_hierarchy(get_transform_of_entity(entity)->root);
}