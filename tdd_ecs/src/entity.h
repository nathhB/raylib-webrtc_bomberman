#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

typedef struct entity
{
    unsigned long   id;
    int             components_mask;
    bag_t*          components;
    bool            marked_for_deletion;
} entity_t;

#endif // ENTITY_H_INCLUDED
