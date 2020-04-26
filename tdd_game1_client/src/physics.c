#include <stdlib.h>

#include "physics.h"

collider_t* collider_create(unsigned int width, unsigned int height, bool is_trigger, int collision_flag)
{
    collider_t* collider = malloc(sizeof(collider_t));

    collider->aabb = aabb_create(width, height);
    
    collider->is_trigger = is_trigger;
    collider->transform_node = NULL;
    collider->on_trigger_enter_func = NULL;
    collider->on_trigger_exit_func = NULL;
    collider->on_collision_enter_func = NULL;
    collider->on_collision_exit_func = NULL;
    collider->collision_flag = collision_flag;
    collider->collision_mask = ~(collider->collision_mask & 0);

    return collider;
}

void collider_destroy(collider_t* collider)
{
    aabb_destroy(collider->aabb);
    free(collider);
}