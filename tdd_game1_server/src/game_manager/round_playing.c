#include <string.h>

#include "../game_manager.h"
#include "../map.h"

static bool         is_round_over();
static void         update_scores();
static player_t*    get_winning_player();
static void         reset_game();
static void         broadcast_round_start_net_message();
static void         broadcast_round_over_net_message();
static void         broadcast_bonus_map_net_message();

static bool round_is_over = false;

void on_round_playing_state_entered(finite_state_machine_t* game_state_machine, state_t* round_playing_state)
{
    printf("Entered round playing state\n");

    round_is_over = false;

    map_spawn_bonuses();
    broadcast_bonus_map_net_message();
    broadcast_round_start_net_message();
}

void on_round_playing_state_exited(finite_state_machine_t* game_state_machine, state_t* round_playing_state)
{
    printf("Exited round playing state\n");

    update_scores();
    broadcast_round_over_net_message();

    reset_game();
}

static void close_round(void* data)
{
    finite_state_machine_set_current_state(game_manager->state_machine, NEXT_ROUND_WAITING_STATE);
}

void on_round_playing_state_updated(finite_state_machine_t* game_state_machine, state_t* round_playing_state)
{
    if (!round_is_over && is_round_over())
    {
        tdd_timer_wait(game_manager->timer, close_round, 2000); // wait 2 seconds before actually closing the round

        round_is_over = true;
    }
}

static bool is_round_over()
{
    unsigned int alive_players_count = 0;

    for (int i = 0; i < game_manager->players_count; i++)
    {
        if (!game_manager->players[i]->is_dead)
        {
            alive_players_count++;
        }
    }

    return alive_players_count <= 1;
}

static void update_scores()
{
    player_t* winner = get_winning_player();

    if (winner)
    {
        winner->score++;

        printf("Player %d won this round, score: %d\n", winner->id, winner->score);
    }
    else
    {
        printf("It's a tie, no score to update\n");
    }
}

static player_t* get_winning_player()
{
    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* player = game_manager->players[i];

        if (!player->is_dead)
        {
            return player;
        }
    }

    return NULL;
}

static void reset_player(player_t* player)
{
    printf("Reset player %d\n", player->id);

    Vector2 spawn = game_manager_get_player_spawn(player->id);

    player->x = spawn.x;
    player->y = spawn.y;
    player->is_dead = false;
}

static void reset_game()
{
    printf("Reset game\n");

    for (int i = 0; i < game_manager->players_count; i++)
    {
        reset_player(game_manager->players[i]);
    }
}

static void broadcast_round_start_net_message()
{
    printf("Broadcast round start net message\n");

    for (int player_id = 0; player_id < game_manager->players_count; player_id++)
    {
        SEND_RELIABLE(round_start_net_message_create(), game_manager->players[player_id]->connection);
    }
}

static void broadcast_round_over_net_message(int winner_player_id)
{
    printf("Broadcast round over net message\n");

    unsigned int scores[MAX_PLAYERS] = { 0 };
    player_t* winner = get_winning_player();

    for (int player_id = 0; player_id < game_manager->players_count; player_id++)
    {
        scores[player_id] = game_manager->players[player_id]->score;
    }

    for (int player_id = 0; player_id < game_manager->players_count; player_id++)
    {
        player_t* player = game_manager->players[player_id];
        round_over_net_message_t* net_message = round_over_net_message_create();

        net_message->winner_player_id = (winner) ? winner->id : -1;
        memcpy(net_message->scores, scores, sizeof(unsigned int) * MAX_PLAYERS);

        SEND_RELIABLE(net_message, player->connection);
    }
}

static void broadcast_bonus_map_net_message()
{
    printf("Broadcast bonus map net message\n");

    for (int player_id = 0; player_id < game_manager->players_count; player_id++)
    {
        bonus_map_net_message_t *bonus_map_net_message = bonus_map_net_message_create();

        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                bonus_map_net_message->bonus_map[y][x] = map_get_bonus(x, y);
            }
        }

        SEND_RELIABLE(bonus_map_net_message, game_manager->players[player_id]->connection);
    }
}