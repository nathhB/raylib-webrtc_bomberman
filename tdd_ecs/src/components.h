#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "tdd_ecs.h"

#define MAX_COMPONENTS 32

typedef void (*component_destroy_func_t)(void*);

typedef struct component
{
    component_destroy_func_t destroy_func;
} component_t;

#define DECLARE_COMPONENT(component_name, component_type) \
void component_name##_initialize(component_type*, va_list); \
void component_name##_destroy(component_type*); \
\
static int component_name##_id = __COUNTER__; \
\
static inline component_type* add_##component_name##_to_entity(entity_t* e, ...) \
{ \
    component_type* c = malloc(sizeof(component_type)); \
\
    c->base.destroy_func = (component_destroy_func_t)component_name##_destroy; \
\
    va_list args; \
    va_start(args, e); \
    component_add_to_entity(e, c, component_name##_id); \
    component_name##_initialize(c, args); \
    va_end(args); \
    return c; \
} \
static inline void remove_##component_name##_from_entity(entity_t* e) \
{ \
    component_remove_from_entity(e, component_name##_id); \
} \
\
static inline component_type* get_##component_name##_of_entity(entity_t* e) \
{ \
    return bag_get_at(e->components, component_name##_id); \
} \
\
static inline bool entity_has_##component_name(entity_t* e) \
{ \
    return component_entity_has(e, component_name##_id); \
} \
static inline int get_##component_name##_id() \
{ \
    return component_name##_id; \
}

void component_add_to_entity(entity_t*, void*, unsigned int);
void component_remove_from_entity(entity_t*, unsigned int);
bool component_entity_has(entity_t*, unsigned int);

#endif // COMPONENTS_H_INCLUDED
