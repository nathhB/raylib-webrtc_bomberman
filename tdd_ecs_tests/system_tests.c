#include <tdd_ecs.h>
#include "CuTest.h"
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

static void bogus_entity_system_process(system_t* system, world_t* world, float delta, entity_t* entity)
{
    bag_append(processed_entitites, &entity->id);
}

void world_systems_tests(CuTest* tc)
{
    world_t* world = world_create();
    system_t* system1 = world_register_system(world, 0, bogus_system_process);
    system_t* system2 = world_register_system(world, 1, bogus2_system_process);

    CuAssertPtrNotNull(tc, system1);
    CuAssertPtrNotNull(tc, system2);

    world_process_systems(world, 0);
    world_process_systems(world, 0);
    world_process_systems(world, 0);

    CuAssertIntEquals(tc, 3, bogus_system_exec_count);
    CuAssertIntEquals(tc, 3, bogus2_system_exec_count);

    system1->is_enabled = false;

    world_process_systems(world, 0);
    world_process_systems(world, 0);
    world_process_systems(world, 0);

    CuAssertIntEquals(tc, 3, bogus_system_exec_count);
    CuAssertIntEquals(tc, 6, bogus2_system_exec_count);

    world_destroy(world);
}

void world_entity_systems_tests(CuTest* tc)
{
    world_t* world = world_create();
    family_t* family = family_create();
    system_t* system = world_register_system(world, 0, bogus3_system_process);
    entity_system_t* entity_system = world_register_entity_system(world, 1, family, bogus_entity_system_process);
    entity_t* entity1 = world_create_entity(world);
    entity_t* entity2 = world_create_entity(world);

    processed_entitites = bag_create(8);

    FAM_ALL_OF(family, A_COMPONENT, B_COMPONENT);

    add_a_to_entity(entity1);
    add_b_to_entity(entity1);

    add_a_to_entity(entity2);

    CuAssertPtrNotNull(tc, system);
    CuAssertPtrNotNull(tc, entity_system);

    world_process_systems(world, 0);
    world_process_systems(world, 0);
    world_process_systems(world, 0);

    CuAssertIntEquals(tc, 3, bogus3_system_exec_count);
    CuAssertIntEquals(tc, 3, processed_entitites->count);

    for (size_t i = 0; i < processed_entitites->count; i++)
    {
        CuAssertIntEquals(tc, 0, *((int*)processed_entitites->internal_array[i]));
    }

    entity_system->base_system.is_enabled = false;

    world_process_systems(world, 0);
    world_process_systems(world, 0);
    world_process_systems(world, 0);

    CuAssertIntEquals(tc, 6, bogus3_system_exec_count);
    CuAssertIntEquals(tc, 3, processed_entitites->count);

    world_destroy(world);
}