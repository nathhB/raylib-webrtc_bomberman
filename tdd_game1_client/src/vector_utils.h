#ifndef VECTOR_UTILS_H_INCLUDED
#define VECTOR_UTILS_H_INCLUDED

#include <raylib.h>

#define VEC2_ZERO (Vector2){0, 0}

typedef struct projection
{
    float min;
    float max;
} projection_t;

float   vec2_dot_product(Vector2*, Vector2*);
Vector2 vec2_mul(Vector2*, float); 
bool    projection_does_overlap(projection_t*, projection_t*);
float   projection_get_overlap(projection_t*, projection_t*);

#endif // VECTOR_UTILS_H_INCLUDED