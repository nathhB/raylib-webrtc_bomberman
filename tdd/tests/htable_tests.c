#include <stdio.h>
#include <tdd.h>
#include <tdd_utests.h>

//TODO: write more tests

typedef struct
{
    int v;
} bogus_item_t;

static void clear_bogus_item(void **item)
{
    free(*item);
    *item = NULL;
}

void htable_create_tests(CuTest* tc)
{
    htable_t* htable = htable_create();

    CuAssertPtrNotNull(tc, htable->internal_array);
    CuAssertIntEquals(tc, HTABLE_DEFAULT_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 0, htable->count);
    CuAssertIntEquals(tc, 0, htable->load_factor);

    for (size_t i = 0; i < htable->capacity; i++)
    {
        CuAssertPtrEquals(tc, NULL, htable->internal_array[i]);
    }

    htable_destroy(&htable, true, clear_bogus_item, false);
}

void htable_add_and_get_tests(CuTest* tc)
{
    htable_t* htable = htable_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item5 = malloc(sizeof(bogus_item_t));

    htable_add(htable, "FIRST", item1);
    htable_add(htable, "SECOND", item2);
    htable_add(htable, "THIRD", item3);

    CuAssertPtrEquals(tc, item1, htable_get(htable, "FIRST"));
    CuAssertPtrEquals(tc, item2, htable_get(htable, "SECOND"));
    CuAssertPtrEquals(tc, item3, htable_get(htable, "THIRD"));

    CuAssertIntEquals(tc, HTABLE_DEFAULT_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 3, htable->count);
    CuAssertIntEquals(tc, 3 / HTABLE_DEFAULT_INITIAL_CAPACITY, htable->load_factor);

    htable_add(htable, "SECOND", item4);

    CuAssertPtrEquals(tc, item1, htable_get(htable, "FIRST"));
    CuAssertPtrEquals(tc, item4, htable_get(htable, "SECOND"));
    CuAssertPtrEquals(tc, item3, htable_get(htable, "THIRD"));

    CuAssertIntEquals(tc, HTABLE_DEFAULT_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 3, htable->count);
    CuAssertIntEquals(tc, 3 / HTABLE_DEFAULT_INITIAL_CAPACITY, htable->load_factor);

    CuAssertPtrEquals(tc, NULL, htable_get(htable, "FOURTH"));

    htable_add(htable, "FOURTH", item5);

    CuAssertPtrEquals(tc, item5, htable_get(htable, "FOURTH"));

    CuAssertIntEquals(tc, HTABLE_DEFAULT_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 4, htable->count);
    CuAssertIntEquals(tc, 4 / HTABLE_DEFAULT_INITIAL_CAPACITY, htable->load_factor);

    //TODO: test cases where probing is needed
    
    free(item2);

    htable_destroy(&htable, true, clear_bogus_item, false);
}

static bool find_item1(void* item)
{
    return ((bogus_item_t*)item)->v == 1;
}

static bool find_item2(void* item)
{
    return ((bogus_item_t*)item)->v == 2;
}

static bool find_item3(void* item)
{
    return ((bogus_item_t*)item)->v == 3;
}

void htable_items_tests(CuTest* tc)
{
    htable_t* htable = htable_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;

    htable_add(htable, "FIRST", item1);
    htable_add(htable, "SECOND", item2);
    htable_add(htable, "THIRD", item3);

    list_t* items = htable_items(htable);

    CuAssertIntEquals(tc, 3, items->count);
    CuAssertPtrEquals(tc, item1, list_find(items, find_item1));
    CuAssertPtrEquals(tc, item2, list_find(items, find_item2));
    CuAssertPtrEquals(tc, item3, list_find(items, find_item3));

    list_destroy(&items, false, NULL);
    htable_destroy(&htable, true, clear_bogus_item, false);
}

void htable_remove_tests(CuTest* tc)
{
    htable_t* htable = htable_create();
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));

    htable_add(htable, "FIRST", item1);
    htable_add(htable, "SECOND", item2);
    htable_add(htable, "THIRD", item3);

    CuAssertPtrEquals(tc, NULL, htable_remove(htable, "TOTO"));
    CuAssertIntEquals(tc, 3, htable->count);

    CuAssertPtrEquals(tc, item1, htable_remove(htable, "FIRST"));
    CuAssertPtrEquals(tc, item3, htable_remove(htable, "THIRD"));

    CuAssertIntEquals(tc, 1, htable->count);
    CuAssertPtrEquals(tc, NULL, htable_remove(htable, "FIRST"));
    CuAssertPtrEquals(tc, NULL, htable_remove(htable, "THIRD"));
    CuAssertPtrEquals(tc, item2, htable_remove(htable, "SECOND"));
    CuAssertIntEquals(tc, 0, htable->count);

    htable_destroy(&htable, true, clear_bogus_item, false);
}

void htable_grow_tests(CuTest* tc)
{
    htable_t *htable = htable_create_with_capacity(4);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));

    htable_add(htable, "FIRST", item1);
    htable_add(htable, "SECOND", item2);

    CuAssertTrue(tc, htable->load_factor == 0.5f);

    htable_add(htable, "THIRD", item3);
    htable_add(htable, "FOURTH", item4);

    CuAssertPtrEquals(tc, item1, htable_remove(htable, "FIRST"));
    CuAssertPtrEquals(tc, item2, htable_remove(htable, "SECOND"));
    CuAssertPtrEquals(tc, item3, htable_remove(htable, "THIRD"));
    CuAssertPtrEquals(tc, item4, htable_remove(htable, "FOURTH"));
}

CuSuite* get_htable_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, htable_create_tests);
    SUITE_ADD_TEST(suite, htable_add_and_get_tests);
    SUITE_ADD_TEST(suite, htable_items_tests);
    SUITE_ADD_TEST(suite, htable_remove_tests);
    SUITE_ADD_TEST(suite, htable_grow_tests);

    return suite;
}

UNIT_TESTS(htable)