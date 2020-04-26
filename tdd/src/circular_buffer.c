#include "tdd.h"

circular_buffer_t* circular_buffer_create(unsigned int capacity, destroy_circular_buffer_item_func_t destroy_circular_buffer_item_func)
{
    circular_buffer_t* circular_buffer = malloc(sizeof(circular_buffer_t));

    circular_buffer->internal_array = malloc(sizeof(void*) * capacity);
    circular_buffer->capacity = capacity;
    circular_buffer->index = 0;
    circular_buffer->count = 0;
    circular_buffer->destroy_circular_buffer_item_func = destroy_circular_buffer_item_func;

    for (int i = 0; i < capacity; i++)
    {
        circular_buffer->internal_array[i] = NULL;
    }

    return circular_buffer;
}

void circular_buffer_destroy(circular_buffer_t* circular_buffer)
{
    for (int i = 0; i < circular_buffer->capacity; i++)
    {
        void* item = circular_buffer->internal_array[i];

        if (circular_buffer->destroy_circular_buffer_item_func)
        {
            circular_buffer->destroy_circular_buffer_item_func(item);
        }
    }
}

void circular_buffer_add(circular_buffer_t* circular_buffer, void* item)
{
    void* existing_item = circular_buffer->internal_array[circular_buffer->index];

    if (existing_item)
    {
        if (circular_buffer->destroy_circular_buffer_item_func)
        {
            circular_buffer->destroy_circular_buffer_item_func(existing_item);
        }
    }
    else
    {
        circular_buffer->count++;
    }
    
    circular_buffer->internal_array[circular_buffer->index] = item;
    circular_buffer->index = (circular_buffer->index + 1) % circular_buffer->capacity;
}

void* circular_buffer_get(circular_buffer_t* buffer, unsigned int index)
{
    return buffer->internal_array[index % buffer->capacity];
}