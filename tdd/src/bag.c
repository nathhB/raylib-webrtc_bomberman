#include "bag.h"

static int grow(bag_t*, size_t);

bag_t* bag_create(int initial_capacity)
{
    bag_t* bag = malloc(sizeof(bag_t));

    bag->count = 0;
    bag->cursor = 0;
    bag->internal_array = NULL;

    grow(bag, initial_capacity);

    if (bag->internal_array == NULL)
    {
        return NULL;
    }

    return bag;
}

void bag_destroy(bag_t* bag, bag_item_clear_func_t clear_func)
{
    bag_clear(bag, clear_func);
    free(bag->internal_array);
    free(bag);
}

int bag_append(bag_t* bag, void* item)
{
    if (bag->count >= bag->capacity)
    {
        if (grow(bag, bag->capacity * 2) < 0)
        {
            return -1;
        }
    }

    bag->internal_array[bag->cursor++] = item;
    bag->count++;

    return 0;
}

void bag_set_at(bag_t* bag, int index, void* item)
{
    if (index > bag->capacity)
    {
        grow(bag, index + bag->capacity / 2);
    }

    if (index > bag->cursor)
    {
        bag->cursor = index + 1;
    }

    void* current_item = bag->internal_array[index];

    bag->internal_array[index] = item;

    if (current_item == NULL)
    {
        bag->count++;
    }
}

void* bag_get_at(bag_t* bag, int index)
{
    return bag->internal_array[index];
}

void* bag_remove_at(bag_t* bag, int index)
{
    void* item = bag->internal_array[index];

    if (item != NULL)
    {
        bag->internal_array[index] = NULL;
        bag->count--;

        if (index == bag->cursor - 1)
        {
            bag->cursor--;
        }
    }

    return item;
}

void bag_clear(bag_t* bag, bag_item_clear_func_t clear_func)
{
    for (size_t i = 0; i < bag->capacity; i++)
    {
        void* item = bag_remove_at(bag, i);

        if (item != NULL)
        {
            clear_func(&item);
        }
    }
}

static int grow(bag_t* bag, size_t new_capacity)
{
    bag->capacity = new_capacity;
    bag->internal_array = realloc(bag->internal_array, sizeof(void*) * new_capacity);

    if (bag->internal_array == NULL)
    {
        return -1;
    }

    for (size_t i = bag->cursor; i < bag->capacity; i++)
    {
        bag->internal_array[i] = NULL;
    }

    return 0;
}
