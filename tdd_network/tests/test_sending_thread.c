#include <stdio.h>
#include <tdd_utests.h>
#include <tdd_network.h>
#include <unistd.h>

#define FOO_NET_MESSAGE 0
#define BAR_NET_MESSAGE 1

/*typedef struct foo_net_message
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

void foo_net_message_init(foo_net_message_t* foo_net_message)
{
    foo_net_message->foo = -42;
    foo_net_message->bar = 1570;
    foo_net_message->flag = 1;
}

void foo_net_message_init_invalid(foo_net_message_t* foo_net_message)
{
    foo_net_message->foo = -4500; // out of bounds
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

static net_message_t* test_net_message_factory(unsigned int net_message_type)
{
    switch (net_message_type)
    {
        case FOO_NET_MESSAGE:
            return (net_message_t*)foo_net_message_create();
            
        case BAR_NET_MESSAGE:
            return (net_message_t*)bar_net_message_create();
    }

    return NULL;
}

static list_t* sent_unreliable_packet_buffers;
static list_t* sent_reliable_packet_buffers;

static int bogus_send_packet(packet_t* packet, bool reliable)
{
    printf("bogus_send_packet (messages count = %d, reliable = %d)\n", packet->messages->count, reliable);

    unsigned char* buffer = malloc(sizeof(unsigned char) * packet->buffer_size);

    memcpy(buffer, packet->buffer, packet->buffer_size);

    if (reliable)
    {
        list_push_back(sent_reliable_packet_buffers, buffer);
    }
    else
    {
        list_push_back(sent_unreliable_packet_buffers, buffer);
    }

    return 0;
}

static void clear_buffer(void **buffer)
{
    free(*buffer);
    buffer = NULL;
}

void test_sending_thread(CuTest* tc)
{
    sending_thread_t* sending_thread = sending_thread_create(16, 1, bogus_send_packet);

    sent_unreliable_packet_buffers = list_create();
    sent_reliable_packet_buffers = list_create();

    int number_of_messages = 4;
    foo_net_message_t* foo_net_message1 = foo_net_message_create();
    foo_net_message_t* foo_net_message2 = foo_net_message_create();
    bar_net_message_t* bar_net_message1 = bar_net_message_create();
    bar_net_message_t* bar_net_message2 = bar_net_message_create();
    bar_net_message_t* bar_net_message3 = bar_net_message_create();

    foo_net_message_init(foo_net_message1);
    foo_net_message_init(foo_net_message2);
    bar_net_message_init(bar_net_message1);
    bar_net_message_init(bar_net_message2);
    bar_net_message_init(bar_net_message3);

    sending_thread_start(sending_thread);

    printf("Let the sending thread work for 8 seconds...\n");

    sleep(2);

    sending_thread_enqueue_message(sending_thread, (net_message_t*)foo_net_message1, false);
    sending_thread_enqueue_message(sending_thread, (net_message_t*)bar_net_message1, true);

    sleep(2);

    sending_thread_enqueue_message(sending_thread, (net_message_t*)foo_net_message2, false);
    sending_thread_enqueue_message(sending_thread, (net_message_t*)bar_net_message2, false);

    sleep(2);

    sending_thread_enqueue_message(sending_thread, (net_message_t*)bar_net_message3, true);

    sleep(2);

    sending_thread_stop(sending_thread);

    printf("Sending thread stopped.\n");

    CuAssertIntEquals(tc, 2, sent_unreliable_packet_buffers->count);
    CuAssertIntEquals(tc, 2, sent_reliable_packet_buffers->count);

    CuAssertTrue(tc, !sending_thread->running);

    packet_t* unreliable_packet1 = packet_create_from_buffer((unsigned char*)list_get_at(sent_unreliable_packet_buffers, 0), 16, test_net_message_factory);
    packet_t* unreliable_packet2 = packet_create_from_buffer((unsigned char*)list_get_at(sent_unreliable_packet_buffers, 1), 16, test_net_message_factory);
    packet_t* reliable_packet1 = packet_create_from_buffer((unsigned char*)list_get_at(sent_reliable_packet_buffers, 0), 16, test_net_message_factory);
    packet_t* reliable_packet2 = packet_create_from_buffer((unsigned char*)list_get_at(sent_reliable_packet_buffers, 1), 16, test_net_message_factory);

    CuAssertIntEquals(tc, 1, unreliable_packet1->messages->count);
    CuAssertIntEquals(tc, 2, unreliable_packet2->messages->count);
    CuAssertIntEquals(tc, 1, reliable_packet1->messages->count);
    CuAssertIntEquals(tc, 1, reliable_packet2->messages->count);

    list_destroy(&sent_unreliable_packet_buffers, true, clear_buffer);
    list_destroy(&sent_reliable_packet_buffers, true, clear_buffer);

    sending_thread_destroy(sending_thread);

    foo_net_message_t* packet_foo_net_message1 = (foo_net_message_t*)list_get_at(unreliable_packet1->messages, 0);
    bar_net_message_t* packet_bar_net_message1 = (bar_net_message_t*)list_get_at(reliable_packet1->messages, 0);
    foo_net_message_t* packet_foo_net_message2 = (foo_net_message_t*)list_get_at(unreliable_packet2->messages, 0);
    bar_net_message_t* packet_bar_net_message2 = (bar_net_message_t*)list_get_at(unreliable_packet2->messages, 1);
    bar_net_message_t* packet_bar_net_message3 = (bar_net_message_t*)list_get_at(reliable_packet2->messages, 0);

    CuAssertIntEquals(tc, -42, packet_foo_net_message1->foo);
    CuAssertIntEquals(tc, 1570, packet_foo_net_message1->bar);
    CuAssertIntEquals(tc, 1, packet_foo_net_message1->flag);

    CuAssertIntEquals(tc, -42, packet_bar_net_message1->foo);
    CuAssertIntEquals(tc, 242, packet_bar_net_message1->bar);
    CuAssertIntEquals(tc, 0, packet_bar_net_message1->flag1);
    CuAssertIntEquals(tc, 1, packet_bar_net_message1->flag2);
    CuAssertIntEquals(tc, 1666, packet_bar_net_message1->plop);

    CuAssertIntEquals(tc, -42, packet_foo_net_message2->foo);
    CuAssertIntEquals(tc, 1570, packet_foo_net_message2->bar);
    CuAssertIntEquals(tc, 1, packet_foo_net_message2->flag);

    CuAssertIntEquals(tc, -42, packet_bar_net_message2->foo);
    CuAssertIntEquals(tc, 242, packet_bar_net_message2->bar);
    CuAssertIntEquals(tc, 0, packet_bar_net_message2->flag1);
    CuAssertIntEquals(tc, 1, packet_bar_net_message2->flag2);
    CuAssertIntEquals(tc, 1666, packet_bar_net_message2->plop);

    CuAssertIntEquals(tc, -42, packet_bar_net_message3->foo);
    CuAssertIntEquals(tc, 242, packet_bar_net_message3->bar);
    CuAssertIntEquals(tc, 0, packet_bar_net_message3->flag1);
    CuAssertIntEquals(tc, 1, packet_bar_net_message3->flag2);
    CuAssertIntEquals(tc, 1666, packet_bar_net_message3->plop);
}

void test_sending_thread_invalid_packet(CuTest* tc)
{
    sending_thread_t* sending_thread = sending_thread_create(16, 1, bogus_send_packet);
    foo_net_message_t* foo_net_message = foo_net_message_create();
    foo_net_message_t* foo_net_message_invalid = foo_net_message_create();

    foo_net_message_init(foo_net_message);
    foo_net_message_init_invalid(foo_net_message_invalid);

    sent_unreliable_packet_buffers = list_create();
    sent_reliable_packet_buffers = list_create();

    printf("Let the sending thread work for 2 seconds...\n");

    sending_thread_start(sending_thread);

    sending_thread_enqueue_message(sending_thread, (net_message_t*)foo_net_message, false);

    sleep(1);

    sending_thread_enqueue_message(sending_thread, (net_message_t*)foo_net_message_invalid, false);

    sleep(1);

    CuAssertTrue(tc, sending_thread->aborted);
    CuAssertTrue(tc, !sending_thread->running);

    sending_thread_destroy(sending_thread);
}
*/

CuSuite* get_sending_thread_suite()
{
    CuSuite* suite = CuSuiteNew();

    // SUITE_ADD_TEST(suite, test_sending_thread);
    // SUITE_ADD_TEST(suite, test_sending_thread_invalid_packet);

    return suite;
}

UNIT_TESTS(sending_thread);