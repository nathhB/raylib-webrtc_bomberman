#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"

void debug_render_colliders_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    physics_t* physics = get_physics_of_entity(entity);
    list_node_t* current_node = physics->colliders->head;

    while (current_node)
    {
        aabb_t* aabb = ((collider_t*)current_node->data)->aabb;
        
        DrawRectangleLinesEx(aabb->box, 1, GREEN);

        current_node = current_node->next;
    }
}