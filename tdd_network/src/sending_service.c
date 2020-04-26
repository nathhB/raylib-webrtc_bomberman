#include <math.h>
#include <time.h>
#include <stdarg.h>

#include "tdd_network.h"

static packet_t*    build_packet(sending_service_t*, list_t*);
static int          send_packet(sending_service_t*, packet_t*, connection_t*, bool);
static void         debug(const char*, ...);
static void         debug_connections();

sending_service_t* sending_service_create(connection_manager_t* connection_manager, size_t packet_size, unsigned int packets_per_second, send_packet_func_t send_packet_func)
{
    sending_service_t* sending_service = malloc(sizeof(sending_service_t));

    sending_service->connection_manager = connection_manager;
    sending_service->packet_size = packet_size;
    sending_service->send_packet_func = send_packet_func;
    sending_service->flush_delay_ms = 1000 / packets_per_second;

    return sending_service;
}

void sending_service_destroy(sending_service_t* sending_service)
{    
    free(sending_service);
}

void sending_service_enqueue_message(sending_service_t* sending_service, net_message_t* net_message, connection_t* conn, bool is_reliable)
{
    list_push_back(is_reliable ? conn->reliable_messages_queue : conn->unreliable_messages_queue, net_message);
}

int sending_service_flush(sending_service_t* sending_service)
{
    for (int i = 0; i < sending_service->connection_manager->connections->capacity; i++)
    {
        connection_t* conn = bag_get_at(sending_service->connection_manager->connections, i);

        if (conn)
        {
            packet_t* unreliable_packet = NULL;
            packet_t* reliable_packet = NULL;

            if (conn->unreliable_messages_queue->count > 0)
            {
                unreliable_packet = build_packet(sending_service, conn->unreliable_messages_queue);

                if (unreliable_packet == NULL)
                {
                    return -1;
                }
            }

            if (conn->reliable_messages_queue->count > 0)
            {
                reliable_packet = build_packet(sending_service, conn->reliable_messages_queue);

                if (reliable_packet == NULL)
                {
                    return -1;
                }
            }

            if (unreliable_packet)
            {
                if (send_packet(sending_service, unreliable_packet, conn, false) < 0)
                {
                    return -1;
                }

                packet_destroy(unreliable_packet);
            }

            if (reliable_packet)
            {
                if (send_packet(sending_service, reliable_packet, conn, true) < 0)
                {
                    return -1;
                }

                packet_destroy(reliable_packet);
            }
        }
    }

    return 0;
}

static int send_packet(sending_service_t* sending_service, packet_t* packet, connection_t* connection, bool is_reliable)
{
    if (packet_seal(packet) < 0)
    {
        debug("failed to seal packet");
        return -1;
    }

    return sending_service->send_packet_func(packet, connection, is_reliable);
}

static packet_t* build_packet(sending_service_t* sending_service, list_t* net_message_queue)
{
    packet_t* packet = packet_create(sending_service->packet_size);

    while (net_message_queue->count > 0)
    {
        net_message_t* net_message = (net_message_t*)list_remove_at(net_message_queue, 0); // fifo

        int can_add_res = packet_can_add_message(packet, net_message);

        if (can_add_res < 0)
        {
            debug("invalid packet (measure)");
            return NULL; // packet is invalid (measure issue)
        }
        else if (!can_add_res)
        {
            break; // packet can't be added because there is no remaining space in packet
        }

        if (packet_add_message(packet, net_message) < 0)
        {
            debug("invalid packet (write)");
            return NULL; // packet is invalid (write issue)
        }
    }

    return packet;
}

static void debug(const char* format, ...)
{
#ifdef DEBUG_NETWORK
    va_list arg;
    va_start(arg, format);

    char msg[256];

    vsprintf(msg, format, arg);
    printf("Sending service: %s\n", msg);
#endif
}

static void debug_connections(sending_service_t* sending_service)
{
#ifdef DEBUG_NETWORK
    for (int i = 0; i < sending_service->connection_manager->connections->capacity; i++)
    {
        connection_t* conn = bag_get_at(sending_service->connection_manager->connections, i);

        if (conn)
        {
            debug("Connection (id: %d)\n\t- Enqueued unreliable messages: %d\n\t- Enqueued reliable messages: %d",
                conn->unique_id, conn->unreliable_messages_queue->count, conn->reliable_messages_queue->count);
        }
    }
#endif
}