#include "test_components.h"

COMPONENT_DEFINITIONS(a, A_COMPONENT)
COMPONENT_DEFINITIONS(b, B_COMPONENT)
COMPONENT_DEFINITIONS(c, C_COMPONENT)
COMPONENT_DEFINITIONS(d, D_COMPONENT)
COMPONENT_DEFINITIONS(e, E_COMPONENT)

void a_initialize(a_t* a, va_list args)
{
}

void a_destroy(void* a)
{
    free(a);
}

void b_initialize(b_t* b, va_list args)
{
}

void b_destroy(void* b)
{
    free(b);
}

void c_initialize(c_t* c, va_list args)
{
}

void c_destroy(void* c)
{
    free(c);
}

void d_initialize(d_t* d, va_list args)
{
}

void d_destroy(void* d)
{
    free(d);
}

void e_initialize(e_t* e, va_list args)
{
}

void e_destroy(void* e)
{
    free(e);
}
