#include <tdd_ecs.h>
#include <tdd_utests.h>
#include "test_components.h"

void entity_id_inc_tests(CuTest* tc)
{
    world_t* world = world_create();
    entity_t* e1 = world_create_entity(world);
    entity_t* e2 = world_create_entity(world);
    entity_t* e3 = world_create_entity(world);

    CuAssertIntEquals(tc, 0, e1->id);
    CuAssertIntEquals(tc, 1, e2->id);
    CuAssertIntEquals(tc, 2, e3->id);

    world_destroy(world);
}

void entity_add_component_tests(CuTest* tc)
{
    world_t* world = world_create();
    entity_t* entity = world_create_entity(world);

    CuAssertIntEquals(tc, 0, entity->components_mask);

    void* a_comp = add_a_to_entity(entity);
    void* d_comp = add_d_to_entity(entity);
    void* e_comp = add_e_to_entity(entity);

    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_a_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_b_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_c_id()));
    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_d_id()));
    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_e_id()));

    CuAssertPtrNotNull(tc, a_comp);
    CuAssertPtrNotNull(tc, d_comp);
    CuAssertPtrNotNull(tc, e_comp);

    CuAssertPtrEquals(tc, a_comp, entity->components->internal_array[get_a_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_b_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_c_id()]);
    CuAssertPtrEquals(tc, d_comp, entity->components->internal_array[get_d_id()]);
    CuAssertPtrEquals(tc, e_comp, entity->components->internal_array[get_e_id()]);

    world_destroy(world);
}

void entity_remove_component_tests(CuTest* tc)
{
    world_t* world = world_create();
    entity_t* entity = world_create_entity(world);

    CuAssertIntEquals(tc, 0, entity->components_mask);

    void* a_comp = add_a_to_entity(entity);
    void* b_comp = add_b_to_entity(entity);
    void* e_comp = add_e_to_entity(entity);

    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_a_id()));
    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_b_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_c_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_d_id()));
    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_e_id()));

    CuAssertPtrNotNull(tc, a_comp);
    CuAssertPtrNotNull(tc, b_comp);
    CuAssertPtrNotNull(tc, e_comp);

    CuAssertPtrEquals(tc, a_comp, entity->components->internal_array[get_a_id()]);
    CuAssertPtrEquals(tc, b_comp, entity->components->internal_array[get_b_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_c_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_d_id()]);
    CuAssertPtrEquals(tc, e_comp, entity->components->internal_array[get_e_id()]);

    remove_a_from_entity(entity);
    remove_e_from_entity(entity);

    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_a_id()));
    CuAssertTrue(tc, B_IS_SET(entity->components_mask, get_b_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_c_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_d_id()));
    CuAssertTrue(tc, B_IS_UNSET(entity->components_mask, get_e_id()));

    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_a_id()]);
    CuAssertPtrEquals(tc, b_comp, entity->components->internal_array[get_b_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_c_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_d_id()]);
    CuAssertPtrEquals(tc, NULL, entity->components->internal_array[get_e_id()]);

    world_destroy(world);
}

void entity_has_component_tests(CuTest* tc)
{
    world_t* world = world_create();
    entity_t* entity = world_create_entity(world);

    CuAssertIntEquals(tc, 0, entity->components_mask);

    add_a_to_entity(entity);
    add_b_to_entity(entity);
    add_e_to_entity(entity);

    CuAssertTrue(tc, entity_has_a(entity));
    CuAssertTrue(tc, entity_has_b(entity));
    CuAssertTrue(tc, !entity_has_c(entity));
    CuAssertTrue(tc, !entity_has_d(entity));
    CuAssertTrue(tc, entity_has_e(entity));

    world_destroy(world);
}

void entity_get_component_tests(CuTest* tc)
{
    world_t* world = world_create();
    entity_t* entity = world_create_entity(world);

    void* a_comp = add_a_to_entity(entity);
    void* b_comp = add_b_to_entity(entity);
    void* e_comp = add_e_to_entity(entity);

    CuAssertPtrEquals(tc, a_comp, get_a_of_entity(entity));
    CuAssertPtrEquals(tc, b_comp, get_b_of_entity(entity));
    CuAssertPtrEquals(tc, NULL, get_c_of_entity(entity));
    CuAssertPtrEquals(tc, NULL, get_d_of_entity(entity));
    CuAssertPtrEquals(tc, e_comp, get_e_of_entity(entity));

    world_destroy(world);
}

CuSuite* get_entity_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, entity_id_inc_tests);
    SUITE_ADD_TEST(suite, entity_add_component_tests);
    SUITE_ADD_TEST(suite, entity_remove_component_tests);
    SUITE_ADD_TEST(suite, entity_has_component_tests);
    SUITE_ADD_TEST(suite, entity_get_component_tests);

    return suite;
}

UNIT_TESTS(entity)