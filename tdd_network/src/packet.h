#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED

#include "tdd_network.h"

#define MAX_MESSAGES_PER_PACKET 5

typedef enum
{
    PACKET_SEND,
    PACKET_RECV
} packet_mode_t;

typedef struct packet
{
    packet_mode_t       mode;
    unsigned int        sealed;
    unsigned char*      buffer;
    unsigned int        buffer_size; // in bytes
    unsigned int        number_of_bits;
    write_stream_t*     w_stream;
    read_stream_t*      r_stream;
    measure_stream_t*   m_stream;
    list_t*             messages;
} packet_t;

typedef net_message_t* (*net_message_factory_func_t)(unsigned int); 

packet_t*   packet_create(size_t);
packet_t*   packet_create_from_buffer(unsigned char*, size_t, net_message_factory_func_t);
void        packet_destroy(packet_t*);
int         packet_add_message(packet_t*, net_message_t*);
int         packet_can_add_message(packet_t*, net_message_t*);
int         packet_seal(packet_t*);

#endif