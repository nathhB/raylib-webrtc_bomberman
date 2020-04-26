#include <tdd_ecs.h>
#include <tdd.h>

#include "../../../components.h"
#include "../../../families.h"

static entity_t* tested_entity = NULL;
static list_t* physics_entities = NULL;
static list_t* collisions = NULL;
static list_t* previous_collisions = NULL;

static void collision_entity_system(entity_system_t*, world_t*, float, entity_t*);
static void collision_entity_system_begin(entity_system_t*, world_t*, list_t*);
static void collision_entity_system_end(entity_system_t*, world_t*, list_t*);
static void collision_entity_system_cleanup(system_t*);
static void collision_entity_on_entity_destroyed(entity_system_t*, world_t*, entity_t*);
static void update_collider(collider_t*);
static void test_for_collisions(void*);
static bool test_collision(collider_t*, collider_t*, collision_t**);
static void resolve_collision(entity_t*, collider_t*, collision_t*);
static void handle_enter_hooks(world_t*, collision_t*);
static void handle_exit_hooks(world_t*, collision_t*);
static void clear_collision(void**);

void world_register_collision_entity_system(world_t* world)
{
    entity_system_t* system = world_register_entity_system(world, get_physics_family(), collision_entity_system);

    system->begin_func = collision_entity_system_begin;
    system->end_func = collision_entity_system_end;
    system->on_entity_destroyed_func = collision_entity_on_entity_destroyed;
    system->base_system.cleanup_func = collision_entity_system_cleanup;
}

static void collision_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    physics_t* physics = get_physics_of_entity(entity);

    if (!physics->is_kinetic)
    {
        // collision with any other physical entity
        tested_entity = entity;

        list_iter(physics_entities, test_for_collisions);
    }
}

static void collision_entity_system_begin(entity_system_t* system, world_t* world, list_t* entities)
{
    physics_entities = entities;
    collisions = list_create();

    // update colliders positions of every physics entities

    list_node_t* current_entity_node = entities->head;

    while (current_entity_node)
    {
        entity_t* entity = (entity_t*)current_entity_node->data;
        physics_t* physics = get_physics_of_entity(entity);

        list_node_t* current_collider_node = physics->colliders->head;

        while (current_collider_node)
        {
            update_collider((collider_t*)current_collider_node->data);

            current_collider_node = current_collider_node->next;
        }

        current_entity_node = current_entity_node->next;
    }
}

static void collision_entity_system_cleanup(system_t* system)
{
    previous_collisions = NULL;
}

static void collision_entity_on_entity_destroyed(entity_system_t* entity_system, world_t* world, entity_t* entity)
{
    if (previous_collisions)
    {
        list_node_t *current_node = previous_collisions->head;

        while (current_node)
        {
            collision_t* collision = (collision_t*)current_node->data;
            list_node_t* next_node = current_node->next;

            if (collision->entity_a->id == entity->id || collision->entity_b->id == entity->id)
            {
                list_remove(previous_collisions, collision);
            }

            current_node = next_node;
        }
    }
}

static collision_t* current_collision = NULL;

static bool collision_predicate(void* collision_ptr)
{
    collision_t* collision = (collision_t*)collision_ptr;

    return collision->collider_a == current_collision->collider_a && collision->collider_b == current_collision->collider_b;
}

static void collision_entity_system_end(entity_system_t* system, world_t* world, list_t* entities)
{
    if (previous_collisions)
    {
        list_node_t *current_node = collisions->head;

        while (current_node)
        {
            current_collision = (collision_t*)current_node->data;

            if (!list_one(previous_collisions, collision_predicate)) // previous_collisions does not contains the collision
            {
                // which means it's a new one and we need to call enter hooks
                handle_enter_hooks(world, current_collision);
            }

            current_node = current_node->next;
        }

        current_node = previous_collisions->head;

        while (current_node)
        {
            current_collision = (collision_t*)current_node->data;

            if (!list_one(collisions, collision_predicate)) // collisions does not contains the previous collision
            {
                // which means the previous collision does no longer exist
                // and we need to call exit hooks
                handle_exit_hooks(world, current_collision);
            }

            current_node = current_node->next;
        }

        list_destroy(&previous_collisions, true, clear_collision);
    }

    previous_collisions = collisions;
}

static void update_collider(collider_t* collider)
{
    aabb_update(collider->aabb, collider->transform_node->position);
}

static void test_for_collisions(void* item)
{
    entity_t* other_entity = (entity_t*)item;

    if (other_entity->id != tested_entity->id) // don't test the entity with itself
    {
        list_t* tested_entity_colliders = get_physics_of_entity(tested_entity)->colliders;
        list_t* other_entity_colliders = get_physics_of_entity(other_entity)->colliders;
        list_node_t* tested_entity_colliders_current_node = tested_entity_colliders->head;

        while (tested_entity_colliders_current_node) // loop over all tested entity colliders
        {
            collider_t* tested_entity_collider = (collider_t*)tested_entity_colliders_current_node->data;
            list_node_t* other_entity_colliders_current_node = other_entity_colliders->head;

            while (other_entity_colliders_current_node) // then loop over tested against entity colliders
            {
                collider_t* other_entity_collider = (collider_t*)other_entity_colliders_current_node->data;
                collision_t* collision = NULL;
                bool can_collide = ((other_entity_collider->collision_mask & tested_entity_collider->collision_flag) != 0) &&
                    ((tested_entity_collider->collision_mask & other_entity_collider->collision_flag) != 0);

                if (can_collide && test_collision(tested_entity_collider, other_entity_collider, &collision))
                {
                    collision->entity_a = tested_entity;
                    collision->entity_b = other_entity;

                    if (!(tested_entity_collider->is_trigger || other_entity_collider->is_trigger))
                    {
                        if (entity_has_movement(tested_entity))
                        {
                            movement_t* movement = get_movement_of_entity(tested_entity);

                            collision->velocity = (Vector2){ movement->x_velocity, movement->y_velocity };
                        }

                        resolve_collision(tested_entity, tested_entity_collider, collision);
                    }

                    list_push_back(collisions, collision);
                }

                other_entity_colliders_current_node = other_entity_colliders_current_node->next;
            }

            tested_entity_colliders_current_node = tested_entity_colliders_current_node->next;
        }
    }
}

static bool test_collision(collider_t* c1, collider_t* c2, collision_t** collision)
{
    projection_t c1_x_proj = aabb_project_on_x(c1->aabb);
    projection_t c2_x_proj = aabb_project_on_x(c2->aabb);

    if (projection_does_overlap(&c1_x_proj, &c2_x_proj))
    {
        projection_t c1_y_proj = aabb_project_on_y(c1->aabb);
        projection_t c2_y_proj = aabb_project_on_y(c2->aabb);

        if (projection_does_overlap(&c1_y_proj, &c2_y_proj))
        {
            float x_overlap = projection_get_overlap(&c1_x_proj, &c2_x_proj);
            float y_overlap = projection_get_overlap(&c1_y_proj, &c2_y_proj);
            float overlap;
            Vector2 axis = { 0, 0 };

            if (x_overlap < y_overlap)
            {
                overlap = x_overlap;
                axis.x = (c1->transform_node->position.x < c2->transform_node->position.x) ? -1 : 1;
            }
            else
            {
                overlap = y_overlap;
                axis.y = (c1->transform_node->position.y < c2->transform_node->position.y) ? -1 : 1;
            }

            *collision = malloc(sizeof(collision_t));

            (*collision)->mtv = Vector2Scale(axis, overlap);
            (*collision)->collider_a = c1;
            (*collision)->collider_b = c2;

            return true;
        }
    }

    return false;
}

static void resolve_collision(entity_t* entity, collider_t* collider, collision_t* collision)
{
    transform_t *tested_entity_transform = get_transform_of_entity(entity);

    tested_entity_transform->root->position = Vector2Add(tested_entity_transform->root->position, collision->mtv);

    transform_node_update_hierarchy(tested_entity_transform->root);
    update_collider(collider);

    if (entity_has_movement(entity))
    {
        movement_t* movement = get_movement_of_entity(entity);

        if (collision->mtv.x != 0)
        {
            movement->x_velocity = 0;
        }
        else if (collision->mtv.y != 0)
        {
            movement->y_velocity = 0;
        }
    }
}

static void handle_enter_hooks(world_t* world, collision_t* collision)
{
    collider_t* collider_a = collision->collider_a;
    collider_t* collider_b = collision->collider_b;

    if (collider_a->is_trigger || collider_b->is_trigger)
    {
        if (collider_a->on_trigger_enter_func)
        {
            collider_a->on_trigger_enter_func(world, collision);
        }

        if (collider_b->on_trigger_enter_func)
        {
            collider_b->on_trigger_enter_func(world, collision);
        }
    }
    else
    {
        if (collider_a->on_collision_enter_func)
        {
            collider_a->on_collision_enter_func(world, collision);
        }

        if (collider_b->on_collision_enter_func)
        {
            collider_b->on_collision_enter_func(world, collision);
        }
    }
}

static void handle_exit_hooks(world_t* world, collision_t* collision)
{
    if (collision->entity_a->marked_for_deletion || collision->entity_b->marked_for_deletion)
    {
        return;
    }

    collider_t* collider_a = collision->collider_a;
    collider_t* collider_b = collision->collider_b;

    if (collider_a->is_trigger || collider_b->is_trigger)
    {
        if (collider_a->on_trigger_exit_func)
        {
            collider_a->on_trigger_exit_func(world, collision);
        }

        if (collider_b->on_trigger_exit_func)
        {
            collider_b->on_trigger_exit_func(world, collision);
        }
    }
    else
    {
        if (collider_a->on_collision_exit_func)
        {
            collider_a->on_collision_exit_func(world, collision);
        }

        if (collider_b->on_collision_exit_func)
        {
            collider_b->on_collision_exit_func(world, collision);
        }
    }
    
}

static void clear_collision(void** collision)
{
    free(*collision);
}