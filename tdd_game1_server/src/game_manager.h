#ifndef TDD_GAME1_SERVER_GAME_MANAGER_H_INCLUDED
#define TDD_GAME1_SERVER_GAME_MANAGER_H_INCLUDED

// game states
#define GAME_WAITING_STATE "game_waiting"
#define ROUND_PLAYING_STATE "round_playing"
#define NEXT_ROUND_WAITING_STATE "next_round_waiting"
#define GAME_OVER_STATE "game_over"

#include <tdd_game1.h>

typedef struct player
{
    connection_t*   connection;
    char            nickname[PLAYER_NICKNAME_MAX_LENGTH];
    int             id;
    int             x;
    int             y;
    bool            is_dead;
    bool            is_ready;
    unsigned int    score;
} player_t;

typedef struct game_manager
{
    player_t*               players[MAX_PLAYERS];
    unsigned int            players_count;
    game_settings_t         game_settings;
    finite_state_machine_t* state_machine;
    tdd_timer_t*            timer;
} game_manager_t;

extern game_manager_t* game_manager;

void            game_manager_init(unsigned int, unsigned int);
void            game_manager_destroy();
player_t*       game_manager_create_player(connection_t*, char*);
void            game_manager_update_game_state();
Vector2         game_manager_get_player_spawn(int);

// game waiting state
void            on_game_waiting_state_entered(finite_state_machine_t*, state_t*);
void            on_game_waiting_state_exited(finite_state_machine_t*, state_t*);
void            on_game_waiting_state_updated(finite_state_machine_t*, state_t*);

// round playing state
void            on_round_playing_state_entered(finite_state_machine_t*, state_t*);
void            on_round_playing_state_exited(finite_state_machine_t*, state_t*);
void            on_round_playing_state_updated(finite_state_machine_t*, state_t*);

// next round waiting state
void            on_next_round_waiting_state_entered(finite_state_machine_t*, state_t*);
void            on_next_round_waiting_state_exited(finite_state_machine_t*, state_t*);

#endif // TDD_GAME1_SERVER_GAME_MANAGER_H_INCLUDED