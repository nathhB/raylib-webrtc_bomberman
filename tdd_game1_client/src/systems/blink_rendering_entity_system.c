#include <tdd.h>
#include <tdd_ecs.h>

#include "../components.h"
#include "../families.h"

static void blink_rendering_entity_system(entity_system_t*, world_t*, float, entity_t*);

void world_register_blink_rendering_entity_system(world_t* world, tdd_timer_t* timer)
{
    entity_system_t* system = world_register_entity_system(world, get_blink_rendering_family(), blink_rendering_entity_system);

    system->base_system.data = timer;
}

static void blink_rendering_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    list_node_t* current_node = get_render_of_entity(entity)->renderables->head;
    blink_rendering_t* blink_rendering = get_blink_rendering_of_entity(entity);
    tdd_timer_t* timer = (tdd_timer_t*)system->base_system.data;

    if (timer->elapsed_ms - blink_rendering->last_blink_time >= (1000 / blink_rendering->blink_per_second))
    {
        while (current_node)
        {
            renderable_t* renderable = (renderable_t*)current_node->data;

            renderable->is_rendering_enabled = !renderable->is_rendering_enabled;

            current_node = current_node->next;
        }

        blink_rendering->last_blink_time = timer->elapsed_ms;
    }
}