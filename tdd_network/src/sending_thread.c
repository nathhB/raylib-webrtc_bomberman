#include <unistd.h>

#include "tdd_network.h"

static void*        send_packets_rountine(void*);
//static void         abort_thread(sending_thread_t*, sending_thread_err_t);

sending_thread_t* sending_thread_create(size_t packet_size, unsigned int packets_per_seconds, send_packet_func_t send_packet_func)
{
    sending_thread_t* sending_thread = malloc(sizeof(sending_thread_t));

    sending_thread->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sending_thread->unreliable_messages_queue = list_create();
    sending_thread->reliable_messages_queue = list_create();
    sending_thread->running = false;
    sending_thread->aborted = false;
    sending_thread->packet_size = packet_size;
    sending_thread->send_frequency = 1.0 / packets_per_seconds;
    sending_thread->send_packet_func = send_packet_func;
    sending_thread->pthread_create_error_code = 0;
    sending_thread->init_func = NULL;

    return sending_thread;
}

void sending_thread_destroy(sending_thread_t* sending_thread)
{
    if (sending_thread->running)
    {
        sending_thread_stop(sending_thread);
    }

    free(sending_thread);
}

int sending_thread_start(sending_thread_t* sending_thread)
{
    if (sending_thread->running)
    {
        return 0;
    }

    int pthread_create_res = pthread_create(&sending_thread->thread, NULL, send_packets_rountine, sending_thread);

    if (pthread_create_res != 0)
    {
        sending_thread->pthread_create_error_code = pthread_create_res;
        return -1;
    }

    sending_thread->running = true;

    return 0;
}

void sending_thread_stop(sending_thread_t* sending_thread)
{
    sending_thread->running = false;

    pthread_join(sending_thread->thread, NULL);
}

void sending_thread_enqueue_message(sending_thread_t* sending_thread, net_message_t* net_message, bool reliable)
{
    pthread_mutex_lock(&sending_thread->mutex);

    list_push_back(reliable ? sending_thread->reliable_messages_queue : sending_thread->unreliable_messages_queue, net_message);

    pthread_mutex_unlock(&sending_thread->mutex);
}

static void* send_packets_rountine(void* arg)
{
    //sending_thread_t* sending_thread = (sending_thread_t*)arg;

    // TODO

    pthread_exit(NULL);
}

/*static void abort_thread(sending_thread_t* sending_thread, sending_thread_err_t code)
{
    printf("Sending thread aborted with code: %d", code);

    sending_thread->aborted = true;
    sending_thread->abort_code = code;
}*/