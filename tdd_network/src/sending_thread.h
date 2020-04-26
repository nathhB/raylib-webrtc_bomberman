#ifndef SENDING_THREAD_H_INCLUDED
#define SENDING_THREAD_H_INCLUDED

#include "tdd_network.h"

#define ABORT_THREAD(err_code) \
{ \
    abort_thread(sending_thread, INVALID_PACKET); \
    break; \
}

#define SEND_PACKET(packet, is_reliable) \
{ \
    packet_seal(packet); \
    if (sending_thread->send_packet_func(packet, is_reliable) < 0) \
    { \
        ABORT_THREAD(PACKET_SEND_ERROR) \
    } \
}

typedef enum
{
    INVALID_PACKET,
    PACKET_SEND_ERROR
} sending_thread_err_t;

typedef struct sending_thread sending_thread_t;

typedef int (*send_packet_func_t)(packet_t*, connection_t*, bool);
typedef void (*sending_thread_init_func_t)(sending_thread_t*);

struct sending_thread
{
    pthread_t                   thread;
    pthread_mutex_t             mutex;
    list_t*                     unreliable_messages_queue;
    list_t*                     reliable_messages_queue;
    bool                        running;
    bool                        aborted;
    sending_thread_err_t        abort_code;
    size_t                      packet_size;
    float                       send_frequency;
    send_packet_func_t          send_packet_func;
    sending_thread_init_func_t  init_func;
    int                         pthread_create_error_code;
};

sending_thread_t*   sending_thread_create(size_t, unsigned int, send_packet_func_t);
void                sending_thread_destroy(sending_thread_t*);
int                 sending_thread_start(sending_thread_t*);
void                sending_thread_stop(sending_thread_t*);
void                sending_thread_enqueue_message(sending_thread_t*, net_message_t*, bool);

#endif //SENDING_THREAD_H_INCLUDED