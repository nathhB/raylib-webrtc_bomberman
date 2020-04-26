#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

typedef struct list_node
{
    void*               data;
    struct list_node*   next;
    struct list_node*   prev;
} list_node_t;

typedef struct
{
    list_node_t*    head;
    list_node_t*    tail;
    unsigned int    count;
} list_t;

typedef void    (*list_item_clear_func_t)(void**);
typedef bool    (*list_item_predicate_t)(void*);
typedef void    (*list_iter_func_t)(void*);

list_t* list_create();
void    list_destroy(list_t**, bool, list_item_clear_func_t);
void    list_push_back(list_t*, void*);
void    list_push_front(list_t*, void*);
void*   list_get_at(list_t*, int);
void*   list_find(list_t*, list_item_predicate_t);
bool    list_one(list_t*, list_item_predicate_t);
void*   list_remove(list_t*, void*);
void*   list_remove_at(list_t*, int);
void*   list_find_and_remove(list_t*, list_item_predicate_t);
void    list_remove_if(list_t*, list_item_predicate_t, list_item_clear_func_t);
bool    list_exists(list_t*, void*);
void    list_iter(list_t*, list_iter_func_t);

#endif // LIST_H_INCLUDED
