#include <bag.h>
#include "CuTest.h"

typedef struct
{
    int v;
} bogus_item_t;

static void clear_bogus_item(void *item)
{
    free(item);
}

void bag_test_initial_state(CuTest* tc)
{
    bag_t* bag = bag_create(5);

    CuAssertIntEquals(tc, 0, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);
    CuAssertIntEquals(tc, 0, bag->cursor);

    for (size_t i = 0; i < bag->capacity; i++)
    {
        CuAssertPtrEquals(tc, NULL, bag->internal_array[i]);
    }
}

void bag_append_tests(CuTest* tc)
{
    bag_t* bag = bag_create(5);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    CuAssertIntEquals(tc, 0, bag_append(bag, item1));

    CuAssertIntEquals(tc, 1, bag->count);
    CuAssertIntEquals(tc, 1, ((bogus_item_t*)bag->internal_array[0])->v);

    CuAssertIntEquals(tc, 0, bag_append(bag, item2));
    CuAssertIntEquals(tc, 0, bag_append(bag, item3));

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)bag->internal_array[1])->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)bag->internal_array[2])->v);

    bag_destroy(bag, clear_bogus_item);
}

void bag_append_beyond_capacity_tests(CuTest* tc)
{
    bag_t* bag = bag_create(2);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    CuAssertIntEquals(tc, 0, bag_append(bag, item1));
    CuAssertIntEquals(tc, 0, bag_append(bag, item2));

    CuAssertIntEquals(tc, 2, bag->count);
    CuAssertIntEquals(tc, 2, bag->capacity);

    CuAssertIntEquals(tc, 1, ((bogus_item_t*)bag->internal_array[0])->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)bag->internal_array[1])->v);

    CuAssertIntEquals(tc, 0, bag_append(bag, item3));

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 4, bag->capacity);

    CuAssertIntEquals(tc, 1, ((bogus_item_t*)bag->internal_array[0])->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)bag->internal_array[1])->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)bag->internal_array[2])->v);

    bag_destroy(bag, clear_bogus_item);
}

void bag_clear_tests(CuTest* tc)
{
    bag_t* bag = bag_create(5);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));

    bag_append(bag, item1);
    bag_append(bag, item2);

    bag_clear(bag, clear_bogus_item);

    CuAssertIntEquals(tc, 0, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);

    CuAssertPtrEquals(tc, NULL, bag->internal_array[0]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[1]);

    bag_destroy(bag, clear_bogus_item);
}

void bag_remove_at_tests(CuTest* tc)
{
    bag_t* bag = bag_create(5);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item5 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item6 = malloc(sizeof(bogus_item_t));

    bag_append(bag, item1);
    bag_append(bag, item2);
    bag_append(bag, item3);
    bag_append(bag, item4);
    bag_append(bag, item5);

    bag_remove_at(bag, 1, clear_bogus_item);
    bag_remove_at(bag, 3, clear_bogus_item);

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);
    CuAssertIntEquals(tc, 5, bag->cursor);

    CuAssertPtrEquals(tc, item1, bag->internal_array[0]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[1]);
    CuAssertPtrEquals(tc, item3, bag->internal_array[2]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[3]);
    CuAssertPtrEquals(tc, item5, bag->internal_array[4]);

    bag_remove_at(bag, 4, clear_bogus_item);

    CuAssertIntEquals(tc, 2, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);
    CuAssertIntEquals(tc, 4, bag->cursor);

    CuAssertPtrEquals(tc, NULL, bag->internal_array[4]);

    bag_append(bag, item6);

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);
    CuAssertIntEquals(tc, 5, bag->cursor);

    CuAssertPtrEquals(tc, item6, bag->internal_array[4]);

    bag_destroy(bag, clear_bogus_item);
}

void bag_set_at_tests(CuTest* tc)
{
    bag_t* bag = bag_create(5);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item5 = malloc(sizeof(bogus_item_t));

    bag_set_at(bag, 1, item1);

    CuAssertPtrEquals(tc, NULL, bag->internal_array[0]);
    CuAssertPtrEquals(tc, item1, bag->internal_array[1]);
    CuAssertIntEquals(tc, 1, bag->count);

    free(item1);

    bag_set_at(bag, 1, item2);

    CuAssertPtrEquals(tc, NULL, bag->internal_array[0]);
    CuAssertPtrEquals(tc, item2, bag->internal_array[1]);

    CuAssertIntEquals(tc, 1, bag->count);
    CuAssertIntEquals(tc, 2, bag->cursor);

    bag_set_at(bag, 0, item3);

    CuAssertPtrEquals(tc, item3, bag->internal_array[0]);
    CuAssertPtrEquals(tc, item2, bag->internal_array[1]);

    CuAssertIntEquals(tc, 2, bag->count);
    CuAssertIntEquals(tc, 2, bag->cursor);

    bag_set_at(bag, 8, item4);

    CuAssertPtrEquals(tc, item3, bag->internal_array[0]);
    CuAssertPtrEquals(tc, item2, bag->internal_array[1]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[2]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[3]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[4]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[5]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[6]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[7]);
    CuAssertPtrEquals(tc, item4, bag->internal_array[8]);

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 9, bag->cursor);
    CuAssertIntEquals(tc, 8 + (5 / 2), bag->capacity);

    bag_append(bag, item5);

    CuAssertPtrEquals(tc, item5, bag->internal_array[9]);

    bag_destroy(bag, clear_bogus_item);
}