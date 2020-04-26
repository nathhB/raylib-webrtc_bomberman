#include <tdd_ecs.h>

#include "../../../components.h"

void movement_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    transform_t* transform = get_transform_of_entity(entity);
    movement_t* movement = get_movement_of_entity(entity);

    transform->root->position.x += movement->x_velocity;
    transform->root->position.y += movement->y_velocity;
}