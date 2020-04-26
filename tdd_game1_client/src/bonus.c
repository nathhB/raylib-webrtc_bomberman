#include "bonus.h"
#include "components.h"
#include "physics.h"

static char* bonus_texture_altas_names[] = {
    "bonus_extra_bomb", // BONUS_EXTRA_BOMB
    "bonus_explosion_expander", // BONUS_EXPLOSION_EXPANDER,
    "bonus_accelerator", // BONUS_ACCELERATOR,
    "bonus_armor", // BONUS_INDESTRUCTIBLE_ARMOR,
    "bonus_max_explosion", // BONUS_MAX_EXPLOSION,
    "bonus_wall_passer", // BONUS_WALL_PASSER,
    "bonus_kick" // BONUS_KICK
};

void create_bonus_entity(world_t* world, int bonus_type, int x, int y)
{
    entity_t* bonus_entity = world_create_entity(world);
    transform_t* transform = add_transform_to_entity(bonus_entity, x, y);
    physics_t* physics = add_physics_to_entity(bonus_entity, true);

    physics_add_collider(physics, transform->root, collider_create(TILE_SIZE, TILE_SIZE, true, COLLIDER_BONUS));

    render_t* render = add_render_to_entity(bonus_entity);
    sprite_data_t* border_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "bonus_border1"));

    render_add_renderable(
        render, transform->root,
        (renderable_t*)create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", bonus_texture_altas_names[bonus_type - 1])));
    render_add_renderable(render, transform->root, (renderable_t *)border_sprite);

    animated_t *animated = add_animated_to_entity(bonus_entity);
    animated_sprite_t* animated_border_sprite = animated_create_animated_sprite(animated, border_sprite);

    animated_add_animation_to_sprite(animated_border_sprite, "bonus_border",
        animation_create("props_atlas", 20, true, 5, "bonus_border1", "bonus_border2", "bonus_border3", "bonus_border4", "bonus_border5"));
    animated_set_sprite_current_animation(animated_border_sprite, "bonus_border");

    entity_group_manager_add_entity(world->entity_group_manager, bonus_entity, BONUS_ENTITIES_GROUP);
}