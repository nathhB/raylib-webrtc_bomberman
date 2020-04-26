#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <raylib.h>
#include <tdd.h>

typedef struct transform_node transform_node_t;

struct transform_node
{
    Vector2             position;
    Vector2             offset;
    const char*         name;
    transform_node_t*   parent;
    list_t*             children;
    htable_t*           named_children;
};

transform_node_t*   transform_node_create(transform_node_t*, Vector2);
void                transform_node_destroy(transform_node_t*);
transform_node_t*   transform_node_attach_child(transform_node_t*, const char*, Vector2);
void                transform_node_update_hierarchy(transform_node_t*);

#endif //TRANSFORM_H_INCLUDED