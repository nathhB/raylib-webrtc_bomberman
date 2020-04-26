#include <tdd_ecs.h>
#include <tdd.h>

#include "../components.h"

void render_entity_system(entity_system_t *system, world_t *world, float delta, entity_t *entity)
{
    list_node_t* current_node = get_render_of_entity(entity)->renderables->head;

    while (current_node)
    {
        renderable_t* renderable = (renderable_t*)current_node->data;

        if (renderable->is_rendering_enabled)
        {
            switch (renderable->type)
            {
            case RENDERABLE_SHAPE:
                render_shape(renderable->transform_node->position, (renderable_shape_t*)renderable);
                break;

            case RENDERABLE_SPRITE:
                render_sprite(renderable->transform_node->position, (sprite_data_t*)renderable);
                break;

            default:
                break;
            }
        }

        current_node = current_node->next;
    }
}