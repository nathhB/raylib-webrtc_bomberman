#include "tdd_ecs.h"

entity_tag_manager_t* entity_tag_manager_create()
{
    entity_tag_manager_t* manager = malloc(sizeof(entity_tag_manager_t));

    manager->tags_table = htable_create();

    return manager;
}

void entity_tag_manager_destroy(entity_tag_manager_t* manager)
{
    htable_destroy(&manager->tags_table, false, NULL, false);
    free(manager);
}

void entity_tag_manager_tag_entity(entity_tag_manager_t* manager, entity_t* entity, const char* tag)
{
    htable_add(manager->tags_table, tag, entity);
}

entity_t* entity_tag_manager_get_entity(entity_tag_manager_t* manager, const char* tag)
{
    return (entity_t*)htable_get(manager->tags_table, tag);
}

bool entity_tag_manager_is_entity_tagged(entity_tag_manager_t* manager, entity_t* entity, const char* tag)
{
    return htable_get(manager->tags_table, tag) == entity;
}

entity_t* entity_tag_manager_remove_entity(entity_tag_manager_t* manager, const char* tag)
{
    return (entity_t*)htable_remove(manager->tags_table, tag);
}