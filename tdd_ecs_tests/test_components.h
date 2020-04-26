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

#define A_COMPONENT 0
#define B_COMPONENT 1
#define C_COMPONENT 2
#define D_COMPONENT 3
#define E_COMPONENT 4

COMPONENT_DECLARATIONS(a)
COMPONENT_DECLARATIONS(b)
COMPONENT_DECLARATIONS(c)
COMPONENT_DECLARATIONS(d)
COMPONENT_DECLARATIONS(e)

#endif // TEST_COMPONENTS_H_INCLUDED
