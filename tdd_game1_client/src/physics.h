#ifndef TDD_GAME1_CLIENT_PHYSICS
#define TDD_GAME1_CLIENT_PHYSICS

#include <stdbool.h>
#include <raylib.h>
#include <tdd_ecs.h>

#include "aabb.h"
#include "transform.h"

#define COLLIDER_PLAYER 1
#define COLLIDER_BOMB 2
#define COLLIDER_WALL 4
#define COLLIDER_DESTRUCTIBLE_WALL 8
#define COLLIDER_EXPLOSION_BLAST 16
#define COLLIDER_BONUS 32

typedef struct collider collider_t;
typedef struct collision collision_t;

typedef void (*on_collision_hook_func_t)(world_t*, collision_t*);

struct collider
{
    transform_node_t*           transform_node;
    aabb_t*                     aabb;
    bool                        is_trigger;
    on_collision_hook_func_t    on_trigger_enter_func;
    on_collision_hook_func_t    on_trigger_exit_func;
    on_collision_hook_func_t    on_collision_enter_func;
    on_collision_hook_func_t    on_collision_exit_func;
    int                         collision_flag;
    int                         collision_mask;
};

struct collision
{
    Vector2 mtv;
    entity_t*   entity_a;
    entity_t*   entity_b;
    collider_t* collider_a;
    collider_t* collider_b;
    Vector2     velocity;
};

collider_t* collider_create(unsigned int, unsigned int, bool, int);
void        collider_destroy(collider_t*);

#endif // TDD_GAME1_CLIENT_PHYSICS