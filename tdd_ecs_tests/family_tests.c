#include <tdd_ecs.h>
#include "CuTest.h"
#include "test_components.h"

void family_all_tests(CuTest* tc)
{
    family_t* family = family_create();

    FAM_ALL_OF(family, 0, 4, 10, 16, 24);

    CuAssertTrue(tc, B_IS_SET(family->all_mask, 0));
    CuAssertTrue(tc, B_IS_SET(family->all_mask, 4));

    CuAssertTrue(tc, B_IS_SET(family->all_mask, 10));
    CuAssertTrue(tc, B_IS_SET(family->all_mask, 16));
    CuAssertTrue(tc, B_IS_SET(family->all_mask, 24));

    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, 1));
    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, 12));
    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, 30));

    family_destroy(family);
}

void family_any_tests(CuTest* tc)
{
    family_t* family = family_create();

    FAM_ANY_OF(family, 0, 4, 10, 16, 24);

    CuAssertTrue(tc, B_IS_SET(family->any_mask, 0));
    CuAssertTrue(tc, B_IS_SET(family->any_mask, 4));

    CuAssertTrue(tc, B_IS_SET(family->any_mask, 10));
    CuAssertTrue(tc, B_IS_SET(family->any_mask, 16));
    CuAssertTrue(tc, B_IS_SET(family->any_mask, 24));

    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, 1));
    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, 12));
    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, 30));

    family_destroy(family);
}

void family_not_tests(CuTest* tc)
{
    family_t* family = family_create();

    FAM_NONE_OF(family, 0, 4, 10, 16, 24);

    CuAssertTrue(tc, B_IS_SET(family->not_mask, 0));
    CuAssertTrue(tc, B_IS_SET(family->not_mask, 4));

    CuAssertTrue(tc, B_IS_SET(family->not_mask, 10));
    CuAssertTrue(tc, B_IS_SET(family->not_mask, 16));
    CuAssertTrue(tc, B_IS_SET(family->not_mask, 24));

    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, 1));
    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, 12));
    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, 30));

    family_destroy(family);
}

void family_entity_is_part_of_tests(CuTest* tc)
{
    world_t* world = world_create();

    family_t* family = family_create();
    entity_t* entity = world_create_entity(world);

    FAM_ALL_OF(family, A_COMPONENT, C_COMPONENT);

    CuAssertTrue(tc, !family_entity_is_part_of(family, entity));

    add_a_to_entity(entity);

    CuAssertTrue(tc, !family_entity_is_part_of(family, entity));

    add_c_to_entity(entity);

    CuAssertTrue(tc, B_IS_SET(family->all_mask, A_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, B_COMPONENT));
    CuAssertTrue(tc, B_IS_SET(family->all_mask, C_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, D_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->all_mask, E_COMPONENT));
    CuAssertTrue(tc, family_entity_is_part_of(family, entity));

    FAM_NONE_OF(family, B_COMPONENT);

    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, A_COMPONENT));
    CuAssertTrue(tc, B_IS_SET(family->not_mask, B_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, C_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, D_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->not_mask, E_COMPONENT));
    CuAssertTrue(tc, family_entity_is_part_of(family, entity));

    add_b_to_entity(entity);

    CuAssertTrue(tc, !family_entity_is_part_of(family, entity));

    remove_b_from_entity(entity);

    CuAssertTrue(tc, family_entity_is_part_of(family, entity));

    FAM_ANY_OF(family, D_COMPONENT, E_COMPONENT);

    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, A_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, B_COMPONENT));
    CuAssertTrue(tc, B_IS_UNSET(family->any_mask, C_COMPONENT));
    CuAssertTrue(tc, B_IS_SET(family->any_mask, D_COMPONENT));
    CuAssertTrue(tc, B_IS_SET(family->any_mask, E_COMPONENT));
    CuAssertTrue(tc, !family_entity_is_part_of(family, entity));

    add_d_to_entity(entity);

    CuAssertTrue(tc, family_entity_is_part_of(family, entity));

    add_e_to_entity(entity);

    CuAssertTrue(tc, family_entity_is_part_of(family, entity));

    remove_a_from_entity(entity);

    CuAssertTrue(tc, !family_entity_is_part_of(family, entity));

    family_destroy(family);
    world_destroy(world);
}