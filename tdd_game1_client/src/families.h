#ifndef TDD_GAME1_CLIENT_FAMILIES_H_INCLUDED
#define TDD_GAME1_CLIENT_FAMILIES_H_INCLUDED

#include <tdd_ecs.h>

struct tdd_game1_client_families
{
    family_t* transform_family;
    family_t* physics_family;
    family_t* inputs_controlled_family;
    family_t* movement_family;
    family_t* render_family;
    family_t* remotely_controlled_family;
    family_t* position_interpolated_family;
    family_t* lifetime_family;
    family_t* animated_family;
    family_t* state_machine_family;
    family_t* player_gear_family;
    family_t* blink_rendering_family;
};

void        destroy_tdd_game1_client_families();

family_t*   get_transform_family();
family_t*   get_inputs_controlled_family();
family_t*   get_physics_family();
family_t*   get_movement_family();
family_t*   get_render_family();
family_t*   get_remotely_controlled_family();
family_t*   get_position_interpolated_family();
family_t*   get_lifetime_family();
family_t*   get_animated_family();
family_t*   get_state_machine_family();
family_t*   get_player_gear_family();
family_t*   get_blink_rendering_family();

#endif // TDD_GAME1_CLIENT_FAMILIES_H_INCLUDED