#include <stdlib.h>

#include "aabb.h"
#include "vector_utils.h"

static Vector2 x_axis = { .x = 1, .y = 0 };
static Vector2 y_axis = { .x = 0, .y = -1 };

aabb_t* aabb_create(int width, int height)
{
    aabb_t* aabb = malloc(sizeof(aabb_t));

    aabb->box.x = 0;
    aabb->box.y = 0;
    aabb->box.width = width;
    aabb->box.height = height;

    return aabb;
}

void aabb_destroy(aabb_t* aabb)
{
    free(aabb);
}

void aabb_update(aabb_t* aabb, Vector2 position)
{
    aabb->box.x = position.x;
    aabb->box.y = position.y;
}

projection_t aabb_project_on_x(aabb_t* aabb)
{
    projection_t p;
    Vector2 x1 = { .x = aabb->box.x, .y = 0 };
    Vector2 x2 = { .x = aabb->box.x + aabb->box.width, .y = 0 };
    float x1_dot = vec2_dot_product(&x1, &x_axis);
    float x2_dot = vec2_dot_product(&x2, &x_axis);

    p.min = (x1_dot < x2_dot) ? x1_dot : x2_dot;
    p.max = (x1_dot > x2_dot) ? x1_dot : x2_dot;

    return p;
}

projection_t aabb_project_on_y(aabb_t* aabb)
{
    projection_t p;
    Vector2 y1 = { .x = 0, .y = aabb->box.y };
    Vector2 y2 = { .x = 0, .y = aabb->box.y + aabb->box.height };
    float y1_dot = vec2_dot_product(&y1, &y_axis);
    float y2_dot = vec2_dot_product(&y2, &y_axis);

    p.min = (y1_dot < y2_dot) ? y1_dot : y2_dot;
    p.max = (y1_dot > y2_dot) ? y1_dot : y2_dot;

    return p;
}