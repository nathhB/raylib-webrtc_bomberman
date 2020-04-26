#ifndef BAG_H_INCLUDED
#define BAG_H_INCLUDED

#include <stdlib.h>

typedef struct
{
    size_t  count;
    size_t  capacity;
    size_t  cursor;
    void**  internal_array;
} bag_t;

typedef void (*bag_item_clear_func_t)(void**);

bag_t*  bag_create(int);
void    bag_destroy(bag_t* bag, bag_item_clear_func_t);
int     bag_append(bag_t*, void*);
void    bag_set_at(bag_t*, int, void*);
void*   bag_get_at(bag_t*, int);
void    bag_clear(bag_t* bag, bag_item_clear_func_t);
void*   bag_remove_at(bag_t*, int);

#endif // BAG_H_INCLUDED
