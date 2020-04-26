#include <stdio.h>
#include <string.h>
#include <tdd_utests.h>
#include <tdd_network.h>

void test_write_and_read_uint(CuTest* tc)
{
    unsigned char buffer[4 * 4];
    unsigned int v = 42;
    unsigned int v2 = 4;
    unsigned int v3 = 1234567;
    unsigned int v4 = 123;
    unsigned int r_v = 0;
    unsigned int r_v2 = 0;
    unsigned int r_v3 = 0;
    unsigned int r_v4 = 0;
    write_stream_t* w_strm = write_stream_create(buffer, 4 * 4);
    read_stream_t* r_strm = read_stream_create(buffer, 4 * 4);

    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v, 33, 100);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v2, 0, 10);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v3, 12345, 9999999);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v4, 42, 123);
    write_stream_flush(w_strm);

    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v, 33, 100);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v2, 0, 10);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v3, 12345, 9999999);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v4, 42, 123);

    CuAssertIntEquals(tc, 42, r_v);
    CuAssertIntEquals(tc, 4, r_v2);
    CuAssertIntEquals(tc, 1234567, r_v3);
    CuAssertIntEquals(tc, 123, r_v4);
}

void test_write_and_read_int(CuTest* tc)
{
    unsigned char buffer[4 * 4];
    int v = -42;
    int v2 = 428;
    int v3 = -1234567;
    int v4 = -1;
    int v5 = -134;
    int v6 = -34567;
    int r_v = 0;
    int r_v2 = 0;
    int r_v3 = 0;
    int r_v4 = 0;
    int r_v5 = 0;
    int r_v6 = 0;
    write_stream_t* w_strm = write_stream_create(buffer, 4 * 4);
    read_stream_t* r_strm = read_stream_create(buffer, 4 * 4);

    w_strm->base.serialize_int_func((stream_t*)w_strm, &v, -50, 0);
    w_strm->base.serialize_int_func((stream_t*)w_strm, &v2, 400, 500);
    w_strm->base.serialize_int_func((stream_t*)w_strm, &v3, -1234567, 1234567);
    w_strm->base.serialize_int_func((stream_t*)w_strm, &v4, -100, 1);
    w_strm->base.serialize_int_func((stream_t*)w_strm, &v5, -150, 150);
    w_strm->base.serialize_int_func((stream_t*)w_strm, &v6, -50000, -30000);
    write_stream_flush(w_strm);

    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v, -50, 0);
    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v2, 400, 500);
    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v3, -1234567, 1234567);
    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v4, -100, 1);
    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v5, -150, 150);
    r_strm->base.serialize_int_func((stream_t*)r_strm, &r_v6, -50000, -30000);

    CuAssertIntEquals(tc, -42, r_v);
    CuAssertIntEquals(tc, 428, r_v2);
    CuAssertIntEquals(tc, -1234567, r_v3);
    CuAssertIntEquals(tc, -1, r_v4);
    CuAssertIntEquals(tc, -134, r_v5);
    CuAssertIntEquals(tc, -34567, r_v6);
}

void test_write_and_read_with_padding(CuTest* tc)
{
    unsigned char buffer[4 * 4];
    unsigned int v = 42;
    unsigned int v2 = 4;
    unsigned int v3 = 1234567;
    unsigned int v4 = 123;
    unsigned int r_v = 0;
    unsigned int r_v2 = 0;
    unsigned int r_v3 = 0;
    unsigned int r_v4 = 0;
    write_stream_t* w_strm = write_stream_create(buffer, 4 * 4);
    read_stream_t* r_strm = read_stream_create(buffer, 4 * 4);

    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v, 33, 100);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v2, 0, 10);
    w_strm->base.serialize_word_padding_func((stream_t*)w_strm);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v3, 12345, 9999999);
    w_strm->base.serialize_word_padding_func((stream_t*)w_strm);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v4, 42, 123);
    write_stream_flush(w_strm);

    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v, 33, 100);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v2, 0, 10);
    r_strm->base.serialize_word_padding_func((stream_t*)r_strm);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v3, 12345, 9999999);
    r_strm->base.serialize_word_padding_func((stream_t*)r_strm);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v4, 42, 123);

    CuAssertIntEquals(tc, 42, r_v);
    CuAssertIntEquals(tc, 4, r_v2);
    CuAssertIntEquals(tc, 1234567, r_v3);
    CuAssertIntEquals(tc, 123, r_v4);
}

void test_write_and_read_byte_array(CuTest* tc)
{
    unsigned char buffer[4 * 10];
    uint8_t array[] = "toto\0";
    uint8_t array2[] = "this is another test\0";
    uint8_t r_array[5];
    uint8_t r_array2[21];
    write_stream_t* w_strm = write_stream_create(buffer, 4 * 4);
    read_stream_t* r_strm = read_stream_create(buffer, 4 * 4);

    w_strm->base.serialize_byte_array_func((stream_t*)w_strm, array, strlen(array) + 1);
    w_strm->base.serialize_byte_array_func((stream_t*)w_strm, array2, strlen(array2) + 1);

    r_strm->base.serialize_byte_array_func((stream_t*)r_strm, r_array, strlen(array) + 1);
    r_strm->base.serialize_byte_array_func((stream_t*)r_strm, r_array2, strlen(array2) + 1);

    CuAssertTrue(tc, strcmp(r_array, "toto") == 0);
    CuAssertTrue(tc, strcmp(r_array2, "this is another test") == 0);
}

void test_write_and_read_mix_everything(CuTest* tc)
{
    unsigned char buffer[4 * 16];
    uint8_t array[] = "toto\0";
    uint8_t array2[] = "this is another test\0";
    unsigned int v = 42;
    unsigned int v2 = 4;
    unsigned int v3 = 1234567;
    unsigned int v4 = 123;
    
    uint8_t r_array[5];
    uint8_t r_array2[21];
    unsigned int r_v = 0;
    unsigned int r_v2 = 0;
    unsigned int r_v3 = 0;
    unsigned int r_v4 = 0;

    write_stream_t* w_strm = write_stream_create(buffer, 4 * 16);
    read_stream_t* r_strm = read_stream_create(buffer, 4 * 16);

    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v, 33, 100);
    w_strm->base.serialize_byte_array_func((stream_t*)w_strm, array, strlen(array) + 1);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v2, 0, 10);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v3, 12345, 9999999);
    w_strm->base.serialize_byte_array_func((stream_t*)w_strm, array2, strlen(array2) + 1);
    w_strm->base.serialize_uint_func((stream_t*)w_strm, &v4, 42, 123);
    write_stream_flush(w_strm);

    r_strm->base.serialize_uint_func((stream_t *)r_strm, &r_v, 33, 100);
    r_strm->base.serialize_byte_array_func((stream_t*)r_strm, r_array, strlen(array) + 1);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v2, 0, 10);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v3, 12345, 9999999);
    r_strm->base.serialize_byte_array_func((stream_t*)r_strm, r_array2, strlen(array2) + 1);
    r_strm->base.serialize_uint_func((stream_t*)r_strm, &r_v4, 42, 123);

    CuAssertIntEquals(tc, 42, r_v);
    CuAssertIntEquals(tc, 4, r_v2);
    CuAssertIntEquals(tc, 1234567, r_v3);
    CuAssertIntEquals(tc, 123, r_v4);
    CuAssertTrue(tc, strcmp(r_array, "toto") == 0);
    CuAssertTrue(tc, strcmp(r_array2, "this is another test") == 0);
}

void test_measure(CuTest* tc)
{
    unsigned int v1 = 42;
    int v2 = -42;
    unsigned int v3 = 1234567;
    int v4 = -1234567;
    int v5 = 4356;
    unsigned int v6 = 1;
    unsigned int v7 = 0;
    int v8 = -4224;
    unsigned int v9 = 4224;
    measure_stream_t* m_strm = measure_stream_create();

    CuAssertIntEquals(tc, 6, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v1, 0, 50));
    CuAssertIntEquals(tc, 7, m_strm->base.serialize_int_func((stream_t*)m_strm, &v2, -50, 50));
    CuAssertIntEquals(tc, 21, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v3, 0, 1234567));
    CuAssertIntEquals(tc, 22, m_strm->base.serialize_int_func((stream_t*)m_strm, &v4, -1234567, 1234567));
    CuAssertIntEquals(tc, 5, m_strm->base.serialize_int_func((stream_t*)m_strm, &v5, 4350, 4360));
    CuAssertIntEquals(tc, 4, m_strm->base.serialize_uint_func((stream_t*)m_strm, (unsigned int*)&v5, 4350, 4360));
    CuAssertIntEquals(tc, 1, m_strm->base.serialize_bool_func((stream_t*)m_strm, &v6));
    CuAssertIntEquals(tc, 1, m_strm->base.serialize_bool_func((stream_t*)m_strm, &v7));
    CuAssertIntEquals(tc, 9, m_strm->base.serialize_int_func((stream_t*)m_strm, &v8, -4324, -4124));
    CuAssertIntEquals(tc, 8, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v9, 4124, 4324));

    CuAssertIntEquals(tc, 84, m_strm->number_of_bits);
}

void test_measure_with_padding(CuTest* tc)
{
    unsigned int v1 = 42;
    int v2 = -42;
    unsigned int v3 = 1234567;
    int v4 = -1234567;
    int v5 = 4356;
    unsigned int v6 = 1;
    unsigned int v7 = 0;
    int v8 = -4224;
    unsigned int v9 = 4224;
    unsigned int v10 = 400;
    measure_stream_t* m_strm = measure_stream_create();

    CuAssertIntEquals(tc, 6, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v1, 0, 50));
    CuAssertIntEquals(tc, 7, m_strm->base.serialize_int_func((stream_t*)m_strm, &v2, -50, 50));
    CuAssertIntEquals(tc, 19, m_strm->base.serialize_word_padding_func((stream_t*)m_strm));
    CuAssertIntEquals(tc, 0, m_strm->base.serialize_word_padding_func((stream_t*)m_strm));
    CuAssertIntEquals(tc, 21, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v3, 0, 1234567));
    CuAssertIntEquals(tc, 22, m_strm->base.serialize_int_func((stream_t*)m_strm, &v4, -1234567, 1234567));
    CuAssertIntEquals(tc, 21, m_strm->base.serialize_word_padding_func((stream_t*)m_strm));
    CuAssertIntEquals(tc, 5, m_strm->base.serialize_int_func((stream_t*)m_strm, &v5, 4350, 4360));
    CuAssertIntEquals(tc, 27, m_strm->base.serialize_word_padding_func((stream_t*)m_strm));
    CuAssertIntEquals(tc, 4, m_strm->base.serialize_uint_func((stream_t*)m_strm, (unsigned int*)&v5, 4350, 4360));
    CuAssertIntEquals(tc, 1, m_strm->base.serialize_bool_func((stream_t*)m_strm, &v6));
    CuAssertIntEquals(tc, 1, m_strm->base.serialize_bool_func((stream_t*)m_strm, &v7));
    CuAssertIntEquals(tc, 9, m_strm->base.serialize_int_func((stream_t*)m_strm, &v8, -4324, -4124));
    CuAssertIntEquals(tc, 8, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v9, 4124, 4324));
    CuAssertIntEquals(tc, 9, m_strm->base.serialize_uint_func((stream_t*)m_strm, &v10, 0, 511));
    CuAssertIntEquals(tc, 0, m_strm->base.serialize_word_padding_func((stream_t*)m_strm));

    CuAssertIntEquals(tc, 160, m_strm->number_of_bits);
}

void test_measure_byte_array(CuTest* tc)
{
    uint8_t array[] = "this is another test\0";
    measure_stream_t* m_strm = measure_stream_create();

    CuAssertIntEquals(tc, (strlen(array) + 1) * 8, m_strm->base.serialize_byte_array_func((stream_t*)m_strm, array, strlen(array) + 1));
}

CuSuite* get_serialize_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_write_and_read_uint);
    SUITE_ADD_TEST(suite, test_write_and_read_int);
    SUITE_ADD_TEST(suite, test_write_and_read_with_padding);
    SUITE_ADD_TEST(suite, test_write_and_read_byte_array);
    SUITE_ADD_TEST(suite, test_write_and_read_mix_everything);
    SUITE_ADD_TEST(suite, test_measure);
    SUITE_ADD_TEST(suite, test_measure_with_padding);
    SUITE_ADD_TEST(suite, test_measure_byte_array);

    return suite;
}

UNIT_TESTS(serialize)