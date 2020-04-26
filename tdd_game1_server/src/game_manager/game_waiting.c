#include <tdd.h>

#include "../game_manager.h"

static unsigned int get_ready_players_count();
static void         game_ready();
static void         broadcast_game_ready_net_message();

static bool is_game_ready = false;
static tdd_timer_t* game_waiting_timer = NULL;

void on_game_waiting_state_entered(finite_state_machine_t* game_state_machine, state_t* game_waiting_state)
{
    printf("Entered game waiting state\n");

    game_waiting_timer = tdd_timer_create();

    tdd_timer_start(game_waiting_timer);
}

void on_game_waiting_state_updated(finite_state_machine_t* game_state_machine, state_t* game_waiting_state)
{
    tdd_timer_update(game_waiting_timer);

    if (get_ready_players_count() == game_manager->game_settings.players_count)
    {
        if (!is_game_ready)
        {
            tdd_timer_wait(game_waiting_timer, game_ready, NEXT_ROUND_DELAY_MS);

            is_game_ready = true;
        }
    }
}

void on_game_waiting_state_exited(finite_state_machine_t* game_state_machine, state_t* game_waiting_state)
{
    printf("Exited game waiting state\n");

    tdd_timer_destroy(game_waiting_timer);
}

static unsigned int get_ready_players_count()
{
    unsigned int count = 0;

    for (int i = 0; i < game_manager->players_count; i++)
    {
        if (game_manager->players[i]->is_ready)
        {
            count++;
        }
    }

    return count;
}

static void game_ready(void* data)
{
    broadcast_game_ready_net_message();

    finite_state_machine_set_current_state(game_manager->state_machine, ROUND_PLAYING_STATE);
}

static void broadcast_game_ready_net_message()
{
    printf("Broadcast game ready net message\n");

    for (int i = 0; i < game_manager->players_count; i++)
    {
        SEND_RELIABLE(game_ready_net_message_create(), game_manager->players[i]->connection);
    }
}