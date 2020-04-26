#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "tdd_ecs.h"

#define MAX_EVENT_CHANNELS 32

typedef struct world
{
    unsigned long           next_entity_id;
    list_t*                 entities;
    list_t*                 systems;
    entity_tag_manager_t*   entity_tag_manager;
    entity_group_manager_t* entity_group_manager;
    bag_t*                  event_bus;
    void*                   user_data;
} world_t;

typedef struct system system_t;
typedef struct entity_system entity_system_t;

typedef void (*system_process_func_t)(system_t*, world_t*, float);
typedef void (*system_cleanup_func_t)(system_t*);
typedef void (*entity_system_process_func_t)(entity_system_t*, world_t*, float, entity_t*);
typedef void (*entity_system_on_entity_destroyed_func_t)(entity_system_t*, world_t*, entity_t*);

typedef enum
{
    PROCESS_SYSTEM,
    ENTITY_SYSTEM
} system_type_t;

struct system
{
    system_type_t           type;
    system_process_func_t   process_func;
    system_cleanup_func_t   cleanup_func;
    bool                    is_enabled;
    void*                   data;
};

typedef void (*entity_system_hook_func_t)(entity_system_t*, world_t*, list_t*);

struct entity_system
{
    system_t                                    base_system;
    family_t*                                   family;
    entity_system_process_func_t                process_entity_func;
    entity_system_hook_func_t                   begin_func;
    entity_system_hook_func_t                   end_func;
    entity_system_on_entity_destroyed_func_t    on_entity_destroyed_func;
};

typedef void (*handle_world_event_func_t)(world_t*, void*);
typedef void (*world_event_consume_func_t)(void*);

typedef struct event
{
    unsigned int                type;
    void*                       data;
    world_event_consume_func_t  consume_func;
} event_t;

world_t*            world_create();
void                world_destroy(world_t*);
entity_t*           world_create_entity(world_t*);
void                world_destroy_entity(world_t*, entity_t*);
void                world_mark_entity_for_deletion(world_t*, entity_t*);
system_t*           world_register_system(world_t*, system_process_func_t);
entity_system_t*    world_register_entity_system(world_t*, family_t*, entity_system_process_func_t);
system_t*           world_get_system(world_t*, system_process_func_t);
entity_system_t*    world_get_entity_system(world_t*, entity_system_process_func_t);
void                world_update(world_t*, float);
list_t*             world_get_all_entities_of_family(world_t*, family_t*);
void                world_push_event(world_t*, unsigned int, void*);
void                world_consume_events(world_t*, unsigned int, handle_world_event_func_t);

#endif // WORLD_H_INCLUDED
