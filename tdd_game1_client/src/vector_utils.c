#include "vector_utils.h"

float vec2_dot_product(Vector2* v1, Vector2* v2)
{
    return v1->x * v2->x + v1->y * v2->y;
}

Vector2 vec2_mul(Vector2* v, float scalar)
{
    return (Vector2) { .x = v->x * scalar, .y = v->y * scalar };
}

bool projection_does_overlap(projection_t* p1, projection_t* p2)
{
    return
        ((p1->min >= p2->min && p1->min <= p2->max) || (p1->max >= p2->min && p1->max <= p2->max)) ||
        ((p2->min >= p1->min && p2->min <= p1->max) || (p2->max >= p1->min && p2->max <= p1->max));  
}

float projection_get_overlap(projection_t* p1, projection_t* p2)
{
    float max = (p1->max < p2->max) ? p1->max : p2->max;
    float min = (p1->min > p2->min) ? p1->min : p2->min;

    return max - min;
}