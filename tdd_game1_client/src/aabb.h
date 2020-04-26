#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include <raylib.h>

#include "vector_utils.h"

typedef struct aabb
{
    Rectangle box;
} aabb_t;

aabb_t*         aabb_create(int, int);
void            aabb_destroy(aabb_t*);
void            aabb_update(aabb_t*, Vector2);
projection_t    aabb_project_on_x(aabb_t*);
projection_t    aabb_project_on_y(aabb_t*);

#endif // AABB_H_INCLUDED