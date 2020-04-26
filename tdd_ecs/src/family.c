#include "tdd_ecs.h"

static bool entity_mathces_all(family_t*, entity_t*);
static bool entity_mathces_any(family_t*, entity_t*);
static bool entity_mathces_not(family_t*, entity_t*);

family_t* family_create()
{
    family_t* family = malloc(sizeof(family_t));

    family->all_mask = 0;
    family->any_mask = 0;
    family->not_mask = 0;

    return family;
}

void family_destroy(family_t* family)
{
    free(family);
}

void family_mask_set(int* family_mask, ...)
{
    va_list args;
    va_start(args, family_mask);

    int component_id;

    while ((component_id = va_arg(args, int)) >= 0)
    {
        B_SET(*family_mask, component_id);
    }

    va_end(args);
}

bool family_entity_is_part_of(family_t* family, entity_t* entity)
{
    return(
              entity_mathces_all(family, entity) &&
              entity_mathces_any(family, entity) &&
              entity_mathces_not(family, entity)
          );
}

static bool entity_mathces_all(family_t* family, entity_t* entity)
{
    return (entity->components_mask & family->all_mask) == family->all_mask;
}

static bool entity_mathces_any(family_t* family, entity_t* entity)
{
    return family->any_mask == 0 || (entity->components_mask & family->any_mask) > 0;
}

static bool entity_mathces_not(family_t* family, entity_t* entity)
{
    return (entity->components_mask & family->not_mask) == 0;
}
