#ifndef GAME_STATES_H_INCLUDED
#define GAME_STATES_H_INCLUDED

#include <tdd_game1.h>
#include <tdd_ecs.h>

typedef void (*game_state_event_handler_func_t)(finite_state_machine_t*, state_t*, int, void*);

typedef struct game_state_data
{
    tdd_timer_t*    timer;
    int             local_player_id;
} game_state_data_t;

// connection state
void    on_connection_state_updated(finite_state_machine_t*, state_t*);
void    connection_state_handle_event(finite_state_machine_t*, state_t*, int, void*);

// lobby state
void    on_lobby_state_entered(finite_state_machine_t*, state_t*);
void    on_lobby_state_exited(finite_state_machine_t*, state_t*);
void    on_lobby_state_updated(finite_state_machine_t*, state_t*);
void    create_lobby_player_entity(world_t*, int, bool);
void    lobby_state_handle_event(finite_state_machine_t*, state_t*, int, void*);

// game state
void    on_game_state_entered(finite_state_machine_t*, state_t*);
void    on_game_state_exited(finite_state_machine_t*, state_t*);
void    on_game_state_updated(finite_state_machine_t*, state_t*);
void    game_state_handle_event(finite_state_machine_t*, state_t*, int, void*);

// score state
void    on_score_state_entered(finite_state_machine_t*, state_t*);
void    on_score_state_exited(finite_state_machine_t*, state_t*);
void    on_score_state_updated(finite_state_machine_t*, state_t*);
void    create_scoreboard_player_entity(world_t*, int, bool);
void    create_scoreboard_cup_entity(world_t*, Vector2, bool);
void    score_state_handle_event(finite_state_machine_t*, state_t*, int, void*);

#endif // GAME_STATES_H_INCLUDED