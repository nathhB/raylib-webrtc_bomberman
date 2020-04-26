#include "tdd_ecs.h"

void component_add_to_entity(entity_t* entity, void* component, unsigned int component_id)
{
    B_SET(entity->components_mask, component_id);
    bag_set_at(entity->components, component_id, component);
}

void component_remove_from_entity(entity_t* entity, unsigned int component_id)
{
    B_UNSET(entity->components_mask, component_id);
    
    void* component_ptr = bag_remove_at(entity->components, component_id);

    if (component_ptr != NULL)
    {
        ((component_t*)component_ptr)->destroy_func(component_ptr);
    }

    // free(component_ptr);
}

bool component_entity_has(entity_t* entity, unsigned int component_id)
{
    int mask = B_MASK(component_id);

    return (entity->components_mask & mask) == mask;
}
