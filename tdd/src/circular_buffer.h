#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

typedef void (*destroy_circular_buffer_item_func_t)(void*);

typedef struct circular_buffer
{
    void**                              internal_array;
    unsigned int                        capacity;
    unsigned int                        index;
    unsigned int                        count;
    destroy_circular_buffer_item_func_t destroy_circular_buffer_item_func;
} circular_buffer_t;

circular_buffer_t*  circular_buffer_create(unsigned int, destroy_circular_buffer_item_func_t);
void                circular_buffer_destroy(circular_buffer_t*);
void                circular_buffer_add(circular_buffer_t*, void*);
void*               circular_buffer_get(circular_buffer_t*, unsigned int);

#endif // CIRCULAR_BUFFER_H_INCLUDED