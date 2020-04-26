#include <string.h>
#include <tdd_game1.h>

#include "map.h"
#include "components.h"
#include "bomb.h"
#include "events.h"
#include "game_states.h"
#include "physics.h"

static entity_t*    create_wall_entity(world_t*, int, int, int, int);
static entity_t*    create_destructible_wall_entity(world_t*, int, int);
static void         on_destructible_wall_hit_by_explosion(world_t*, entity_t*, entity_t*, collider_t*);
static void         on_destructible_wall_destructed(world_t*, entity_t*);

static map_t saved_map = { 0 };

void map_create_physics(world_t* world)
{
    // vertical walls
    create_wall_entity(world, 0, 0, TILE_SIZE, TILE_SIZE * MAP_HEIGHT);
    create_wall_entity(world, MAP_WIDTH * TILE_SIZE - TILE_SIZE, 0, TILE_SIZE, TILE_SIZE * MAP_HEIGHT);

    // horizontal walls
    create_wall_entity(world, 0, 0, TILE_SIZE * MAP_WIDTH, TILE_SIZE);
    create_wall_entity(world, 0, MAP_HEIGHT * TILE_SIZE - TILE_SIZE, TILE_SIZE * MAP_WIDTH, TILE_SIZE);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (map.cells[y][x] == MAP_WALL && x != 0 && x != MAP_WIDTH - 1 && y != 0 && y != MAP_HEIGHT - 1)
            {
                create_wall_entity(world, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }
        }
    }
}

void map_create_destructible_walls_entities(world_t* world)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (map_get_cell(x, y) == MAP_DEST_WALL)
            {
                create_destructible_wall_entity(world, x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
}

void map_save()
{
    memcpy(&saved_map, &map, sizeof(map_t));
}

void map_reset()
{
    memcpy(&map, &saved_map, sizeof(map_t));
}

static entity_t *create_wall_entity(world_t* world, int x, int y, int w, int h)
{
    entity_t *entity = world_create_entity(world);
    transform_t *transform = add_transform_to_entity(entity, x, y);
    physics_t *physics = add_physics_to_entity(entity, true);

    physics_add_collider(physics, transform->root, collider_create(w, h, false, COLLIDER_WALL));

    return entity;
}

static entity_t* create_destructible_wall_entity(world_t* world, int x, int y)
{
    entity_t *entity = world_create_entity(world);
    transform_t *transform = add_transform_to_entity(entity, x, y);
    physics_t *physics = add_physics_to_entity(entity, true);

    physics_add_collider(physics, transform->root, collider_create(TILE_SIZE, TILE_SIZE, false, COLLIDER_DESTRUCTIBLE_WALL));

    render_t* render = add_render_to_entity(entity);

    render_add_renderable(render, transform->root, (renderable_t *)create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("props_atlas", "destructible_wall")));

    add_destructible_to_entity(entity, on_destructible_wall_hit_by_explosion);

    entity_group_manager_add_entity(world->entity_group_manager, entity, DESTRUCTIBLE_ENTITIES_GROUP);

    return entity;
}

static void on_destructible_wall_hit_by_explosion(world_t* world, entity_t* entity, entity_t* explosion_entity, collider_t* blast_collider)
{
    Vector2 position = get_transform_of_entity(entity)->root->position;
    Vector2 explosion_center = blast_collider->transform_node->parent->position;
    unsigned int explosion_range = get_explosion_of_entity(explosion_entity)->range;
    bool destruct = false;

    if (strcmp(blast_collider->transform_node->name, "h_blast_collider") == 0)
    {
        destruct = ((int)blast_collider->transform_node->position.y == (int)position.y) &&
            ABS((int)position.x / TILE_SIZE - (int)explosion_center.x / TILE_SIZE) <= explosion_range;
    }
    else if (strcmp(blast_collider->transform_node->name, "v_blast_collider") == 0)
    {
        destruct = (int)blast_collider->transform_node->position.x == (int)position.x &&
            ABS((int)position.y / TILE_SIZE - (int)explosion_center.y / TILE_SIZE) <= explosion_range;
    }

    if (destruct)
    {
        animated_t* animated = add_animated_to_entity(entity);
        animated_sprite_t* animated_sprite = animated_create_animated_sprite(animated, list_get_at(get_render_of_entity(entity)->renderables, 0));

        animated_add_animation_to_sprite(animated_sprite,
                                         "destruction",
                                         animation_create("props_atlas", 10, false, 6, "destructible_wall_destroy1", "destructible_wall_destroy2", "destructible_wall_destroy3", "destructible_wall_destroy4", "destructible_wall_destroy5", "destructible_wall_destroy6"));
        animated_set_sprite_current_animation(animated_sprite, "destruction");

        add_lifetime_to_entity(entity, 600, on_destructible_wall_destructed)->create_time = ((game_state_data_t *)world->user_data)->timer->elapsed_ms;
    }
}

static void on_destructible_wall_destructed(world_t* world, entity_t* wall_entity)
{
    Vector2 position = get_transform_of_entity(wall_entity)->root->position;
    int map_x = (int)position.x / TILE_SIZE;
    int map_y = (int)position.y / TILE_SIZE;
    int bonus_type = map_get_bonus(map_x, map_y);

    if (bonus_type)
    {
        create_bonus_spawned_event(bonus_type, map_x, map_y);
    }

    entity_group_manager_remove_entity(world->entity_group_manager, wall_entity, DESTRUCTIBLE_ENTITIES_GROUP);
    map_set_cell(map_x, map_y, MAP_GROUND);
}