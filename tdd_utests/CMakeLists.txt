cmake_minimum_required (VERSION 3.0)
project (libtdd_utests)

set(SOURCES src/CuTest.c)
set(HEADERS src/CuTest.h tdd_utests.h)

add_library(tdd_utests ${SOURCES} ${HEADERS})