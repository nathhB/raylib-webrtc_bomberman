#include <emscripten/emscripten.h>

#include "../game_state_machine.h"
#include "../events.h"

static bool is_unreliable_channel_ready = false;
static bool is_reliable_channel_ready = false;
static bool received_connected_message = false;

void EMSCRIPTEN_KEEPALIVE set_unreliable_channel_ready()
{
    TraceLog(LOG_INFO, "Unreliable channel ready");

    is_unreliable_channel_ready = true;
}

void EMSCRIPTEN_KEEPALIVE set_reliable_channel_ready()
{
    TraceLog(LOG_INFO, "Reliable channel ready");

    is_reliable_channel_ready = true;
}

void on_connection_state_updated(finite_state_machine_t* game_state_machine, state_t* connection_state)
{
    if (is_unreliable_channel_ready && is_reliable_channel_ready && received_connected_message)
    {
        ((game_state_machine_data_t*)game_state_machine->data)->is_connected = true;
    }
}

void connection_state_handle_event(finite_state_machine_t* game_state_machine, state_t* connection_state, int event_type, void* event_data)
{
    if (event_type == EVENT_PLAYER_CONNECTED)
    {
        player_connected_event_t* ev = (player_connected_event_t*)event_data;

        TraceLog(LOG_INFO, "game_state_machine: connection state received EVENT_PLAYER_CONNECTED event (player_id: %d, is_local: %d)",
            ev->player_id, ev->is_local);
        TraceLog(LOG_INFO, "ready_players_mask: %u", ev->ready_players_mask);

        game_state_machine_register_players_nicknames(ev->players_nicknames, ev->players_count);

        if (ev->is_local)
        {
            received_connected_message = true;

            game_state_machine_data_t* game_state_machine_data = (game_state_machine_data_t*)game_state_machine->data;

            game_state_machine_data->local_player_id = ev->player_id;
            game_state_machine_data->ready_players_mask = ev->ready_players_mask;
            game_state_machine_data->game_settings = ev->game_settings;

            TraceLog(LOG_INFO, "game_state_machine: set local_player_id: %d", game_state_machine_data->local_player_id);
        }
    }

    free(event_data);
}