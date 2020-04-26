#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"
#include "../../../bomb.h"


void update_bombs_colliders_system(system_t *system, world_t *world, float delta)
{
    list_t* bombs_entities = entity_group_manager_get_group(world->entity_group_manager, BOMBS_GROUP);

    if (bombs_entities == NULL)
    {
        return;
    }

    list_node_t* current_node = bombs_entities->head;

    while (current_node)
    {
        entity_t* bomb_entity = (entity_t*)current_node->data;
        
        if (!get_bomb_of_entity(bomb_entity)->has_player_inside)
        {
            ((collider_t *)list_get_at(get_physics_of_entity(bomb_entity)->colliders, 0))->is_trigger = false;
        }

        current_node = current_node->next;
    }
}