#ifndef ENTITY_TAG_MANAGER_H_INCLUDED
#define ENTITY_TAG_MANAGER_H_INCLUDED

typedef struct entity_tag_manager
{
    htable_t* tags_table;
} entity_tag_manager_t;

entity_tag_manager_t*   entity_tag_manager_create();
void                    entity_tag_manager_destroy(entity_tag_manager_t*);
void                    entity_tag_manager_tag_entity(entity_tag_manager_t*, entity_t*, const char*);
entity_t*               entity_tag_manager_get_entity(entity_tag_manager_t*, const char*);
bool                    entity_tag_manager_is_entity_tagged(entity_tag_manager_t*, entity_t*, const char*);
entity_t*               entity_tag_manager_remove_entity(entity_tag_manager_t*, const char*);

#endif // ENTITY_TAG_MANAGER_H_INCLUDED