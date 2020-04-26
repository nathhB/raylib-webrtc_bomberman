#include <stdbool.h>
#include <list.h>
#include "CuTest.h"

typedef struct
{
    int v;
} bogus_item_t;

static void clear_bogus_item(void **item)
{
    free(*item);
    *item = NULL;
}

void list_create_tests(CuTest* tc)
{
    list_t* list = list_create();

    CuAssertPtrEquals(tc, NULL, list->head);
    CuAssertPtrEquals(tc, NULL, list->tail);

    CuAssertIntEquals(tc, 0, list->count);

    list_destroy(&list, true, clear_bogus_item);
}

void list_push_back_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    CuAssertPtrNotNull(tc, list->head);
    CuAssertPtrNotNull(tc, list->tail);

    CuAssertPtrEquals(tc, item1, list->head->data);
    CuAssertPtrEquals(tc, item3, list->tail->data);
    CuAssertPtrEquals(tc, item2, list->head->next->data);
    CuAssertPtrEquals(tc, item3, list->head->next->next->data);

    CuAssertIntEquals(tc, 3, list->count);

    list_destroy(&list, true, clear_bogus_item);
}

void list_push_front_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_front(list, item1);
    list_push_front(list, item2);
    list_push_front(list, item3);

    CuAssertPtrNotNull(tc, list->head);
    CuAssertPtrNotNull(tc, list->tail);

    CuAssertPtrEquals(tc, item3, list->head->data);
    CuAssertPtrEquals(tc, item1, list->tail->data);
    CuAssertPtrEquals(tc, item2, list->head->next->data);
    CuAssertPtrEquals(tc, item1, list->head->next->next->data);

    CuAssertIntEquals(tc, 3, list->count);

    list_destroy(&list, true, clear_bogus_item);
}

void list_get_at_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    CuAssertPtrNotNull(tc, list_get_at(list, 0));
    CuAssertPtrNotNull(tc, list_get_at(list, 1));
    CuAssertPtrNotNull(tc, list_get_at(list, 2));

    CuAssertIntEquals(tc, 1, ((bogus_item_t*)list_get_at(list, 0))->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)list_get_at(list, 1))->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)list_get_at(list, 2))->v);

    CuAssertPtrEquals(tc, NULL, list_get_at(list, 42));

    list_destroy(&list, true, clear_bogus_item);
}

static bool bogus_find1(void *data)
{
    bogus_item_t* item = (bogus_item_t*)data;

    return item->v == 2;
}

static bool bogus_find42(void *data)
{
    bogus_item_t* item = (bogus_item_t*)data;

    return item->v == 42;
}

void list_find_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    CuAssertPtrEquals(tc, item2, list_find(list, bogus_find1));
    CuAssertPtrEquals(tc, NULL, list_find(list, bogus_find42));

    list_destroy(&list, true, clear_bogus_item);
}

void list_remove_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;
    item4->v = 4;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    void* removed = list_remove(list, item1);

    CuAssertPtrEquals(tc, item1, removed);

    CuAssertPtrEquals(tc, item2, list->head->data);
    CuAssertPtrEquals(tc, item3, list->tail->data);

    CuAssertIntEquals(tc, 2, list->count);

    list_remove(list, item3);

    CuAssertPtrEquals(tc, item2, list->head->data);
    CuAssertPtrEquals(tc, item2, list->tail->data);

    list_remove(list, item4); // not in the list

    CuAssertIntEquals(tc, 1, list->count);

    list_remove(list, item2);

    CuAssertPtrEquals(tc, NULL, list->head);
    CuAssertPtrEquals(tc, NULL, list->tail);

    CuAssertIntEquals(tc, 0, list->count);

    list_destroy(&list, true, clear_bogus_item);
}

void list_remove_at_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    list_remove_at(list, 0);

    CuAssertPtrEquals(tc, item2, list->head->data);
    CuAssertPtrEquals(tc, item3, list->tail->data);

    CuAssertIntEquals(tc, 2, list->count);

    list_remove_at(list, 1);

    CuAssertPtrEquals(tc, item2, list->head->data);
    CuAssertPtrEquals(tc, item2, list->tail->data);

    CuAssertIntEquals(tc, 1, list->count);

    list_remove_at(list, 42);
    list_remove_at(list, 0);

    CuAssertPtrEquals(tc, NULL, list->head);
    CuAssertPtrEquals(tc, NULL, list->tail);

    CuAssertIntEquals(tc, 0, list->count);

    list_destroy(&list, true, clear_bogus_item);
}

void list_destroy_tests(CuTest* tc)
{
    list_t* list = list_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    list_push_back(list, item1);
    list_push_back(list, item2);
    list_push_back(list, item3);

    list_destroy(&list, false, NULL);

    CuAssertPtrEquals(tc, NULL, list);

    CuAssertIntEquals(tc, 1, item1->v);
    CuAssertIntEquals(tc, 2, item2->v);
    CuAssertIntEquals(tc, 3, item3->v);

    list_t* list2 = list_create();

    list_push_back(list2, item1);
    list_push_back(list2, item2);
    list_push_back(list2, item3);

    list_destroy(&list2, true, clear_bogus_item);

    CuAssertPtrEquals(tc, NULL, list2);
}
