#include <tdd_ecs.h>
#include <tdd.h>

#include "../animation.h"
#include "../components.h"
#include "../families.h"

static unsigned long current_animation_time = 0;

static void animate_entity_system(entity_system_t*, world_t*, float, entity_t*);
static void animate_entity_system_begin(entity_system_t*, world_t*, list_t*);
static bool update_animation(animation_t*);

void world_register_animate_entity_system(world_t* world, tdd_timer_t* timer)
{
    entity_system_t* system = world_register_entity_system(world, get_animated_family(), animate_entity_system);

    system->base_system.data = timer;
    system->begin_func = animate_entity_system_begin;
}

static void animate_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    animated_t* animated = get_animated_of_entity(entity);
    list_node_t* current_animated_sprite_node = animated->animated_sprites->head;

    while (current_animated_sprite_node)
    {
        animated_sprite_t* animated_sprite = (animated_sprite_t*)current_animated_sprite_node->data;
        animation_t* animation = animated_sprite->current_animation;

        if (update_animation(animation))
        {
            animated_sprite->sprite->atlas_region_rect = &(GET_TEXTURE_ATLAS_REGION(animation->atlas_name, animation->frames[animation->current_frame]))->rect;
        }

        current_animated_sprite_node = current_animated_sprite_node->next;
    }
}

static void animate_entity_system_begin(entity_system_t* system, world_t* world, list_t* entities)
{
    current_animation_time = ((tdd_timer_t*)system->base_system.data)->elapsed_ms;
}

static bool update_animation(animation_t* animation)
{
    // if animation finished
    if (!animation->loop && animation->current_frame == animation->frames_count - 1)
    {
        return false;
    }

    if (!animation->started)
    {
        animation->last_frame_time = current_animation_time;
        animation->started = true;
    }

    unsigned int frame_time = 1000 / animation->fps;

    if (current_animation_time - animation->last_frame_time >= frame_time)
    {
        animation->current_frame++;
        animation->last_frame_time = current_animation_time;
    }

    animation->current_frame = animation->current_frame % animation->frames_count;

    return true;
}