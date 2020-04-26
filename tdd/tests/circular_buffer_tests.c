#include <stdio.h>
#include <tdd.h>
#include <tdd_utests.h>

typedef struct
{
    int v;
} bogus_item_t;

void circular_buffer_create_tests(CuTest* tc)
{
    circular_buffer_t* buffer = circular_buffer_create(5, NULL);

    CuAssertIntEquals(tc, 5, buffer->capacity);
    CuAssertIntEquals(tc, 0, buffer->count);
    CuAssertIntEquals(tc, 0, buffer->index);

    circular_buffer_destroy(buffer);
}

static unsigned int destroy_count = 0;

static void clear_buffer_item(void* item)
{
    destroy_count++;
}

void circular_buffer_add_tests(CuTest* tc)
{
    circular_buffer_t* buffer = circular_buffer_create(3, clear_buffer_item);
    bogus_item_t* item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item4 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item5 = malloc(sizeof(bogus_item_t));
    bogus_item_t* item6 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;
    item4->v = 4;
    item5->v = 5;
    item6->v = 6;

    circular_buffer_add(buffer, item1);
    
    CuAssertIntEquals(tc, 1, buffer->count);
    CuAssertIntEquals(tc, 1, buffer->index);
    CuAssertIntEquals(tc, 1, ((bogus_item_t*)buffer->internal_array[0])->v);
    CuAssertIntEquals(tc, 0, destroy_count);

    circular_buffer_add(buffer, item2);
    circular_buffer_add(buffer, item3);

    CuAssertIntEquals(tc, 3, buffer->count);
    CuAssertIntEquals(tc, 0, buffer->index);
    CuAssertIntEquals(tc, 1, ((bogus_item_t*)buffer->internal_array[0])->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)buffer->internal_array[1])->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)buffer->internal_array[2])->v);
    CuAssertIntEquals(tc, 0, destroy_count);

    circular_buffer_add(buffer, item4);
    circular_buffer_add(buffer, item5);

    CuAssertIntEquals(tc, 3, buffer->count);
    CuAssertIntEquals(tc, 2, buffer->index);
    CuAssertIntEquals(tc, 4, ((bogus_item_t*)buffer->internal_array[0])->v);
    CuAssertIntEquals(tc, 5, ((bogus_item_t*)buffer->internal_array[1])->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)buffer->internal_array[2])->v);
    CuAssertIntEquals(tc, 2, destroy_count);

    destroy_count = 0;

    circular_buffer_add(buffer, item6);

    CuAssertIntEquals(tc, 3, buffer->count);
    CuAssertIntEquals(tc, 0, buffer->index);
    CuAssertIntEquals(tc, 4, ((bogus_item_t*)buffer->internal_array[0])->v);
    CuAssertIntEquals(tc, 5, ((bogus_item_t*)buffer->internal_array[1])->v);
    CuAssertIntEquals(tc, 6, ((bogus_item_t*)buffer->internal_array[2])->v);
    CuAssertIntEquals(tc, 1, destroy_count);

    circular_buffer_destroy(buffer);
}

void circular_buffer_get_tests(CuTest* tc)
{
    circular_buffer_t *buffer = circular_buffer_create(3, clear_buffer_item);
    bogus_item_t *item1 = malloc(sizeof(bogus_item_t));
    bogus_item_t *item2 = malloc(sizeof(bogus_item_t));
    bogus_item_t *item3 = malloc(sizeof(bogus_item_t));
    bogus_item_t *item4 = malloc(sizeof(bogus_item_t));
    bogus_item_t *item5 = malloc(sizeof(bogus_item_t));
    bogus_item_t *item6 = malloc(sizeof(bogus_item_t));

    item1->v = 1;
    item2->v = 2;
    item3->v = 3;
    item4->v = 4;
    item5->v = 5;
    item6->v = 6;

    circular_buffer_add(buffer, item1);
    circular_buffer_add(buffer, item2);
    circular_buffer_add(buffer, item3);

    CuAssertIntEquals(tc, 1, ((bogus_item_t*)circular_buffer_get(buffer, 0))->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)circular_buffer_get(buffer, 1))->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)circular_buffer_get(buffer, 2))->v);
    CuAssertIntEquals(tc, 1, ((bogus_item_t *)circular_buffer_get(buffer, 3))->v);
    CuAssertIntEquals(tc, 2, ((bogus_item_t*)circular_buffer_get(buffer, 4))->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)circular_buffer_get(buffer, 5))->v);

    circular_buffer_add(buffer, item4);
    circular_buffer_add(buffer, item5);

    CuAssertIntEquals(tc, 4, ((bogus_item_t *)circular_buffer_get(buffer, 0))->v);
    CuAssertIntEquals(tc, 5, ((bogus_item_t*)circular_buffer_get(buffer, 1))->v);
    CuAssertIntEquals(tc, 3, ((bogus_item_t*)circular_buffer_get(buffer, 2))->v);

    circular_buffer_add(buffer, item6);

    CuAssertIntEquals(tc, 6, ((bogus_item_t*)circular_buffer_get(buffer, 2))->v);

    circular_buffer_destroy(buffer);
}

CuSuite *get_htable_suite()
{
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, circular_buffer_create_tests);
    SUITE_ADD_TEST(suite, circular_buffer_add_tests);
    SUITE_ADD_TEST(suite, circular_buffer_get_tests);

    return suite;
}

UNIT_TESTS(htable)