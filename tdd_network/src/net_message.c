#include "tdd_network.h"

int net_message_serialize(net_message_t* net_message, stream_t* stream)
{
    if (stream->type == STREAM_WRITE || stream->type == STREAM_MEASURE)
    {
        SERIALIZE_UINT(net_message->type, 0, MAX_MESSAGE_TYPES);
    }

    return net_message->serialize_members_func(net_message, stream);
}

size_t net_message_ensure_buffer_size(net_message_t* net_message)
{
    measure_stream_t* m_stream = measure_stream_create();
    size_t size;
    unsigned int remainder;

    net_message_serialize(net_message, (stream_t*)m_stream);

    // make the size of the buffer a multiple of word size (in bits)
    size = (m_stream->number_of_bits + 8 - 1) / 8; //round up after division
    remainder = size % (WORD_SIZE / 8);

    measure_stream_destroy(m_stream);

    return (remainder == 0) ? size : size + (WORD_SIZE / 8) - remainder;
}