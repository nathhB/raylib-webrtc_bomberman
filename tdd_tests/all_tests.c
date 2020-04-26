#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"

void bag_test_initial_state(CuTest*);
void bag_append_tests(CuTest*);
void bag_append_beyond_capacity_tests(CuTest*);
void bag_clear_tests(CuTest*);
void bag_remove_at_tests(CuTest*);
void bag_set_at_tests(CuTest*);

void list_create_tests(CuTest*);
void list_push_back_tests(CuTest*);
void list_push_front_tests(CuTest*);
void list_get_at_tests(CuTest*);
void list_find_tests(CuTest*);
void list_remove_tests(CuTest*);
void list_remove_at_tests(CuTest*);
void list_destroy_tests(CuTest*);

void htable_create_tests(CuTest*);
void htable_add_and_get_tests(CuTest*);

CuSuite* get_bag_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, bag_test_initial_state);
    SUITE_ADD_TEST(suite, bag_append_tests);
    SUITE_ADD_TEST(suite, bag_append_beyond_capacity_tests);
    SUITE_ADD_TEST(suite, bag_clear_tests);
    SUITE_ADD_TEST(suite, bag_remove_at_tests);
    SUITE_ADD_TEST(suite, bag_set_at_tests);

    return suite;
}

CuSuite* get_list_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, list_create_tests);
    SUITE_ADD_TEST(suite, list_push_back_tests);
    SUITE_ADD_TEST(suite, list_push_front_tests);
    SUITE_ADD_TEST(suite, list_get_at_tests);
    SUITE_ADD_TEST(suite, list_find_tests);
    SUITE_ADD_TEST(suite, list_remove_at_tests);
    SUITE_ADD_TEST(suite, list_remove_tests);
    SUITE_ADD_TEST(suite, list_destroy_tests);

    return suite;
}

CuSuite* get_htable_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, htable_create_tests);
    SUITE_ADD_TEST(suite, htable_add_and_get_tests);

    return suite;
}

void run_tests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_bag_suite());
	CuSuiteAddSuite(suite, get_list_suite());
    CuSuiteAddSuite(suite, get_htable_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	run_tests();
}
