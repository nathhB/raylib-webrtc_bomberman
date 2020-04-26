#include <stdio.h>
#include "list.h"

static list_node_t* create_list_node(void*);
static void* remove_node_from_list(list_t*, list_node_t*);

list_t* list_create()
{
    list_t* list = malloc(sizeof(list_t));

    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    return list;
}

void list_destroy(list_t** list, bool clear_items, list_item_clear_func_t clear_item_func)
{
    list_node_t* current_node = (*list)->head;

    while (current_node != NULL)
    {
        list_node_t* next_node = current_node->next;

        if (clear_items)
        {
            clear_item_func(&current_node->data);
        }

        free(current_node);

        current_node = next_node;
    }

    free(*list);
    *list = NULL;
}

void list_push_back(list_t* list, void* data)
{
    list_node_t* node = create_list_node(data);

    if (list->count == 0)
    {
        node->next = NULL;
        node->prev = NULL;

        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = NULL;
        node->prev = list->tail;

        list->tail->next = node;
        list->tail = node;
    }

    list->count++;
}

void list_push_front(list_t* list, void* data)
{
    list_node_t* node = create_list_node(data);

    node->prev = NULL;;

    if (list->count == 0)
    {
        node->next = NULL;
        node->prev = NULL;

        list->head = node;
        list->tail = node;
    }
    else
    {
        node->prev = NULL;
        node->next = list->head;
        list->head = node;
    }

    list->count++;
}

void* list_get_at(list_t* list, int index)
{
    list_node_t* current_node = list->head;

    for (size_t i = 0; current_node != NULL && i < index; i++)
    {
        current_node = current_node->next;
    }

    return (current_node != NULL) ? current_node->data : NULL;
}

void* list_find(list_t* list, list_item_predicate_t predicate)
{
    list_node_t* current_node = list->head;

    while (current_node != NULL)
    {
        if (predicate(current_node->data))
        {
            return current_node->data;
        }

        current_node = current_node->next;
    }

    return NULL;
}

bool list_one(list_t* list, list_item_predicate_t predicate)
{
    return list_find(list, predicate) != NULL;
}

void* list_remove(list_t* list, void* data)
{
    list_node_t* current_node = list->head;

    for (size_t i = 0; current_node != NULL && current_node->data != data; i++)
        current_node = current_node->next;

    if (current_node != NULL)
    {
        return remove_node_from_list(list, current_node);
    }

    return NULL;
}

void* list_remove_at(list_t* list, int index)
{
    list_node_t* current_node = list->head;

    for (size_t i = 0; current_node != NULL && i < index; i++)
        current_node = current_node->next;

    if (current_node != NULL)
    {
        return remove_node_from_list(list, current_node);
    }

    return NULL;
}

void* list_find_and_remove(list_t* list, list_item_predicate_t predicate)
{
    list_node_t* current_node = list->head;

    for (size_t i = 0; current_node != NULL && !predicate(current_node->data); i++)
        current_node = current_node->next;

    if (current_node != NULL)
    {
        return remove_node_from_list(list, current_node);
    }

    return NULL;
}

void list_remove_if(list_t* list, list_item_predicate_t predicate, list_item_clear_func_t clear_func)
{
    list_node_t* current_node = list->head;

    while (current_node != NULL)
    {
        list_node_t* next = current_node->next;

        if (predicate(current_node->data))
        {
            void* rm_data = remove_node_from_list(list, current_node);

            clear_func(&rm_data);
        }

        current_node = next;
    }
}

bool list_exists(list_t* list, void* data)
{
    list_node_t *current_node = list->head;

    for (size_t i = 0; current_node != NULL && current_node->data != data; i++)
        current_node = current_node->next;

    return current_node != NULL;
}

void list_iter(list_t* list, list_iter_func_t iter_func)
{
    list_node_t* current_node = list->head;

    while (current_node != NULL)
    {
        iter_func(current_node->data);
        
        current_node = current_node->next;
    }
}

static list_node_t* create_list_node(void* data)
{
    list_node_t* node = malloc(sizeof(list_node_t));

    node->data = data;

    return node;
}

static void* remove_node_from_list(list_t* list, list_node_t* node)
{
    if (node == list->head)
    {
        list_node_t* new_head = node->next;

        if (new_head != NULL)
            new_head->prev = NULL;
        else
            list->tail = NULL;

        list->head = new_head;

        void* data = node->data;

        free(node);
        list->count--;

        return data;
    }

    if (node == list->tail)
    {
        list_node_t* new_tail = node->prev;

        new_tail->next = NULL;
        list->tail = new_tail;

        void* data = node->data;

        free(node);
        list->count--;

        return data;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    void* data = node->data;

    free(node);
    list->count--;

    return data;
}
