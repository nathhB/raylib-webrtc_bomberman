#include <stdio.h>
#include <tdd_utests.h>
#include <tdd_network.h>

void test_write_and_read(CuTest* tc)
{
    word_t buffer[4];
    bit_writer_t* writer = bit_writer_create(buffer, 4);
    bit_reader_t* reader = bit_reader_create(buffer, 4);

    bit_writer_write(writer, 42, 6);
    bit_writer_write(writer, 1, 1);
    bit_writer_write(writer, 1234567, 21);
    bit_writer_write(writer, 3456, 12);
    bit_writer_write(writer, 24, 5);
    bit_writer_write(writer, 7, 3);
    bit_writer_flush(writer);

    word_t v;
    word_t v2;
    word_t v3;
    word_t v4;
    word_t v5;
    word_t v6;

    bit_reader_read(reader, &v, 6);
    bit_reader_read(reader, &v2, 1);
    bit_reader_read(reader, &v3, 21);
    bit_reader_read(reader, &v4, 12);
    bit_reader_read(reader, &v5, 5);
    bit_reader_read(reader, &v6, 3);

    CuAssertIntEquals(tc, 42, v);
    CuAssertIntEquals(tc, 1, v2);
    CuAssertIntEquals(tc, 1234567, v3);
    CuAssertIntEquals(tc, 3456, v4);
    CuAssertIntEquals(tc, 24, v5);
    CuAssertIntEquals(tc, 7, v6);

    bit_writer_destroy(writer);
    bit_reader_destroy(reader);
}

void test_write_overflow(CuTest* tc)
{
    word_t buffer[1];
    bit_writer_t* writer = bit_writer_create(buffer, 1);

    CuAssertIntEquals(tc, 0, bit_writer_write(writer, 1234567, 21));
    CuAssertIntEquals(tc, 0, bit_writer_write(writer, 1234567, 21));
    CuAssertIntEquals(tc, -1, bit_writer_flush(writer));

    bit_writer_destroy(writer);
}

void test_write_overflow2(CuTest* tc)
{
    word_t buffer[2];
    bit_writer_t* writer = bit_writer_create(buffer, 2);

    CuAssertIntEquals(tc, 0, bit_writer_write(writer, 1234567, 32));
    CuAssertIntEquals(tc, 0, bit_writer_write(writer, 1234567, 32));
    CuAssertIntEquals(tc, -1, bit_writer_write(writer, 1234567, 32));

    bit_writer_destroy(writer);
}

CuSuite* get_bits_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_write_and_read);
    SUITE_ADD_TEST(suite, test_write_overflow);
    SUITE_ADD_TEST(suite, test_write_overflow2);

    return suite;
}

UNIT_TESTS(bits)