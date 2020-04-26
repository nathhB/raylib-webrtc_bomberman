#ifndef TDD_GAME1_CLIENT_NETWORK_H_INCLUDED
#define TDD_GAME1_CLIENT_NETWORK_H_INCLUDED

#define CLIENT_PACKETS_PER_SECOND 30
#define INTERPOLATION_STATES_COUNT 15
#define INTERPOLATION_TIME_MS 100

typedef struct interpolation_state
{
    Vector2         position;
    unsigned long   time; // in ms
} interpolation_state_t;

// functions accessible from js
void set_unreliable_channel_ready();
void set_reliable_channel_ready();
// ---

void            init_client_network();
void            destroy_client_network();
void            enqueue_client_net_message(net_message_t*, bool);
unsigned long   network_get_ping();

#endif //TDD_GAME1_CLIENT_NETWORK_H_INCLUDED