#ifndef TEST_COMPONENTS_H_INCLUDED
#define TEST_COMPONENTS_H_INCLUDED

#include <tdd_ecs.h>

typedef struct a
{
    component_t base;
    int a;
    int b;
} a_t;

typedef struct b
{
    component_t base;
    int c;
    int d;
} b_t;

typedef struct c
{
    component_t base;
    int e;
    int f;
} c_t;

typedef struct d
{
    component_t base;
    int g;
    int h;
} d_t;

typedef struct e
{
    component_t base;
    int i;
    int j;
} e_t;

DECLARE_COMPONENT(a, a_t)
DECLARE_COMPONENT(b, b_t)
DECLARE_COMPONENT(c, c_t)
DECLARE_COMPONENT(d, d_t)
DECLARE_COMPONENT(e, e_t)

#endif // TEST_COMPONENTS_H_INCLUDED
