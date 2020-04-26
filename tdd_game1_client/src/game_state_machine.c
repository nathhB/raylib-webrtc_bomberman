#include <string.h>

#include "game_state_machine.h"
#include "game_states.h"

static finite_state_machine_t* game_state_machine = NULL;
static game_state_machine_data_t game_state_machine_data = {
    .local_player_id = -1,
    .ready_players_mask = 0,
    .scores = { 0 },
    .is_round_playing = false
};

static bool connection_to_lobby_transition(void* game_state_machine_data, void* connection_state_data)
{
    return ((game_state_machine_data_t*)game_state_machine_data)->is_connected;
}

static bool lobby_to_game_transition(void* game_state_machine_data, void* lobby_state_data)
{
    return ((game_state_machine_data_t*)game_state_machine_data)->is_round_playing;
}

static bool game_to_score_transition(void* game_state_machine_data, void* game_state_data)
{
    bool is_round_playing = ((game_state_machine_data_t*)game_state_machine_data)->is_round_playing;

    return !is_round_playing;
}

static bool score_to_game_transition(void* game_state_machine_data, void* game_state_data)
{
    return ((game_state_machine_data_t*)game_state_machine_data)->is_round_playing;
}

void create_game_state_machine()
{
    game_state_machine = finite_state_machine_create(&game_state_machine_data);

    state_t* connection_state = finite_state_machine_create_state(game_state_machine, CONNECTION_STATE);

    connection_state->on_state_updated = on_connection_state_updated;
    
    state_t* lobby_state = finite_state_machine_create_state(game_state_machine, LOBBY_STATE);

    lobby_state->on_state_entered = on_lobby_state_entered;
    lobby_state->on_state_exited = on_lobby_state_exited;
    lobby_state->on_state_updated = on_lobby_state_updated;

    state_t* game_state = finite_state_machine_create_state(game_state_machine, GAME_STATE);

    game_state->on_state_entered = on_game_state_entered;
    game_state->on_state_exited = on_game_state_exited;
    game_state->on_state_updated = on_game_state_updated;

    state_t* score_state = finite_state_machine_create_state(game_state_machine, SCORE_STATE);

    score_state->on_state_entered = on_score_state_entered;
    score_state->on_state_exited = on_score_state_exited;
    score_state->on_state_updated = on_score_state_updated;

    finite_state_machine_create_transition(game_state_machine, CONNECTION_STATE, LOBBY_STATE, connection_to_lobby_transition);
    finite_state_machine_create_transition(game_state_machine, LOBBY_STATE, GAME_STATE, lobby_to_game_transition);
    finite_state_machine_create_transition(game_state_machine, GAME_STATE, SCORE_STATE, game_to_score_transition);
    finite_state_machine_create_transition(game_state_machine, SCORE_STATE, GAME_STATE, score_to_game_transition);

    game_state_machine->current_state = connection_state;
}

void destroy_game_state_machine()
{
    // TODO
}

void game_state_machine_update()
{
    finite_state_machine_update(game_state_machine);
}

void game_state_machine_push_event(int event_type, void* event_data)
{
    if (IS_CURRENT_STATE(CONNECTION_STATE))
    {
        connection_state_handle_event(game_state_machine, game_state_machine->current_state, event_type, event_data);
    }
    else if (IS_CURRENT_STATE(LOBBY_STATE))
    {
        lobby_state_handle_event(game_state_machine, game_state_machine->current_state, event_type, event_data);
    }
    else if (IS_CURRENT_STATE(GAME_STATE))
    {
        game_state_handle_event(game_state_machine, game_state_machine->current_state, event_type, event_data);
    }
    else if (IS_CURRENT_STATE(SCORE_STATE))
    {
        score_state_handle_event(game_state_machine, game_state_machine->current_state, event_type, event_data);
    }
}

game_state_machine_data_t game_state_machine_get_data()
{
    return game_state_machine_data;
}

void game_state_machine_register_players_nicknames(char nicknames[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH], unsigned int players_count)
{
    TraceLog(LOG_INFO, "Players:");

    for (int i = 0; i < players_count; i++)
    {
        strcpy(game_state_machine_data.nicknames[i], nicknames[i]);

        TraceLog(LOG_INFO, "Nickname: %s", game_state_machine_data.nicknames[i]);
    }
}