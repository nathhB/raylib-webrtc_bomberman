#include <stdio.h>
#include <tdd.h>
#include <tdd_utests.h>

typedef struct
{
    int v;
} bogus_item_t;

static void clear_bogus_item(void **item)
{
    free(*item);
    *item = NULL;
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

    void* removed_item2 = bag_remove_at(bag, 1);
    void* removed_item4 = bag_remove_at(bag, 3);

    CuAssertPtrEquals(tc, item2, removed_item2);
    CuAssertPtrEquals(tc, item4, removed_item4);

    CuAssertIntEquals(tc, 3, bag->count);
    CuAssertIntEquals(tc, 5, bag->capacity);
    CuAssertIntEquals(tc, 5, bag->cursor);

    CuAssertPtrEquals(tc, item1, bag->internal_array[0]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[1]);
    CuAssertPtrEquals(tc, item3, bag->internal_array[2]);
    CuAssertPtrEquals(tc, NULL, bag->internal_array[3]);
    CuAssertPtrEquals(tc, item5, bag->internal_array[4]);

    void* removed_item5 = bag_remove_at(bag, 4);

    CuAssertPtrEquals(tc, item5, removed_item5);

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

CuSuite* get_bag_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, bag_test_initial_state);
    SUITE_ADD_TEST(suite, bag_append_tests);
    SUITE_ADD_TEST(suite, bag_append_beyond_capacity_tests);
    SUITE_ADD_TEST(suite, bag_clear_tests);
    SUITE_ADD_TEST(suite, bag_remove_at_tests);

    return suite;
}

UNIT_TESTS(bag)