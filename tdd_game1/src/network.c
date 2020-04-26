#include "../tdd_game1.h"

static void flush_send_queue(void*);
static void handle_packet(packet_t*);
static int send_packet(packet_t*, connection_t*, bool);

connection_manager_t*       connection_manager = NULL;
static sending_service_t*   sending_service = NULL;
static tdd_timer_t*         timer = NULL;

void init_network(unsigned int packets_per_second)
{
    connection_manager = connection_manager_create();
    sending_service = sending_service_create(connection_manager, MAX_PACKET_SIZE, packets_per_second, send_packet);

#ifdef TDD_SERVER
    server_init();
#elif TDD_CLIENT
    client_init();
#endif

    timer = tdd_timer_create();

    tdd_timer_repeat(timer, flush_send_queue, 1000 / packets_per_second);
    tdd_timer_start(timer);
}

void destroy_network()
{
    sending_service_destroy(sending_service);
    connection_manager_destroy(connection_manager);
}

void enqueue_net_message(net_message_t* net_message, connection_t* connection, bool is_reliable)
{
    sending_service_enqueue_message(sending_service, net_message, connection, is_reliable);
}

void process_pending_packets(dequeue_packet_func_t dequeue_packet_func)
{
    unsigned char* packet_buffer;

    while ((packet_buffer = dequeue_packet_func()) != NULL)
    {
        packet_t* packet = packet_create_from_buffer(packet_buffer, MAX_PACKET_SIZE, net_messages_factory);

        handle_packet(packet);
        packet_destroy(packet);
    }
}

void update_network_timer()
{
    tdd_timer_update(timer);
}

tdd_timer_t* get_network_timer()
{
    return timer;
}

static void flush_send_queue(void* data)
{
    if (sending_service_flush(sending_service) < 0)
    {
        printf("Something went wrong while flushing the send queue, this should not happen\n");
    }
}

static void handle_packet(packet_t* packet)
{
    list_node_t* current_node = packet->messages->head;

    while (current_node != NULL)
    {
        net_message_t* net_message = (net_message_t*)current_node->data;

#ifdef TDD_SERVER
        handle_net_message_from_client(net_message);
#elif TDD_CLIENT
        handle_net_message_from_server(net_message);
#endif

        current_node = current_node->next;
    }
}

static int send_packet(packet_t* packet, connection_t* connection, bool is_reliable)
{
    if (is_reliable)
    {
#ifdef TDD_SERVER
        server_send_reliable_packet(packet->buffer, packet->buffer_size, connection->unique_id);
#elif TDD_CLIENT
        client_send_reliable_packet(packet->buffer, packet->buffer_size);
#endif
    }
    else
    {
#ifdef TDD_SERVER
        server_send_unreliable_packet(packet->buffer, packet->buffer_size, connection->unique_id);
#elif TDD_CLIENT
        client_send_unreliable_packet(packet->buffer, packet->buffer_size);
#endif
    }
    
    return 0;
}