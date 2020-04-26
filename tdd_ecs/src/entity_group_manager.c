#include "tdd_ecs.h"

static list_t* get_or_create_group(entity_group_manager_t*, const char*);
static void clear_group(void**);

entity_group_manager_t* entity_group_manager_create()
{
    entity_group_manager_t* manager = malloc(sizeof(entity_group_manager_t));

    manager->groups_table = htable_create();

    return manager;
}

void entity_group_manager_destroy(entity_group_manager_t* manager)
{
    htable_destroy(&manager->groups_table, true, clear_group, false);
    free(manager);
}

void entity_group_manager_add_entity(entity_group_manager_t* manager, entity_t* entity, const char* group_name)
{
    list_push_back(get_or_create_group(manager, group_name), entity);
}

void entity_group_manager_remove_entity(entity_group_manager_t* manager, entity_t* entity, const char* group_name)
{
    list_t* group = entity_group_manager_get_group(manager, group_name);

    if (group)
    {
        list_remove(group, entity);
    }
}

bool entity_group_manager_is_entity_in_group(entity_group_manager_t* manager, entity_t* entity, const char* group_name)
{
    list_t* group = entity_group_manager_get_group(manager, group_name);

    if (group)
    {
        return list_exists(group, entity);
    }
    
    return false;
}

list_t* entity_group_manager_get_group(entity_group_manager_t* manager, const char* group_name)
{
    return (list_t *)htable_get(manager->groups_table, group_name);
}

static list_t* get_or_create_group(entity_group_manager_t* manager, const char* group_name)
{
    list_t* group = entity_group_manager_get_group(manager, group_name);

    if (group == NULL)
    {
        group = list_create();

        htable_add(manager->groups_table, group_name, group);
    }

    return group;
}

static void clear_group(void** group_ptr)
{
    list_t* group = (list_t*)*group_ptr;

    list_destroy(&group, false, NULL);
}