#include "../game_manager.h"

static void next_round(void*);

void on_next_round_waiting_state_entered(finite_state_machine_t* game_state_machine, state_t* next_round_waiting_state)
{
    printf("Entered next round waiting state. Will enter round playing state again in %d seconds...\n", NEXT_ROUND_DELAY_MS / 1000);

    tdd_timer_wait(game_manager->timer, next_round, NEXT_ROUND_DELAY_MS);
}

void on_next_round_waiting_state_exited(finite_state_machine_t* game_state_machine, state_t* next_round_waiting_state)
{
    printf("Exited next round waiting state\n");
}

static void next_round(void* data)
{
    finite_state_machine_set_current_state(game_manager->state_machine, ROUND_PLAYING_STATE);
}