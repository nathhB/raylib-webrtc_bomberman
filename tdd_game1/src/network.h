#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

// js functions
extern void             server_init(void);
extern void             client_init(void);
extern unsigned char*   server_dequeue_packet(void);
extern unsigned char*   client_dequeue_packet(void);
extern void             server_send_reliable_packet(unsigned char*, unsigned int, int);
extern void             server_send_unreliable_packet(unsigned char*, unsigned int, int);
extern void             client_send_reliable_packet(unsigned char*, unsigned int);
extern void             client_send_unreliable_packet(unsigned char*, unsigned int);
// ---

typedef unsigned char*  (*dequeue_packet_func_t)(void);
typedef void            (*hande_packet_func_t)(packet_t*);

#ifdef TDD_SERVER
#define SEND_RELIABLE(net_msg, conn) enqueue_net_message((net_message_t*)net_msg, conn, true);
#define SEND_UNRELIABLE(net_msg, conn) enqueue_net_message((net_message_t*)net_msg, conn, false);
#elif TDD_CLIENT
#define SEND_RELIABLE(net_msg) enqueue_client_net_message((net_message_t*)net_msg, true);
#define SEND_UNRELIABLE(net_msg) enqueue_client_net_message((net_message_t*)net_msg, false);
#endif

#define MAX_PACKET_SIZE 128

extern connection_manager_t* connection_manager;

void            init_network(unsigned int);
void            destroy_network();
void            enqueue_net_message(net_message_t*, connection_t*, bool);
void            process_pending_packets(dequeue_packet_func_t);
void            handle_net_message_from_client(net_message_t*);
void            handle_net_message_from_server(net_message_t*);
void            update_network_timer();
tdd_timer_t*    get_network_timer();

#endif // NETWORK_H_INCLUDED