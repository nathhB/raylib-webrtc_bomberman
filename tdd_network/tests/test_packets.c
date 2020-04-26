#include <stdio.h>
#include <tdd_utests.h>
#include <tdd_network.h>

#define FOO_NET_MESSAGE 0
#define BAR_NET_MESSAGE 1
#define PLOP_NET_MESSAGE 2

typedef struct foo_net_message
{
    net_message_t   base;
    int             foo;
    unsigned int    bar;
    unsigned int    flag;
} foo_net_message_t;

typedef struct bar_net_message
{
    net_message_t   base;
    int             foo;
    int             bar;
    unsigned int    flag1;
    unsigned int    flag2;
    unsigned int    plop;
} bar_net_message_t;

typedef struct plop_net_message
{
    net_message_t   base;
    unsigned int    flag;
    int             foo;
    int             bar;
} plop_net_message_t;

int foo_net_message_serialize_members(foo_net_message_t* foo_net_message, stream_t* stream)
{
    SERIALIZE_INT(foo_net_message->foo, -50, 50);
    SERIALIZE_UINT(foo_net_message->bar, 1500, 3000);
    SERIALIZE_BOOL(foo_net_message->flag);

    return 0;
}

int bar_net_message_serialize_members(bar_net_message_t* bar_net_message, stream_t* stream)
{
    SERIALIZE_INT(bar_net_message->foo, -50, 50);
    SERIALIZE_INT(bar_net_message->bar, -100, 500);
    SERIALIZE_BOOL(bar_net_message->flag1);
    SERIALIZE_BOOL(bar_net_message->flag2);
    SERIALIZE_UINT(bar_net_message->plop, 0, 2000);

    return 0;
}

int plop_net_message_serialize_members(plop_net_message_t* plop_net_message, stream_t* stream)
{
    SERIALIZE_BOOL(plop_net_message->flag);
    SERIALIZE_INT(plop_net_message->foo, -50, 50);
    SERIALIZE_INT(plop_net_message->bar, -200, 200);

    return 0;
}

foo_net_message_t* foo_net_message_create()
{
    foo_net_message_t* foo_net_message = malloc(sizeof(foo_net_message_t));

    foo_net_message->base.type = FOO_NET_MESSAGE;
    foo_net_message->base.serialize_members_func = (net_message_serialize_members_func_t)foo_net_message_serialize_members;

    return foo_net_message;
}

bar_net_message_t* bar_net_message_create()
{
    bar_net_message_t* bar_net_message = malloc(sizeof(bar_net_message_t));

    bar_net_message->base.type = BAR_NET_MESSAGE;
    bar_net_message->base.serialize_members_func = (net_message_serialize_members_func_t)bar_net_message_serialize_members;

    return bar_net_message;
}

plop_net_message_t* plop_net_message_create()
{
    plop_net_message_t* plop_net_message = malloc(sizeof(plop_net_message_t));

    plop_net_message->base.type = PLOP_NET_MESSAGE;
    plop_net_message->base.serialize_members_func = (net_message_serialize_members_func_t)plop_net_message_serialize_members;

    return plop_net_message;
}

void foo_net_message_init(foo_net_message_t* foo_net_message)
{
    foo_net_message->foo = -42;
    foo_net_message->bar = 1570;
    foo_net_message->flag = 1;
}

void bar_net_message_init(bar_net_message_t* bar_net_message)
{
    bar_net_message->foo = -42;
    bar_net_message->bar = 242;
    bar_net_message->flag1 = 0;
    bar_net_message->flag2 = 1;
    bar_net_message->plop = 1666;
}

void plop_net_message_init(plop_net_message_t* plop_net_message)
{
    plop_net_message->flag = 1;
    plop_net_message->foo = 12;
    plop_net_message->bar = -200;
}

net_message_t* test_net_message_factory(unsigned int net_message_type)
{
    switch (net_message_type)
    {
        case FOO_NET_MESSAGE:
            return (net_message_t*)foo_net_message_create();
            
        case BAR_NET_MESSAGE:
            return (net_message_t*)bar_net_message_create();

        case PLOP_NET_MESSAGE:
            return (net_message_t*)plop_net_message_create();
    }

    return NULL;
}

void test_packet_write_message(CuTest* tc)
{
    packet_t* packet = packet_create(16);
    foo_net_message_t* foo_net_message = foo_net_message_create();
    bar_net_message_t* bar_net_message = bar_net_message_create();
    plop_net_message_t* plop_net_message = plop_net_message_create();

    foo_net_message_init(foo_net_message);
    bar_net_message_init(bar_net_message);
    plop_net_message_init(plop_net_message);

    packet_add_message(packet, (net_message_t*)foo_net_message);
    packet_add_message(packet, (net_message_t*)bar_net_message);
    packet_add_message(packet, (net_message_t*)plop_net_message);
    packet_seal(packet);

    read_stream_t* r_stream = read_stream_create(packet->buffer, 16);
    unsigned int message_count;
    unsigned int foo_message_type;
    unsigned int bar_message_type;
    unsigned int plop_message_type;
    foo_net_message_t* foo_net_message2 = foo_net_message_create();
    bar_net_message_t* bar_net_message2 = bar_net_message_create();
    plop_net_message_t* plop_net_message2 = plop_net_message_create();

    read_stream_serialize_uint(r_stream, &message_count, 0, MAX_MESSAGES_PER_PACKET);

    CuAssertIntEquals(tc, 3, message_count);

    read_stream_serialize_uint(r_stream, &foo_message_type, 0, MAX_MESSAGE_TYPES);
    read_stream_serialize_int(r_stream, &foo_net_message2->foo, -50, 50);
    read_stream_serialize_uint(r_stream, &foo_net_message2->bar, 1500, 3000);
    read_stream_serialize_bool(r_stream, &foo_net_message2->flag);

    CuAssertIntEquals(tc, FOO_NET_MESSAGE, foo_message_type);
    CuAssertIntEquals(tc, -42, foo_net_message2->foo);
    CuAssertIntEquals(tc, 1570, foo_net_message2->bar);
    CuAssertIntEquals(tc, 1, foo_net_message2->flag);

    read_stream_serialize_uint(r_stream, &bar_message_type, 0, MAX_MESSAGE_TYPES);
    read_stream_serialize_int(r_stream, &bar_net_message2->foo, -50, 50);
    read_stream_serialize_int(r_stream, &bar_net_message2->bar, -100, 500);
    read_stream_serialize_bool(r_stream, &bar_net_message2->flag1);
    read_stream_serialize_bool(r_stream, &bar_net_message2->flag2);
    read_stream_serialize_uint(r_stream, &bar_net_message2->plop, 0, 2000);

    CuAssertIntEquals(tc, BAR_NET_MESSAGE, bar_message_type);
    CuAssertIntEquals(tc, -42, bar_net_message2->foo);
    CuAssertIntEquals(tc, 242, bar_net_message2->bar);
    CuAssertIntEquals(tc, 0, bar_net_message2->flag1);
    CuAssertIntEquals(tc, 1, bar_net_message2->flag2);
    CuAssertIntEquals(tc, 1666, bar_net_message2->plop);

    read_stream_serialize_uint(r_stream, &plop_message_type, 0, MAX_MESSAGE_TYPES);
    read_stream_serialize_bool(r_stream, &plop_net_message2->flag);
    read_stream_serialize_int(r_stream, &plop_net_message2->foo, -50, 50);
    read_stream_serialize_int(r_stream, &plop_net_message2->bar, -200, 200);

    CuAssertIntEquals(tc, PLOP_NET_MESSAGE, plop_message_type);
    CuAssertIntEquals(tc, 1, plop_net_message2->flag);
    CuAssertIntEquals(tc, 12, plop_net_message2->foo);
    CuAssertIntEquals(tc, -200, plop_net_message2->bar);
}

void test_packet_read_message(CuTest* tc)
{
    packet_t* packet = packet_create(16);
    foo_net_message_t* foo_net_message = foo_net_message_create();
    bar_net_message_t* bar_net_message = bar_net_message_create();
    plop_net_message_t* plop_net_message = plop_net_message_create();

    foo_net_message_init(foo_net_message);
    bar_net_message_init(bar_net_message);
    plop_net_message_init(plop_net_message);

    packet_add_message(packet, (net_message_t*)foo_net_message);
    packet_add_message(packet, (net_message_t*)bar_net_message);
    packet_add_message(packet, (net_message_t*)plop_net_message);
    packet_seal(packet);

    packet_t* packet2 = packet_create_from_buffer(packet->buffer, 16, test_net_message_factory);

    CuAssertIntEquals(tc, 3, packet2->messages->count);

    net_message_t* net_message1 = (net_message_t*)list_get_at(packet2->messages, 0);
    net_message_t* net_message2 = (net_message_t*)list_get_at(packet2->messages, 1);
    net_message_t* net_message3 = (net_message_t*)list_get_at(packet2->messages, 2);

    CuAssertIntEquals(tc, FOO_NET_MESSAGE, net_message1->type);
    CuAssertIntEquals(tc, BAR_NET_MESSAGE, net_message2->type);
    CuAssertIntEquals(tc, PLOP_NET_MESSAGE, net_message3->type);
}

void test_packet_can_add_message(CuTest* tc)
{
    packet_t* packet = packet_create(16);
    foo_net_message_t* foo_net_message = foo_net_message_create();

    foo_net_message_init(foo_net_message);

    // cannot add more than "MAX_MESSAGES_PER_PACKET" messages per packet
    CuAssertIntEquals(tc, 1, packet_can_add_message(packet, (net_message_t*)foo_net_message));

    packet_add_message(packet, (net_message_t*)foo_net_message);
    CuAssertIntEquals(tc, 1, packet_can_add_message(packet, (net_message_t*)foo_net_message));

    packet_add_message(packet, (net_message_t*)foo_net_message);
    CuAssertIntEquals(tc, 1, packet_can_add_message(packet, (net_message_t*)foo_net_message));

    packet_add_message(packet, (net_message_t*)foo_net_message);
    CuAssertIntEquals(tc, 1, packet_can_add_message(packet, (net_message_t*)foo_net_message));

    packet_add_message(packet, (net_message_t*)foo_net_message);
    CuAssertIntEquals(tc, 1, packet_can_add_message(packet, (net_message_t*)foo_net_message));

    packet_add_message(packet, (net_message_t*)foo_net_message);
    CuAssertIntEquals(tc, 0, packet_can_add_message(packet, (net_message_t*)foo_net_message));
}

CuSuite* get_packets_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_packet_write_message);
    SUITE_ADD_TEST(suite, test_packet_read_message);
    SUITE_ADD_TEST(suite, test_packet_can_add_message);

    return suite;
}

UNIT_TESTS(packets)