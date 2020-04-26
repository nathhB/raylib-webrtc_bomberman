#ifndef HTABLE_H_INCLUDED
#define HTABLE_H_INCLUDED

#define HTABLE_DEFAULT_INITIAL_CAPACITY 32
#define HTABLE_LOAD_FACTOR_THRESHOLD 0.75

#include <stdlib.h>
#include <stdbool.h>

typedef struct htable_entry
{
    const char*     key;
    void*           item;
    unsigned int    slot;
} htable_entry_t;

typedef struct htable
{
    htable_entry_t**    internal_array;
    size_t              capacity;
    size_t              count;
    float               load_factor;
} htable_t;

typedef void (*htable_item_clear_func_t)(void**);

htable_t*   htable_create();
htable_t*   htable_create_with_capacity(size_t);
void        htable_destroy(htable_t**, bool, htable_item_clear_func_t, bool);
void        htable_add(htable_t*, const char*, void*);
void*       htable_get(htable_t*, const char*);
void*       htable_remove(htable_t*, const char*);
list_t*     htable_items(htable_t*);
void        htable_print(htable_t*);

#endif // HTABLE_H_INCLUDED