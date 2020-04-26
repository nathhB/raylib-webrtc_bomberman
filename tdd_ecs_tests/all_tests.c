#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"

void family_all_tests(CuTest*);
void family_any_tests(CuTest*);
void family_not_tests(CuTest*);
void family_entity_is_part_of_tests(CuTest*);

void entity_id_inc_tests(CuTest*);
void entity_add_component_tests(CuTest*);
void entity_remove_component_tests(CuTest*);
void entity_has_component_tests(CuTest*);
void entity_get_component_tests(CuTest*);

void world_systems_tests(CuTest*);
void world_entity_systems_tests(CuTest*);

void world_register_and_get_family_tests(CuTest*);

CuSuite* get_family_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, family_all_tests);
    SUITE_ADD_TEST(suite, family_any_tests);
    SUITE_ADD_TEST(suite, family_not_tests);
    SUITE_ADD_TEST(suite, family_entity_is_part_of_tests);

    return suite;
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

CuSuite* get_system_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, world_systems_tests);
    SUITE_ADD_TEST(suite, world_entity_systems_tests);

    return suite;
}

CuSuite* get_world_suite()
{
    CuSuite* suite = CuSuiteNew();

    //SUITE_ADD_TEST(suite, world_register_and_get_family_tests);

    return suite;
}

void run_tests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_family_suite());
	CuSuiteAddSuite(suite, get_entity_suite());
	CuSuiteAddSuite(suite, get_system_suite());
    CuSuiteAddSuite(suite, get_world_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	run_tests();
}
