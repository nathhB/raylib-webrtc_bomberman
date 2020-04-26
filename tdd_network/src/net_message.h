#ifndef NET_MESSAGE_H_INCLUDED
#define NET_MESSAGE_H_INCLUDED

#include "tdd_network.h"

#define MAX_MESSAGE_TYPES 32

typedef struct  net_message net_message_t;
typedef int     (*net_message_serialize_members_func_t)(net_message_t*, stream_t*);

struct net_message
{
    unsigned int                            type;
    net_message_serialize_members_func_t    serialize_members_func;
};

int     net_message_serialize(net_message_t*, stream_t*);
size_t  net_message_ensure_buffer_size(net_message_t*);

#endif