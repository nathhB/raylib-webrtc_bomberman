#ifndef TDD_GAME1_SERVER_NETWORK_H_INCLUDED
#define TDD_GAME1_SERVER_NETWORK_H_INCLUDED

#define SERVER_PACKETS_PER_SECOND 20

// functions accessible from js
void on_player_session_ready(int, char*);
// ---

void send_game_over_net_message(int);

#endif // TDD_GAME1_SERVER_NETWORK_H_INCLUDED