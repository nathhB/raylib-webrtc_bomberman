#include <stdlib.h>

#include "families.h"
#include "components.h"

static struct tdd_game1_client_families fams = {
    .transform_family = NULL,
    .physics_family = NULL,
    .inputs_controlled_family = NULL,
    .movement_family = NULL,
    .render_family = NULL,
    .remotely_controlled_family = NULL,
    .position_interpolated_family = NULL,
    .lifetime_family = NULL,
    .animated_family = NULL,
    .state_machine_family = NULL,
    .player_gear_family = NULL,
    .blink_rendering_family = NULL
};

void destroy_tdd_game1_client_families()
{
    family_destroy(fams.transform_family);
    family_destroy(fams.inputs_controlled_family);
    family_destroy(fams.movement_family);
    family_destroy(fams.physics_family);
    family_destroy(fams.render_family);
    family_destroy(fams.remotely_controlled_family);
    family_destroy(fams.position_interpolated_family);
    family_destroy(fams.lifetime_family);
    family_destroy(fams.animated_family);
    family_destroy(fams.state_machine_family);
    family_destroy(fams.player_gear_family);
    family_destroy(fams.blink_rendering_family);
}

family_t* get_transform_family()
{
    if (fams.transform_family == NULL)
    {
        fams.transform_family = family_create();

        FAM_ALL_OF(fams.transform_family, get_transform_id());
    }

    return fams.transform_family;
}

family_t* get_physics_family()
{
    if (fams.physics_family == NULL)
    {
        fams.physics_family = family_create();

        FAM_ALL_OF(fams.physics_family, get_transform_id(), get_physics_id());
    }

    return fams.physics_family;
}

family_t* get_inputs_controlled_family()
{
    if (fams.inputs_controlled_family == NULL)
    {
        fams.inputs_controlled_family = family_create();

        FAM_ALL_OF(fams.inputs_controlled_family, get_inputs_controlled_id());
    }

    return fams.inputs_controlled_family;
}

family_t* get_movement_family()
{
    if (fams.movement_family == NULL)
    {
        fams.movement_family = family_create();

        FAM_ALL_OF(fams.movement_family, get_transform_id(), get_movement_id());
    }

    return fams.movement_family;
}

family_t* get_render_family()
{
    if (fams.render_family == NULL)
    {
        fams.render_family = family_create();

        FAM_ALL_OF(fams.render_family, get_transform_id(), get_render_id());
    }

    return fams.render_family;
}

family_t* get_remotely_controlled_family()
{
    if (fams.remotely_controlled_family == NULL)
    {
        fams.remotely_controlled_family = family_create();

        FAM_ALL_OF(fams.remotely_controlled_family, get_remotely_controlled_id());
    }

    return fams.remotely_controlled_family;
}

family_t* get_position_interpolated_family()
{
    if (fams.position_interpolated_family == NULL)
    {
        fams.position_interpolated_family = family_create();

        FAM_ALL_OF(fams.position_interpolated_family, get_transform_id(), get_remotely_controlled_id(), get_interpolated_position_id());
    }

    return fams.position_interpolated_family;
}

family_t* get_lifetime_family()
{
    if (fams.lifetime_family == NULL)
    {
        fams.lifetime_family = family_create();

        FAM_ALL_OF(fams.lifetime_family, get_lifetime_id());
    }

    return fams.lifetime_family;
}

family_t* get_animated_family()
{
    if (fams.animated_family == NULL)
    {
        fams.animated_family = family_create();

        FAM_ALL_OF(fams.animated_family, get_render_id(), get_animated_id());
    }

    return fams.animated_family;
}

family_t* get_state_machine_family()
{
    if (fams.state_machine_family == NULL)
    {
        fams.state_machine_family = family_create();

        FAM_ALL_OF(fams.state_machine_family, get_state_machine_id());
    }

    return fams.state_machine_family;
}

family_t* get_player_gear_family()
{
    if (fams.player_gear_family == NULL)
    {
        fams.player_gear_family = family_create();

        FAM_ALL_OF(fams.player_gear_family, get_player_gear_id());
    }

    return fams.player_gear_family;
}

family_t* get_blink_rendering_family()
{
    if (fams.blink_rendering_family == NULL)
    {
        fams.blink_rendering_family = family_create();

        FAM_ALL_OF(fams.blink_rendering_family, get_render_id(), get_blink_rendering_id());
    }

    return fams.blink_rendering_family;
}