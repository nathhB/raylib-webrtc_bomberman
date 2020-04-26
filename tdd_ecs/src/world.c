#include "tdd_ecs.h"

static void world_process_systems(world_t*, float);
static void world_destroy_marked_for_deletion_entities(world_t*);
static void entity_system_process_func(system_t*, world_t*, float);
static void consume_event(event_t*);
static void clear_entity(void**);
static void clear_system(void**);
static void clear_event_bus(void**);
static void handle_entity_destroyed_entity_systems_callbacks(world_t*, entity_t*);

world_t* world_create()
{
    world_t* world = malloc(sizeof(world_t));

    world->next_entity_id = 0;
    world->entities = list_create();
    world->systems = list_create();
    world->entity_tag_manager = entity_tag_manager_create();
    world->entity_group_manager = entity_group_manager_create();
    world->event_bus = bag_create(MAX_EVENT_CHANNELS);
    world->user_data = NULL;

    for (int i = 0; i < world->event_bus->capacity; i++)
    {
        bag_append(world->event_bus, list_create());
    }

    return world;
}

void world_destroy(world_t* world)
{
    list_destroy(&world->entities, true, clear_entity);
    list_destroy(&world->systems, true, clear_system);
    bag_destroy(world->event_bus, clear_event_bus);
    entity_tag_manager_destroy(world->entity_tag_manager);
    entity_group_manager_destroy(world->entity_group_manager);
    free(world);
}

entity_t* world_create_entity(world_t* world)
{
    entity_t* e = malloc(sizeof(entity_t));

    e->id = world->next_entity_id++;
    e->components_mask = 0;
    e->components = bag_create(MAX_COMPONENTS);
    e->marked_for_deletion = false;

    list_push_back(world->entities, e);

    return e;
}

void world_destroy_entity(world_t* world, entity_t* entity)
{
    void* entity_ptr = list_remove(world->entities, entity);

    if (entity_ptr == entity)
    {
        handle_entity_destroyed_entity_systems_callbacks(world, entity);
        clear_entity(&entity_ptr);
    }
}

void world_mark_entity_for_deletion(world_t* world, entity_t* entity)
{
    entity->marked_for_deletion = true;
}

system_t* world_register_system(world_t* world, system_process_func_t process_func)
{
    system_t* system = malloc(sizeof(system_t));

    system->type = PROCESS_SYSTEM;
    system->process_func = process_func;
    system->cleanup_func = NULL;
    system->is_enabled = true;

    list_push_back(world->systems, system);

    return system;
}

entity_system_t* world_register_entity_system(world_t* world, family_t* family, entity_system_process_func_t entity_process_func)
{
    entity_system_t* entity_system = malloc(sizeof(entity_system_t));

    entity_system->base_system.type = ENTITY_SYSTEM;
    entity_system->family = family;
    entity_system->process_entity_func = entity_process_func;
    entity_system->begin_func = NULL;
    entity_system->end_func = NULL;
    entity_system->on_entity_destroyed_func = NULL;
    entity_system->base_system.process_func = entity_system_process_func;
    entity_system->base_system.cleanup_func = NULL;
    entity_system->base_system.is_enabled = true;

    list_push_back(world->systems, entity_system);

    return entity_system;
}

system_t* world_get_system(world_t* world, system_process_func_t process_func)
{
    list_node_t* current_system_node = world->systems->head;

    while (current_system_node)
    {
        system_t* system = (system_t*)current_system_node->data;

        if (system->process_func == process_func)
        {
            return system;
        }

        current_system_node = current_system_node->next;
    }

    return NULL;
}

entity_system_t* world_get_entity_system(world_t* world, entity_system_process_func_t process_func)
{
    list_node_t *current_system_node = world->systems->head;

    while (current_system_node)
    {
        system_t* system = (system_t*)current_system_node->data;

        if (system->type == ENTITY_SYSTEM && ((entity_system_t*)system)->process_entity_func == process_func)
        {
            return (entity_system_t*)system;
        }

        current_system_node = current_system_node->next;
    }

    return NULL;
}

void world_update(world_t* world, float delta)
{
    world_process_systems(world, delta);
    world_destroy_marked_for_deletion_entities(world);
}

list_t* world_get_all_entities_of_family(world_t* world, family_t* family)
{
    list_t* entities = list_create();
    list_node_t* current_node = world->entities->head;

    while (current_node != NULL)
    {
        entity_t* entity = current_node->data;

        if (family_entity_is_part_of(family, entity))
        {
            list_push_back(entities, entity);
        }

        current_node = current_node->next;
    }

    return entities;
}

void world_push_event(world_t* world, unsigned int type, void* data)
{
    event_t* ev = malloc(sizeof(event_t));

    ev->type = type;
    ev->data = data;
    ev->consume_func = NULL;

    list_push_back(bag_get_at(world->event_bus, type), ev);
}

void world_consume_events(world_t* world, unsigned int type, handle_world_event_func_t handle_func)
{
    list_t* events_channel = (list_t*)bag_get_at(world->event_bus, type);

    while (events_channel->count > 0)
    {
        event_t* ev = (event_t*)list_remove_at(events_channel, 0);

        handle_func(world, ev->data);
        consume_event(ev);
    }
}

static void world_process_systems(world_t* world, float delta)
{
    list_node_t* current_system_node = world->systems->head;

    while (current_system_node)
    {
        system_t* system = current_system_node->data;

        if (system && system->is_enabled)
        {
            system->process_func(system, world, delta);
        }

        current_system_node = current_system_node->next;
    }
}

static void world_destroy_marked_for_deletion_entities(world_t* world)
{
    list_node_t* current_node = world->entities->head;

    while (current_node)
    {
        entity_t* entity = (entity_t*)current_node->data;
        list_node_t* next_node = current_node->next;

        if (entity->marked_for_deletion)
        {
            world_destroy_entity(world, entity);
        }

        current_node = next_node;
    }

    // list_remove_if(world->entities, is_entity_marked_for_deletion, clear_entity);
}

static void entity_system_process_func(system_t* system, world_t* world, float delta)
{
    entity_system_t* entity_system = (entity_system_t*)system;
    list_t* entities = world_get_all_entities_of_family(world, entity_system->family);

    if (entity_system->begin_func)
    {
        entity_system->begin_func(entity_system, world, entities);
    }

    list_node_t* current_node = entities->head;

    while (current_node != NULL)
    {
        entity_t* entity = current_node->data;

        entity_system->process_entity_func(entity_system, world, delta, entity);

        current_node = current_node->next;
    }

    if (entity_system->end_func)
    {
        entity_system->end_func(entity_system, world, entities);
    }

    list_destroy(&entities, false, NULL);
}

static void clear_entity(void** entity_ptr)
{
    entity_t* entity = (entity_t*)*entity_ptr;

    for (int i = 0; i < entity->components->capacity; i++)
    {
        component_t* component = (component_t*)bag_get_at(entity->components, i);

        if (component)
        {
            component->destroy_func(component);
        }
    }

    free(entity);
}

static void clear_system(void** data)
{
    system_t* system = (system_t*)*data;

    if (system->cleanup_func)
    {
        system->cleanup_func(system);
    }

    free(system);
}

static void consume_event(event_t* event)
{
    if (event->consume_func)
    {
        event->consume_func(event->data);
    }

    free(event->data);
    free(event);
}

static void clear_event(void** data)
{
    free(*data);
}

static void clear_event_bus(void** data)
{
    list_destroy((list_t**)data, true, clear_event);
}

static void handle_entity_destroyed_entity_systems_callbacks(world_t* world, entity_t* entity)
{
    list_node_t* current_node = world->systems->head;

    while (current_node)
    {
        system_t* system = (system_t*)current_node->data;

        if (system->type == ENTITY_SYSTEM)
        {
            entity_system_t* entity_system = (entity_system_t*)system;

            if (family_entity_is_part_of(entity_system->family, entity))
            {
                if (entity_system->on_entity_destroyed_func)
                {
                    entity_system->on_entity_destroyed_func(entity_system, world, entity);
                }
            }
        }

        current_node = current_node->next;
    }
}