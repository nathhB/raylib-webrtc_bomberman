#ifndef GAME_STATE_MACHINE_H_INCLUDED
#define GAME_STATE_MACHINE_H_INCLUDED

#include <tdd_game1.h>

#define CONNECTION_STATE "connection"
#define LOBBY_STATE "lobby"
#define GAME_STATE "game"
#define SCORE_STATE "score"

#define IS_CURRENT_STATE(state) (strcmp(game_state_machine->current_state->name, state) == 0)

typedef struct game_state_machine_data
{
    bool            is_connected;
    int             local_player_id;
    int             ready_players_mask;
    game_settings_t game_settings;
    char            nicknames[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH];
    unsigned int    scores[MAX_PLAYERS];
    bool            is_round_playing;
    int             last_round_winner_player_id;
} game_state_machine_data_t;

void                        create_game_state_machine();
void                        destroy_game_state_machine();
void                        game_state_machine_update();
void                        game_state_machine_push_event();
game_state_machine_data_t   game_state_machine_get_data();
void                        game_state_machine_register_players_nicknames(char[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH], unsigned int);

#endif