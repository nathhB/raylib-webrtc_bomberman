#ifndef ENTITY_GROUP_MANAGER_H_INCLUDED
#define ENTITY_GROUP_MANAGER_H_INCLUDED

typedef struct entity_group_manager
{
    htable_t* groups_table;
} entity_group_manager_t;

entity_group_manager_t* entity_group_manager_create();
void                    entity_group_manager_destroy(entity_group_manager_t*);
void                    entity_group_manager_add_entity(entity_group_manager_t*, entity_t*, const char*);
void                    entity_group_manager_remove_entity(entity_group_manager_t*, entity_t*, const char*);
bool                    entity_group_manager_is_entity_in_group(entity_group_manager_t*, entity_t*, const char*);
list_t*                 entity_group_manager_get_group(entity_group_manager_t*, const char*);

#endif // ENTITY_GROUP_MANAGER_H_INCLUDED