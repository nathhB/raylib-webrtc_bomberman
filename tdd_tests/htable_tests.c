#include <htable.h>
#include "CuTest.h"

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
    CuAssertIntEquals(tc, HTABLE_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 0, htable->count);
    CuAssertIntEquals(tc, 0, htable->load_factor);

    for (size_t i = 0; i < htable->capacity; i++)
    {
        CuAssertPtrEquals(tc, NULL, htable->internal_array[i]);
    }

    htable_destroy(&htable, true, clear_bogus_item);
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

    CuAssertIntEquals(tc, HTABLE_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 3, htable->count);
    CuAssertIntEquals(tc, 3 / HTABLE_INITIAL_CAPACITY, htable->load_factor);

    htable_add(htable, "SECOND", item4);

    CuAssertPtrEquals(tc, item1, htable_get(htable, "FIRST"));
    CuAssertPtrEquals(tc, item4, htable_get(htable, "SECOND"));
    CuAssertPtrEquals(tc, item3, htable_get(htable, "THIRD"));

    CuAssertIntEquals(tc, HTABLE_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 3, htable->count);
    CuAssertIntEquals(tc, 3 / HTABLE_INITIAL_CAPACITY, htable->load_factor);

    CuAssertPtrEquals(tc, NULL, htable_get(htable, "FOURTH"));

    htable_add(htable, "FOURTH", item5);

    CuAssertPtrEquals(tc, item5, htable_get(htable, "FOURTH"));

    CuAssertIntEquals(tc, HTABLE_INITIAL_CAPACITY, htable->capacity);
    CuAssertIntEquals(tc, 4, htable->count);
    CuAssertIntEquals(tc, 4 / HTABLE_INITIAL_CAPACITY, htable->load_factor);

    //TODO: test cases where probing is needed
    
    free(item2);

    htable_destroy(&htable, true, clear_bogus_item);
}