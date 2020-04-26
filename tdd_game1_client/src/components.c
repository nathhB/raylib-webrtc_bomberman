#include <stdlib.h>

#include "components.h"
#include "player.h"
#include "network.h"
#include "vector_utils.h"

void transform_initialize(transform_t* transform, va_list args)
{
    transform->root = transform_node_create(NULL, VEC2_ZERO);
    transform->root->position = (Vector2) { .x = va_arg(args, int), .y = va_arg(args, int) };
}

void transform_destroy(transform_t* transform)
{
    transform_node_destroy(transform->root);
    free(transform);
}

void physics_initialize(physics_t* physics, va_list args)
{
    physics->colliders = list_create();
    physics->is_kinetic = va_arg(args, int);
}

static void clear_collider(void **collider_ptr)
{
    collider_destroy((collider_t*)*collider_ptr);
}

void physics_destroy(physics_t* physics)
{
    list_destroy(&physics->colliders, true, clear_collider);
    free(physics);
}

collider_t* physics_add_collider(physics_t *physics, transform_node_t *transform_node, collider_t *collider)
{
    collider->transform_node = transform_node;

    list_push_back(physics->colliders, collider);

    return collider;
}

void inputs_controlled_initialize(inputs_controlled_t* inputs_controlled, va_list args)
{
    inputs_t* inputs = malloc(sizeof(inputs_t));

    inputs->player_id = va_arg(args, int);
    inputs->h_axis = 0;
    inputs->space_pressed = false;

    inputs_controlled->inputs = inputs;
}

void inputs_controlled_destroy(inputs_controlled_t* inputs_controlled)
{
    free(inputs_controlled->inputs);
    free(inputs_controlled);
}

void movement_initialize(movement_t* movement, va_list args)
{
    movement->x_velocity = 0;
    movement->y_velocity = 0;
    movement->grounded = false;
}

void movement_destroy(movement_t* movement)
{
    free(movement);
}

void render_initialize(render_t* render, va_list args)
{
    render->renderables = list_create();
}

static void clear_renderable(void** renderable)
{
    free(*renderable);
}

void render_destroy(render_t* render)
{
    list_destroy(&render->renderables, true, clear_renderable);
    free(render);
}

void render_add_renderable(render_t* render, transform_node_t* transform_node, renderable_t* renderable)
{
    renderable->transform_node = transform_node;
    
    list_push_back(render->renderables, renderable);
}

void remotely_controlled_initialize(remotely_controlled_t* remotely_controlled, va_list args)
{
    remotely_controlled->player_id = va_arg(args, int);
    remotely_controlled->moving_direction = VEC2_ZERO;
}

void remotely_controlled_destroy(remotely_controlled_t* remotely_controlled)
{
    free(remotely_controlled);
}

static void clear_interpolation_state(void* interpolation_state_ptr)
{
    free(interpolation_state_ptr);
}

void interpolated_position_initialize(interpolated_position_t* interpolated_position, va_list args)
{
    interpolated_position->buffer = circular_buffer_create(INTERPOLATION_STATES_COUNT, clear_interpolation_state);
}

void interpolated_position_destroy(interpolated_position_t* interpolated_position)
{
    free(interpolated_position);
}

void lifetime_initialize(lifetime_t* lifetime, va_list args)
{
    lifetime->lifetime = va_arg(args, unsigned long);
    lifetime->lifetime_expired_func = va_arg(args, lifetime_expired_func_t);
}

void lifetime_destroy(lifetime_t* lifetime)
{
    free(lifetime);
}

void destructible_initialize(destructible_t* destructible, va_list args)
{
    destructible->on_entity_destructed_func = va_arg(args, on_entity_destructed_func_t);
}

void destructible_destroy(destructible_t* destructible)
{
    free(destructible);
}

void animated_initialize(animated_t* animated, va_list args)
{
    animated->animated_sprites = list_create();
}

static void clear_animation(void** animation_ptr)
{
    animation_destroy((animation_t*)*animation_ptr);
}

static void clear_animated_sprite(void** animated_sprite_ptr)
{
    animated_sprite_t* animated_sprite = (animated_sprite_t*)*animated_sprite_ptr;

    htable_destroy(&animated_sprite->animations, true, clear_animation, false);
}

void animated_destroy(animated_t* animated)
{
    list_destroy(&animated->animated_sprites, true, clear_animated_sprite);
    free(animated);
}

animated_sprite_t* animated_create_animated_sprite(animated_t* animated, sprite_data_t* sprite)
{
    animated_sprite_t* animated_sprite = malloc(sizeof(animated_sprite_t));

    animated_sprite->animations = htable_create();
    animated_sprite->current_animation = NULL;
    animated_sprite->current_animation_name = NULL;
    animated_sprite->sprite = sprite;

    list_push_back(animated->animated_sprites, animated_sprite);

    return animated_sprite;
}

void animated_add_animation_to_sprite(animated_sprite_t* animated_sprite, const char* name, animation_t* animation)
{
    htable_add(animated_sprite->animations, name, animation);
}

void animated_set_sprite_current_animation(animated_sprite_t* animated_sprite, const char* name)
{
    animation_t* animation = (animation_t*)htable_get(animated_sprite->animations, name);

    animated_sprite->current_animation = animation;
    animated_sprite->current_animation_name = name;

    animation_reset(animation);
}

void player_gear_initialize(player_gear_t* player_gear, va_list args)
{
    player_gear->bombs_count = PLAYER_BASE_BOMBS_COUNT;
    player_gear->explosion_range = PLAYER_BASE_EXPLOSION_RANGE;
    player_gear->speed = PLAYER_BASE_SPEED;
    player_gear->is_indestructible = false;
    player_gear->can_kick = false;
    player_gear->can_pass_through_walls = false;
}

void player_gear_destroy(player_gear_t* player_gear)
{
    free(player_gear);
}

void bomb_initialize(bomb_t* bomb, va_list args)
{
    bomb->explosion_range = va_arg(args, unsigned int);
    bomb->is_kicked = false;
    bomb->has_player_inside = false;
}

void bomb_destroy(bomb_t* bomb)
{
    free(bomb);
}

void explosion_initialize(explosion_t* explosion, va_list args)
{
    explosion->range = va_arg(args, unsigned int);
}

void explosion_destroy(explosion_t* explosion)
{
    free(explosion);
}

void blink_rendering_initialize(blink_rendering_t* blink_rendering, va_list args)
{
    blink_rendering->blink_per_second = va_arg(args, unsigned int);
    blink_rendering->last_blink_time = 0;
}

void blink_rendering_destroy(blink_rendering_t* blink_rendering)
{
    free(blink_rendering);
}

void state_machine_initialize(state_machine_t* state_machine, va_list args)
{
    state_machine->finite_state_machine = va_arg(args, finite_state_machine_t*);
}

void state_machine_destroy(state_machine_t* state_machine)
{
    finite_state_machine_destroy(state_machine->finite_state_machine);
    free(state_machine);
}