#include <tdd_ecs.h>
#include <tdd_utests.h>

#define EVENT_1 1
#define EVENT_2 2
#define EVENT_3 3

static list_t* handled_events_data = NULL;

static void handle_event(world_t* world, void* data)
{
    list_push_back(handled_events_data, data);
}

void world_push_event_tests(CuTest* tc)
{
    world_t* world = world_create();
    char* ev_data1 = "foo";
    char* ev_data2 = "bar";
    char* ev_data3 = "plop";
    char* ev_data4 = "plap";

    world_push_event(world, EVENT_1, ev_data1);
    world_push_event(world, EVENT_1, ev_data2);
    world_push_event(world, EVENT_2, ev_data3);
    world_push_event(world, EVENT_3, ev_data4);

    list_t* ev_bus1 = (list_t*)bag_get_at(world->event_bus, EVENT_1);
    list_t* ev_bus2 = (list_t*)bag_get_at(world->event_bus, EVENT_2);
    list_t* ev_bus3 = (list_t*)bag_get_at(world->event_bus, EVENT_3);

    CuAssertIntEquals(tc, 2, ev_bus1->count);
    CuAssertIntEquals(tc, 1, ev_bus2->count);
    CuAssertIntEquals(tc, 1, ev_bus3->count);

    event_t* ev1 = list_get_at(ev_bus1, 0);
    event_t* ev2 = list_get_at(ev_bus1, 1);
    event_t* ev3 = list_get_at(ev_bus2, 0);
    event_t* ev4 = list_get_at(ev_bus3, 0);

    CuAssertIntEquals(tc, EVENT_1, ev1->type);
    CuAssertIntEquals(tc, EVENT_1, ev2->type);
    CuAssertIntEquals(tc, EVENT_2, ev3->type);
    CuAssertIntEquals(tc, EVENT_3, ev4->type);

    CuAssertPtrEquals(tc, ev_data1, ev1->data);
    CuAssertPtrEquals(tc, ev_data2, ev2->data);
    CuAssertPtrEquals(tc, ev_data3, ev3->data);
    CuAssertPtrEquals(tc, ev_data4, ev4->data);

    world_destroy(world);
}

void world_consume_events_tests(CuTest* tc)
{
    world_t* world = world_create();
    handled_events_data = list_create();
    void* ev_data1 = malloc(8);
    void* ev_data2 = malloc(8);
    void* ev_data3 = malloc(8);
    void* ev_data4 = malloc(8);

    list_t* ev_bus1 = (list_t*)bag_get_at(world->event_bus, EVENT_1);
    list_t* ev_bus2 = (list_t*)bag_get_at(world->event_bus, EVENT_2);
    list_t* ev_bus3 = (list_t*)bag_get_at(world->event_bus, EVENT_3);

    world_push_event(world, EVENT_1, ev_data1);
    world_push_event(world, EVENT_1, ev_data2);
    world_push_event(world, EVENT_2, ev_data3);
    world_push_event(world, EVENT_3, ev_data4);

    world_consume_events(world, EVENT_1, handle_event);

    CuAssertIntEquals(tc, 2, handled_events_data->count);
    CuAssertIntEquals(tc, 0, ev_bus1->count);
    CuAssertIntEquals(tc, 1, ev_bus2->count);
    CuAssertIntEquals(tc, 1, ev_bus3->count);

    CuAssertPtrEquals(tc, ev_data1, list_get_at(handled_events_data, 0));
    CuAssertPtrEquals(tc, ev_data2, list_get_at(handled_events_data, 1));

    world_consume_events(world, EVENT_2, handle_event);

    CuAssertIntEquals(tc, 3, handled_events_data->count);
    CuAssertIntEquals(tc, 0, ev_bus1->count);
    CuAssertIntEquals(tc, 0, ev_bus2->count);
    CuAssertIntEquals(tc, 1, ev_bus3->count);

    CuAssertPtrEquals(tc, ev_data3, list_get_at(handled_events_data, 2));

    world_consume_events(world, EVENT_3, handle_event);
    CuAssertIntEquals(tc, 0, ev_bus1->count);
    CuAssertIntEquals(tc, 0, ev_bus2->count);
    CuAssertIntEquals(tc, 0, ev_bus3->count);

    CuAssertIntEquals(tc, 4, handled_events_data->count);

    CuAssertPtrEquals(tc, ev_data4, list_get_at(handled_events_data, 3));

    list_destroy(&handled_events_data, false, NULL);
    world_destroy(world);
}

CuSuite* get_world_events_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, world_push_event_tests);
    SUITE_ADD_TEST(suite, world_consume_events_tests);

    return suite;
}

UNIT_TESTS(world_events)