#include "explosion.h"
#include "components.h"
#include "game_states.h"
#include "bomb.h"
#include "physics.h"

#define EXPLOSION_ANIM_FPS 10

static void on_explosion_trigger_enter(world_t*, collision_t*);
static void compute_explosion_rendering(transform_node_t*, render_t* render, animated_t*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void create_explosion_entity(world_t* world, Vector2 explosion_center, unsigned int explosion_range)
{
    int x = explosion_center.x / TILE_SIZE;
    int y = explosion_center.y / TILE_SIZE;

    // horizontal blast

    // compute x offsets
    unsigned int x_offset_left = 0;
    unsigned int x_offset_right = 0;

    for (; map_get_cell(x - (x_offset_left + 1), y) == MAP_GROUND && x_offset_left < explosion_range; x_offset_left++) {}
    for (; map_get_cell(x + (x_offset_right + 1), y) == MAP_GROUND && x_offset_right < explosion_range; x_offset_right++) {}

    // compute width
    unsigned int width = x_offset_left + x_offset_right + 1;

    // vertical blast

    // compute y offset
    unsigned int y_offset_top = 0;
    unsigned int y_offset_down = 0;

    for (; map_get_cell(x, y - (y_offset_top + 1)) == MAP_GROUND && y_offset_top < explosion_range; y_offset_top++) {}
    for (; map_get_cell(x, y + (y_offset_down + 1)) == MAP_GROUND && y_offset_down < explosion_range; y_offset_down++) {}

    // compute height
    unsigned int height = y_offset_top + y_offset_down + 1;

    TraceLog(LOG_INFO, "Computed explosion (x_offset_left: %d, x_offset_right: %d, y_offset_top: %d, y_offset_down: %d)", x_offset_left, x_offset_right, y_offset_top, y_offset_down);

    // create the entity

    entity_t* explosion_entity = world_create_entity(world);
    transform_t* transform = add_transform_to_entity(explosion_entity, (int)explosion_center.x, (int)explosion_center.y);
    physics_t* physics = add_physics_to_entity(explosion_entity, false);
    render_t* render = add_render_to_entity(explosion_entity);
    animated_t* animated = add_animated_to_entity(explosion_entity);

    transform_node_t* h_blast_collider_node = transform_node_attach_child(transform->root, "h_blast_collider", (Vector2) { .x = (int)-(x_offset_left * TILE_SIZE), .y = 0 });
    transform_node_t* v_blast_collider_node = transform_node_attach_child(transform->root, "v_blast_collider", (Vector2) { .x = 0, .y = (int)-(y_offset_top * TILE_SIZE) });

    collider_t* h_blast_collider = physics_add_collider(physics, h_blast_collider_node, collider_create(width * TILE_SIZE, TILE_SIZE, true, COLLIDER_EXPLOSION_BLAST));
    collider_t* v_blast_collider = physics_add_collider(physics, v_blast_collider_node, collider_create(TILE_SIZE, height * TILE_SIZE, true, COLLIDER_EXPLOSION_BLAST));

    h_blast_collider->on_trigger_enter_func = on_explosion_trigger_enter;
    v_blast_collider->on_trigger_enter_func = on_explosion_trigger_enter;

    compute_explosion_rendering(transform->root, render, animated, x, y, x_offset_left, x_offset_right, y_offset_top, y_offset_down, explosion_range);

    add_explosion_to_entity(explosion_entity, explosion_range);
    add_lifetime_to_entity(explosion_entity, EXPLOSION_LIFETIME, NULL)->create_time = ((game_state_data_t*)world->user_data)->timer->elapsed_ms;
}

static void on_explosion_trigger_enter(world_t* world, collision_t* collision)
{
    entity_t* destructed_entity = collision->entity_b;

    if (entity_group_manager_is_entity_in_group(world->entity_group_manager, destructed_entity, DESTRUCTIBLE_ENTITIES_GROUP))
    {
        get_destructible_of_entity(destructed_entity)->on_entity_destructed_func(world, destructed_entity, collision->entity_a, collision->collider_a);
    }
}

static void compute_explosion_rendering(
    transform_node_t* root,
    render_t* render,
    animated_t* animated,
    unsigned int x,
    unsigned int y,
    unsigned int x_offset_left,
    unsigned int x_offset_right,
    unsigned int y_offset_top,
    unsigned int y_offset_down,
    unsigned int explosion_range)
{
    sprite_data_t* center_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "center1"));

    render_add_renderable(render, root, (renderable_t*)center_sprite);

    animated_sprite_t* center_animated_sprite = animated_create_animated_sprite(animated, center_sprite);
    
    animated_add_animation_to_sprite(center_animated_sprite, "center_blast", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "center1", "center2", "center3", "center4", "center5"));
    animated_set_sprite_current_animation(center_animated_sprite, "center_blast");

    for (int i = 1; i < x_offset_left; i++)
    {
        transform_node_t* node = transform_node_attach_child(root, NULL, (Vector2){.x = (int)-(i * TILE_SIZE), .y = 0});
        sprite_data_t* h_repeat_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "h_repeat1"));

        render_add_renderable(render, node, (renderable_t*)h_repeat_sprite);

        animated_sprite_t* h_repeat_animated_sprite = animated_create_animated_sprite(animated, h_repeat_sprite);

        animated_add_animation_to_sprite(h_repeat_animated_sprite, "h_blast", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "h_repeat1", "h_repeat2", "h_repeat3", "h_repeat4", "h_repeat5"));
        animated_set_sprite_current_animation(h_repeat_animated_sprite, "h_blast");
    }

    for (int i = 1; i < x_offset_right; i++)
    {
        transform_node_t* node = transform_node_attach_child(root, NULL, (Vector2){.x = (int)i * TILE_SIZE, .y = 0});
        sprite_data_t *h_repeat_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "h_repeat1"));

        render_add_renderable(render, node, (renderable_t*)h_repeat_sprite);

        animated_sprite_t* h_repeat_animated_sprite = animated_create_animated_sprite(animated, h_repeat_sprite);

        animated_add_animation_to_sprite(h_repeat_animated_sprite, "h_blast", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "h_repeat1", "h_repeat2", "h_repeat3", "h_repeat4", "h_repeat5"));
        animated_set_sprite_current_animation(h_repeat_animated_sprite, "h_blast");
    }

    for (int i = 1; i < y_offset_top; i++)
    {
        transform_node_t* node = transform_node_attach_child(root, NULL, (Vector2){ .x = 0, .y = (int)-(i * TILE_SIZE) });
        sprite_data_t *v_repeat_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "v_repeat1"));

        render_add_renderable(render, node, (renderable_t*)v_repeat_sprite);

        animated_sprite_t* v_repeat_animated_sprite = animated_create_animated_sprite(animated, v_repeat_sprite);

        animated_add_animation_to_sprite(v_repeat_animated_sprite, "v_blast", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "v_repeat1", "v_repeat2", "v_repeat3", "v_repeat4", "v_repeat5"));
        animated_set_sprite_current_animation(v_repeat_animated_sprite, "v_blast");
    }

    for (int i = 1; i < y_offset_down; i++)
    {
        transform_node_t* node = transform_node_attach_child(root, NULL, (Vector2){ .x = 0, .y = (int)i * TILE_SIZE });
        sprite_data_t *v_repeat_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "v_repeat1"));

        render_add_renderable(render, node, (renderable_t*)v_repeat_sprite);

        animated_sprite_t* v_repeat_animated_sprite = animated_create_animated_sprite(animated, v_repeat_sprite);

        animated_add_animation_to_sprite(v_repeat_animated_sprite, "v_blast", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "v_repeat1", "v_repeat2", "v_repeat3", "v_repeat4", "v_repeat5"));
        animated_set_sprite_current_animation(v_repeat_animated_sprite, "v_blast");
    }

    if (x_offset_left > 0)
    {
        transform_node_t *left_end_node = transform_node_attach_child(root, NULL, (Vector2){.x = (int)-(x_offset_left * TILE_SIZE), .y = 0});

        if (x_offset_left == explosion_range)
        {
            sprite_data_t *h_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "l_end1"));

            render_add_renderable(render, left_end_node, (renderable_t*)h_end_sprite);

            animated_sprite_t *h_end_animated_sprite = animated_create_animated_sprite(animated, h_end_sprite);

            animated_add_animation_to_sprite(h_end_animated_sprite, "l_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "l_end1", "l_end2", "l_end3", "l_end4", "l_end5"));
            animated_set_sprite_current_animation(h_end_animated_sprite, "l_blast_end");
        }
        else
        {
            sprite_data_t *h_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "h_repeat1"));

            render_add_renderable(render, left_end_node, (renderable_t *)h_end_sprite);

            animated_sprite_t *h_end_animated_sprite = animated_create_animated_sprite(animated, h_end_sprite);

            animated_add_animation_to_sprite(h_end_animated_sprite, "l_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "h_repeat1", "h_repeat2", "h_repeat3", "h_repeat4", "h_repeat5"));
            animated_set_sprite_current_animation(h_end_animated_sprite, "l_blast_end");
        }
    }

    if (x_offset_right > 0)
    {
        transform_node_t *right_end_node = transform_node_attach_child(root, NULL, (Vector2){.x = (int)x_offset_right * TILE_SIZE, .y = 0});

        if (x_offset_right == explosion_range)
        {
            sprite_data_t *h_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "l_end1"));

            render_add_renderable(render, right_end_node, (renderable_t*)h_end_sprite);

            animated_sprite_t *h_end_animated_sprite = animated_create_animated_sprite(animated, h_end_sprite);

            animated_add_animation_to_sprite(h_end_animated_sprite, "r_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "r_end1", "r_end2", "r_end3", "r_end4", "r_end5"));
            animated_set_sprite_current_animation(h_end_animated_sprite, "r_blast_end");
        }
        else
        {
            sprite_data_t *h_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "h_repeat1"));

            render_add_renderable(render, right_end_node, (renderable_t *)h_end_sprite);

            animated_sprite_t *h_end_animated_sprite = animated_create_animated_sprite(animated, h_end_sprite);

            animated_add_animation_to_sprite(h_end_animated_sprite, "r_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "h_repeat1", "h_repeat2", "h_repeat3", "h_repeat4", "h_repeat5"));
            animated_set_sprite_current_animation(h_end_animated_sprite, "r_blast_end");
        }
    }

    if (y_offset_top > 0)
    {
        transform_node_t *top_end_node = transform_node_attach_child(root, NULL, (Vector2){ .x = 0, .y = (int)-(y_offset_top * TILE_SIZE) });

        if (y_offset_top == explosion_range)
        {
            sprite_data_t *v_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "t_end1"));

            render_add_renderable(render, top_end_node, (renderable_t*)v_end_sprite);

            animated_sprite_t *v_end_animated_sprite = animated_create_animated_sprite(animated, v_end_sprite);

            animated_add_animation_to_sprite(v_end_animated_sprite, "t_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "t_end1", "t_end2", "t_end3", "t_end4", "t_end5"));
            animated_set_sprite_current_animation(v_end_animated_sprite, "t_blast_end");
        }
        else
        {
            sprite_data_t *v_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "t_end1"));

            render_add_renderable(render, top_end_node, (renderable_t *)v_end_sprite);

            animated_sprite_t *v_end_animated_sprite = animated_create_animated_sprite(animated, v_end_sprite);

            animated_add_animation_to_sprite(v_end_animated_sprite, "t_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "v_repeat1", "v_repeat2", "v_repeat3", "v_repeat4", "v_repeat5"));
            animated_set_sprite_current_animation(v_end_animated_sprite, "t_blast_end");
        }
    }

    if (y_offset_down > 0)
    {
        transform_node_t *down_end_node = transform_node_attach_child(root, NULL, (Vector2){.x = 0, .y = (int)y_offset_down * TILE_SIZE});

        if (y_offset_down == explosion_range)
        {
            sprite_data_t *v_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "d_end1"));

            render_add_renderable(render, down_end_node, (renderable_t *)v_end_sprite);

            animated_sprite_t *v_end_animated_sprite = animated_create_animated_sprite(animated, v_end_sprite);

            animated_add_animation_to_sprite(v_end_animated_sprite, "d_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "d_end1", "d_end2", "d_end3", "d_end4", "d_end5"));
            animated_set_sprite_current_animation(v_end_animated_sprite, "d_blast_end");
        }
        else
        {
            sprite_data_t *v_end_sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "d_end1"));

            render_add_renderable(render, down_end_node, (renderable_t *)v_end_sprite);

            animated_sprite_t *v_end_animated_sprite = animated_create_animated_sprite(animated, v_end_sprite);

            animated_add_animation_to_sprite(v_end_animated_sprite, "d_blast_end", animation_create("props_atlas", EXPLOSION_ANIM_FPS, false, 5, "v_repeat1", "v_repeat2", "v_repeat3", "v_repeat4", "v_repeat5"));
            animated_set_sprite_current_animation(v_end_animated_sprite, "d_blast_end");
        }
    }
}