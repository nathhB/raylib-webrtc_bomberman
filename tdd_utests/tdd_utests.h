#ifndef TDD_UTESTS_H_INCLUDED
#define TDD_UTESTS_H_INCLUDED

#include <stdio.h>
#include "src/CuTest.h"

#define UNIT_TESTS(suite_name) \
int main(void) \
{ \
    CuString *output = CuStringNew(); \
	CuSuite* suite = CuSuiteNew(); \
\
	CuSuiteAddSuite(suite, get_##suite_name##_suite()); \
\
	CuSuiteRun(suite); \
	CuSuiteSummary(suite, output); \
	CuSuiteDetails(suite, output); \
	printf("%s\n", output->buffer); \
\
    return suite->failCount; \
}

#endif // TDD_UTESTS_H_INCLUDED