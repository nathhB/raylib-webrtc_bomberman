#include <string.h>

#include "game_manager.h"

static finite_state_machine_t*  create_game_state_machine();
static player_t*                create_player(connection_t*, char*);

static Vector2 player_spawns[MAX_PLAYERS] = {
    (Vector2){ .x = 1, .y = 1 },
    (Vector2){ .x = 13, .y = 1 },
    (Vector2){ .x = 1, .y = 9 },
    (Vector2){ .x = 13, .y = 9 }
};

game_manager_t* game_manager = NULL;

void game_manager_init(unsigned int expected_players_count, unsigned int required_rounds_to_win)
{
    game_manager = malloc(sizeof(game_manager_t));

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        game_manager->players[i] = NULL;
    }

    game_manager->game_settings.players_count = expected_players_count;
    game_manager->game_settings.round_time = ROUND_TIME_SEC;
    game_manager->game_settings.required_rounds_to_win = required_rounds_to_win;
    game_manager->players_count = 0;
    game_manager->state_machine = create_game_state_machine();
    game_manager->timer = tdd_timer_create();

    finite_state_machine_set_current_state(game_manager->state_machine, GAME_WAITING_STATE);
    tdd_timer_start(game_manager->timer);
}

void game_manager_destroy()
{
    for (int i = 0; i < game_manager->players_count; i++)
    {
        free(game_manager->players[i]);
    }

    tdd_timer_destroy(game_manager->timer);
    free(game_manager);
}

player_t* game_manager_create_player(connection_t* connection, char* nickname)
{
    if (game_manager->players_count == MAX_PLAYERS)
    {
        return NULL;
    }

    player_t* player = create_player(connection, nickname);

    game_manager->players[game_manager->players_count] = player;
    game_manager->players_count++;

    printf("Created new player (id: %d, x: %d, y: %d, nickname: %s)\n", player->id, player->x, player->y, player->nickname);
    printf("Players count: %d\n", game_manager->players_count);

    return player;
}

void game_manager_update_game_state()
{
    tdd_timer_update(game_manager->timer);
    finite_state_machine_update(game_manager->state_machine);
}

Vector2 game_manager_get_player_spawn(int player_id)
{
    return player_spawns[player_id];
}

static finite_state_machine_t* create_game_state_machine()
{
    finite_state_machine_t* game_state_machine = finite_state_machine_create(NULL);

    state_t* game_waiting_state =  finite_state_machine_create_state(game_state_machine, GAME_WAITING_STATE);

    game_waiting_state->on_state_entered = on_game_waiting_state_entered;
    game_waiting_state->on_state_exited = on_game_waiting_state_exited;
    game_waiting_state->on_state_updated = on_game_waiting_state_updated;

    state_t* round_playing_state = finite_state_machine_create_state(game_state_machine, ROUND_PLAYING_STATE);

    round_playing_state->on_state_entered = on_round_playing_state_entered;
    round_playing_state->on_state_exited = on_round_playing_state_exited;
    round_playing_state->on_state_updated = on_round_playing_state_updated;

    state_t* next_round_waiting_state = finite_state_machine_create_state(game_state_machine, NEXT_ROUND_WAITING_STATE);

    next_round_waiting_state->on_state_entered = on_next_round_waiting_state_entered;
    next_round_waiting_state->on_state_exited = on_next_round_waiting_state_exited;

    finite_state_machine_create_state(game_state_machine, GAME_OVER_STATE);

    return game_state_machine;
}

static player_t* create_player(connection_t* connection, char* nickname)
{
    int player_id = connection->unique_id;
    Vector2 spawn = game_manager_get_player_spawn(player_id);
    player_t* player = malloc(sizeof(player_t));

    player->id = player_id;
    player->connection = connection;
    player->x = spawn.x;
    player->y = spawn.y;
    player->is_dead = false;
    player->is_ready = false;
    player->score = 0;

    strncpy(player->nickname, nickname, PLAYER_NICKNAME_MAX_LENGTH);

    return player;
}