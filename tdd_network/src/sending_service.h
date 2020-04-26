#ifndef SENDING_SERVICE_H_INCLUDED
#define SENDING_SERVICE_H_INCLUDED

#include "tdd_network.h"

typedef struct sending_service
{
    connection_manager_t*   connection_manager;
    size_t                  packet_size;
    send_packet_func_t      send_packet_func;
    unsigned int            flush_delay_ms;
} sending_service_t;

sending_service_t*  sending_service_create(connection_manager_t*, size_t, unsigned int, send_packet_func_t);
void                sending_service_destroy(sending_service_t*);
void                sending_service_enqueue_message(sending_service_t*, net_message_t*, connection_t*, bool);
int                 sending_service_update(sending_service_t*);
int                 sending_service_flush(sending_service_t*);

#endif // SENDING_SERVICE_H_INCLUDED