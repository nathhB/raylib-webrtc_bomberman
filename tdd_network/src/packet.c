#include "tdd_network.h"

static int              serialize_packet_message_count(unsigned int*, stream_t*);
static net_message_t*   read_next_net_message(packet_t*, net_message_factory_func_t);
static int              measure_net_message_size(packet_t*, net_message_t*);
static void             clear_net_message(void**);
static void             debug(const char*);

packet_t* packet_create(size_t max_buffer_size)
{
    packet_t* packet = malloc(sizeof(packet_t));

    packet->mode = PACKET_SEND;
    packet->sealed = 0;
    packet->buffer = malloc(max_buffer_size);
    packet->buffer_size = max_buffer_size;
    packet->number_of_bits = 0;
    packet->w_stream = write_stream_create(packet->buffer, max_buffer_size);
    packet->m_stream = measure_stream_create();
    packet->messages = list_create();

    return packet;
}

packet_t* packet_create_from_buffer(unsigned char* buffer, size_t buffer_size, net_message_factory_func_t net_message_factory_func)
{
    packet_t* packet = malloc(sizeof(packet_t));

    packet->mode = PACKET_RECV;
    packet->buffer = buffer;
    packet->buffer_size = buffer_size;
    packet->r_stream = read_stream_create(packet->buffer, buffer_size);
    packet->messages = list_create();

    unsigned int message_count = 0;

    serialize_packet_message_count(&message_count, (stream_t*)packet->r_stream);

    for (int i = 0; i < message_count; i++)
    {
        net_message_t* net_message = read_next_net_message(packet, net_message_factory_func);

        if (net_message)
        {
            list_push_back(packet->messages, net_message);
        }
        else
        {
            return NULL;
        }
        
    }

    return packet;
}

void packet_destroy(packet_t* packet)
{
    if (packet->mode == PACKET_SEND)
    {
        write_stream_destroy(packet->w_stream);
        measure_stream_destroy(packet->m_stream);
        list_destroy(&packet->messages, true, clear_net_message);
        free(packet->buffer);
    }
    else if (packet->mode == PACKET_RECV)
    {
        read_stream_destroy(packet->r_stream);
        list_destroy(&packet->messages, true, clear_net_message);
    }
}

int packet_add_message(packet_t* packet, net_message_t* net_message)
{
    if (packet->mode != PACKET_SEND || packet->sealed)
    {
        return -1;
    }

    int net_message_size = measure_net_message_size(packet, net_message);

    if (net_message_size < 0)
    {
        return -1;
    }

    packet->number_of_bits += net_message_size;

    list_push_back(packet->messages, net_message);

    return 0;
}

int packet_can_add_message(packet_t* packet, net_message_t* net_message)
{
    int net_message_size = measure_net_message_size(packet, net_message);

    if (net_message_size < 0)
    {
        return -1;
    }

    return packet->messages->count + 1 <= MAX_MESSAGES_PER_PACKET &&
        packet->number_of_bits + net_message_size <= packet->buffer_size * 8;
}

int packet_seal(packet_t* packet)
{
    if (packet->mode != PACKET_SEND)
    {
        debug("failed to seal packet, not in send mode");
        return -1;
    }

    if (serialize_packet_message_count(&packet->messages->count, (stream_t*)packet->w_stream) < 0)
    {
        debug("failed to serialize message count");
        return -1;
    }

    list_node_t* current = packet->messages->head;

    while (current != NULL)
    {
        net_message_t* net_message = (net_message_t*)current->data;

        if (net_message_serialize(net_message, (stream_t*)packet->w_stream) < 0)
        {
            debug("failed to write message");
            return -1;
        }

        current = current->next;
    }

    if (write_stream_flush(packet->w_stream) < 0)
    {
        debug("failed to flush write stream");
        return -1;
    }

    packet->sealed = 1;

    return 0;
}

static int serialize_packet_message_count(unsigned int* message_count, stream_t* stream)
{
    SERIALIZE_UINT(*message_count, 0, MAX_MESSAGES_PER_PACKET);

    return 0;
}

static net_message_t* read_next_net_message(packet_t* packet, net_message_factory_func_t net_message_factory_func)
{
    unsigned int net_message_type;

    if (read_stream_serialize_uint(packet->r_stream, &net_message_type, 0, MAX_MESSAGE_TYPES) < 0)
    {
        return NULL;
    }

    net_message_t* net_message = net_message_factory_func(net_message_type);

    if (net_message_serialize(net_message, (stream_t*)packet->r_stream) < 0)
    {
        return NULL;
    }

    return net_message;
}

static int measure_net_message_size(packet_t* packet, net_message_t* net_message)
{
    measure_stream_reset(packet->m_stream);

    if (net_message_serialize(net_message, (stream_t*)packet->m_stream) < 0)
    {
        return -1;
    }

    return packet->m_stream->number_of_bits;
}

static void clear_net_message(void** data)
{
    free(*data); // TODO: this is not enough if net message allocates memory
}

static void debug(const char* msg)
{
#ifdef DEBUG_NETWORK
    printf("Packet: %s\n", msg);
#endif
}