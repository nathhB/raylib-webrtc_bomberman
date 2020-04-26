#include <stdio.h>
#include <tdd_utests.h>
#include <tdd_network.h>

#define FOO_NET_MESSAGE 0

typedef struct foo_net_message
{
    net_message_t   base;
    int             foo;
    unsigned int    bar;
    unsigned int    flag;
} foo_net_message_t;

int foo_net_message_serialize_members(foo_net_message_t*, stream_t*);

foo_net_message_t* foo_net_message_create()
{
    foo_net_message_t* foo_net_message = malloc(sizeof(foo_net_message_t));

    foo_net_message->base.type = FOO_NET_MESSAGE;
    foo_net_message->base.serialize_members_func = (net_message_serialize_members_func_t)foo_net_message_serialize_members;

    return foo_net_message;
}

void foo_net_message_init(foo_net_message_t* foo_net_message)
{
    foo_net_message->foo = -42;
    foo_net_message->bar = 1570;
    foo_net_message->flag = 1;
}

int foo_net_message_serialize_members(foo_net_message_t* foo_net_message, stream_t* stream)
{
    SERIALIZE_INT(foo_net_message->foo, -50, 50);
    SERIALIZE_UINT(foo_net_message->bar, 1500, 3000);
    SERIALIZE_BOOL(foo_net_message->flag);
}

void test_measure_message(CuTest* tc)
{
    foo_net_message_t* foo_msg = foo_net_message_create();
    measure_stream_t* m_strm = measure_stream_create();

    foo_net_message_init(foo_msg);
    net_message_serialize((net_message_t*)foo_msg, (stream_t*)m_strm);

    CuAssertIntEquals(tc, 25, m_strm->number_of_bits);
}

void test_write_and_read_message(CuTest* tc)
{
    foo_net_message_t* foo_msg = foo_net_message_create();

    foo_net_message_init(foo_msg);

    size_t bufferSize = net_message_ensure_buffer_size((net_message_t*)foo_msg);
    unsigned char *buffer = malloc(bufferSize);
    write_stream_t* w_strm = write_stream_create(buffer, bufferSize);

    CuAssertIntEquals(tc, 4, bufferSize);

    net_message_serialize((net_message_t*)foo_msg, (stream_t*)w_strm); // write

    write_stream_flush(w_strm);

    foo_net_message_t* foo_msg2 = foo_net_message_create();
    read_stream_t* r_strm = read_stream_create(buffer, bufferSize);
    unsigned int message_type;

    read_stream_serialize_uint(r_strm, &message_type, 0, MAX_MESSAGE_TYPES);

    net_message_serialize((net_message_t*)foo_msg2, (stream_t*)r_strm); // read

    CuAssertIntEquals(tc, -42, foo_msg2->foo);
    CuAssertIntEquals(tc, 1570, foo_msg2->bar);
    CuAssertIntEquals(tc, 1, foo_msg2->flag);
}

CuSuite* get_net_messages_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_measure_message);
    SUITE_ADD_TEST(suite, test_write_and_read_message);

    return suite;
}

UNIT_TESTS(net_messages)