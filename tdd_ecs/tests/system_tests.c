#include <tdd_ecs.h>
#include <tdd_utests.h>
#include "test_components.h"

static unsigned int bogus_system_exec_count = 0;
static unsigned int bogus2_system_exec_count = 0;
static unsigned int bogus3_system_exec_count = 0;
static bag_t*       processed_entitites = NULL;

static void bogus_system_process(system_t* system, world_t* world, float delta)
{
    bogus_system_exec_count++;
}

static void bogus2_system_process(system_t* system, world_t* world, float delta)
{
    bogus2_system_exec_count++;
}

static void bogus3_system_process(system_t* system, world_t* world, float delta)
{
    bogus3_system_exec_count++;
}

static void bogus_entity_system_process(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    bag_append(processed_entitites, &entity->id);
}

static void bogus2_entity_system_process(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
}

void world_systems_tests(CuTest* tc)
{
    world_t* world = world_create();
    system_t* system1 = world_register_system(world, bogus_system_process);
    system_t* system2 = world_register_system(world, bogus2_system_process);

    CuAssertPtrNotNull(tc, system1);
    CuAssertPtrNotNull(tc, system2);

    world_update(world, 0);
    world_update(world, 0);
    world_update(world, 0);

    CuAssertIntEquals(tc, 3, bogus_system_exec_count);
    CuAssertIntEquals(tc, 3, bogus2_system_exec_count);

    system1->is_enabled = false;

    world_update(world, 0);
    world_update(world, 0);
    world_update(world, 0);

    CuAssertIntEquals(tc, 3, bogus_system_exec_count);
    CuAssertIntEquals(tc, 6, bogus2_system_exec_count);

    world_destroy(world);
}

static int begin_count = 0;
static int end_count = 0;

static void bogus_begin(entity_system_t* entity_system, world_t* world, list_t* entities)
{
    begin_count++;
}

static void bogus_end(entity_system_t* entity_system, world_t* world, list_t* entities)
{
    end_count++;
}

void world_entity_systems_tests(CuTest* tc)
{
    world_t* world = world_create();
    family_t* family = family_create();
    system_t* system = world_register_system(world, bogus3_system_process);
    entity_system_t* entity_system = world_register_entity_system(world, family, bogus_entity_system_process);
    entity_t* entity1 = world_create_entity(world);
    entity_t* entity2 = world_create_entity(world);

    processed_entitites = bag_create(8);

    FAM_ALL_OF(family, get_a_id(), get_b_id());

    add_a_to_entity(entity1);
    add_b_to_entity(entity1);

    add_a_to_entity(entity2);

    CuAssertPtrNotNull(tc, system);
    CuAssertPtrNotNull(tc, entity_system);

    world_update(world, 0);
    world_update(world, 0);
    world_update(world, 0);

    CuAssertIntEquals(tc, 3, bogus3_system_exec_count);
    CuAssertIntEquals(tc, 3, processed_entitites->count);

    for (size_t i = 0; i < processed_entitites->count; i++)
    {
        CuAssertIntEquals(tc, 0, *((int*)processed_entitites->internal_array[i]));
    }

    entity_system->base_system.is_enabled = false;

    world_update(world, 0);
    world_update(world, 0);
    world_update(world, 0);

    CuAssertIntEquals(tc, 6, bogus3_system_exec_count);
    CuAssertIntEquals(tc, 3, processed_entitites->count);

    entity_system->base_system.is_enabled = true;

    // attach a begin hook
    entity_system->begin_func = bogus_begin;
    entity_system->end_func = bogus_end;

    world_update(world, 0);
    world_update(world, 0);

    CuAssertIntEquals(tc, 2, begin_count);
    CuAssertIntEquals(tc, 2, end_count);

    world_destroy(world);
}

void world_get_system_tests(CuTest* tc)
{
    world_t* world = world_create();
    family_t* family = family_create();
    system_t* system = world_register_system(world, bogus_system_process);

    CuAssertPtrEquals(tc, NULL, world_get_system(world, bogus2_system_process));
    CuAssertPtrEquals(tc, system, world_get_system(world, bogus_system_process));

    world_destroy(world);
}

void world_get_entity_system_tests(CuTest* tc)
{
    world_t* world = world_create();
    family_t* family = family_create();
    entity_system_t* entity_system = world_register_entity_system(world, family, bogus2_entity_system_process);

    CuAssertPtrEquals(tc, NULL, world_get_entity_system(world, bogus_entity_system_process));
    CuAssertPtrEquals(tc, entity_system, world_get_entity_system(world, bogus2_entity_system_process));

    world_destroy(world);
}

static long bogus_entity_destroyed_id1 = -1;
static long bogus_entity_destroyed_id2 = -1;

static void bogus_entity_destroyed1(entity_system_t* entity_system, world_t* world, entity_t* entity)
{
    bogus_entity_destroyed_id1 = entity->id;
}

static void bogus_entity_destroyed2(entity_system_t* entity_system, world_t* world, entity_t* entity)
{
    bogus_entity_destroyed_id2 = entity->id;
}

void entity_system_entity_destroyed_callback_tests(CuTest* tc)
{
    world_t* world = world_create();
    family_t* family1 = family_create();
    family_t* family2 = family_create();
    entity_system_t* entity_system1 = world_register_entity_system(world, family1, bogus_entity_system_process);
    entity_system_t* entity_system2 = world_register_entity_system(world, family2, bogus2_entity_system_process);

    entity_system1->entity_destroyed_func = bogus_entity_destroyed1;
    entity_system2->entity_destroyed_func = bogus_entity_destroyed2;

    FAM_ALL_OF(family1, get_a_id(), get_b_id());
    FAM_ALL_OF(family2, get_c_id(), get_d_id());

    entity_t* e1 = world_create_entity(world);
    entity_t* e2 = world_create_entity(world);

    add_a_to_entity(e1);
    add_b_to_entity(e1);

    add_c_to_entity(e2);
    add_d_to_entity(e2);

    world_destroy_entity(world, e1);

    CuAssertIntEquals(tc, 0, bogus_entity_destroyed_id1);
    CuAssertIntEquals(tc, -1, bogus_entity_destroyed_id2);

    world_destroy_entity(world, e2);

    CuAssertIntEquals(tc, 0, bogus_entity_destroyed_id1);
    CuAssertIntEquals(tc, 1, bogus_entity_destroyed_id2);

    world_destroy(world);
}

CuSuite *get_system_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, world_systems_tests);
    SUITE_ADD_TEST(suite, world_entity_systems_tests);
    SUITE_ADD_TEST(suite, world_get_system_tests);
    SUITE_ADD_TEST(suite, world_get_entity_system_tests);
    SUITE_ADD_TEST(suite, entity_system_entity_destroyed_callback_tests);

    return suite;
}

UNIT_TESTS(system)