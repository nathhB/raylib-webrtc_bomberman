#ifndef TDD_GAME1_CLIENT_COMPONENTS_H_INCLUDED
#define TDD_GAME1_CLIENT_COMPONENTS_H_INCLUDED

#include <tdd_game1.h>
#include <tdd_ecs.h>
#include <tdd.h>

#include "animation.h"
#include "rendering.h"
#include "inputs.h"
#include "transform.h"
#include "physics.h"

typedef struct transform
{
    component_t         base;
    transform_node_t*   root;
} transform_t;

typedef struct physics
{
    component_t base;
    list_t*     colliders;
    bool        is_kinetic;
} physics_t;

typedef struct inputs_controlled
{
    component_t base;
    inputs_t*   inputs;
} inputs_controlled_t;

typedef struct movement
{
    component_t base;
    float       x_velocity;
    float       y_velocity;
    bool        grounded;
} movement_t;

typedef struct render
{
    component_t base;
    list_t*     renderables;
} render_t;

typedef struct remotely_controlled
{
    component_t base;
    int         player_id;
    Vector2     moving_direction;
} remotely_controlled_t;

typedef struct interpolated_position
{
    component_t         base;
    circular_buffer_t*  buffer;
} interpolated_position_t;

typedef void (*lifetime_expired_func_t)(world_t*, entity_t*);

typedef struct lifetime
{
    component_t             base;
    unsigned long           lifetime;
    unsigned long           create_time;
    lifetime_expired_func_t lifetime_expired_func;
} lifetime_t;

typedef void (*on_entity_destructed_func_t)(world_t*, entity_t*, entity_t*, collider_t*);

typedef struct destructible
{
    component_t                 base;
    on_entity_destructed_func_t on_entity_destructed_func;
} destructible_t;

typedef struct animated_sprite
{
    htable_t*       animations;
    animation_t*    current_animation;
    const char*     current_animation_name;
    sprite_data_t*  sprite;
} animated_sprite_t;

typedef struct animated
{
    component_t     base;
    list_t*         animated_sprites;
} animated_t;

typedef struct player_gear
{
    component_t     base;
    unsigned int    bombs_count;
    unsigned int    explosion_range;
    unsigned int    speed;
    bool            is_indestructible;
    unsigned long   armor_equip_time;
    bool            can_kick;
    bool            can_pass_through_walls;
} player_gear_t;

typedef struct bomb
{
    component_t     base;
    unsigned int    explosion_range;
    bool            is_kicked;
    bool            has_player_inside;
} bomb_t;

typedef struct explosion
{
    component_t     base;
    unsigned int    range;
} explosion_t;

typedef struct blink_rendering
{
    component_t     base;
    unsigned int    blink_per_second;
    unsigned long   last_blink_time;
} blink_rendering_t;

typedef struct state_machine
{
    component_t             base;
    finite_state_machine_t* finite_state_machine;
} state_machine_t;

collider_t*         physics_add_collider(physics_t*, transform_node_t*, collider_t*);

animated_sprite_t*  animated_create_animated_sprite(animated_t*, sprite_data_t*);
void                animated_add_animation_to_sprite(animated_sprite_t*, const char*, animation_t*);
void                animated_set_sprite_current_animation(animated_sprite_t*, const char*);

DECLARE_COMPONENT(transform, transform_t)
DECLARE_COMPONENT(physics, physics_t)
DECLARE_COMPONENT(inputs_controlled, inputs_controlled_t)
DECLARE_COMPONENT(movement, movement_t)
DECLARE_COMPONENT(render, render_t)
DECLARE_COMPONENT(remotely_controlled, remotely_controlled_t)
DECLARE_COMPONENT(interpolated_position, interpolated_position_t)
DECLARE_COMPONENT(lifetime, lifetime_t)
DECLARE_COMPONENT(destructible, destructible_t)
DECLARE_COMPONENT(animated, animated_t)
DECLARE_COMPONENT(player_gear, player_gear_t)
DECLARE_COMPONENT(bomb, bomb_t)
DECLARE_COMPONENT(explosion, explosion_t)
DECLARE_COMPONENT(blink_rendering, blink_rendering_t)
DECLARE_COMPONENT(state_machine, state_machine_t)

void render_add_renderable(render_t*, transform_node_t*, renderable_t*);

#endif // TDD_GAME1_CLIENT_COMPONENTS_H_INCLUDED