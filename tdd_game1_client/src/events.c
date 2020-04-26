#include <stdlib.h>
#include <string.h>

#include "events.h"
#include "game_state_machine.h"

void create_player_connected_event(
    int player_id, bool is_local, int ready_players_mask, game_settings_t game_settings, unsigned int players_count, char nicknames[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH])
{
    player_connected_event_t* ev = malloc(sizeof(player_connected_event_t));

    ev->player_id = player_id;
    ev->is_local = is_local;
    ev->ready_players_mask = ready_players_mask;
    ev->game_settings = game_settings;
    ev->players_count = players_count;

    for (int i = 0; i < players_count; i++)
    {
        strncpy(ev->players_nicknames[i], nicknames[i], PLAYER_NICKNAME_MAX_LENGTH);
    }

    game_state_machine_push_event(EVENT_PLAYER_CONNECTED, ev);
}

void create_player_ready_event(int player_id)
{
    player_ready_event_t* ev = malloc(sizeof(player_ready_event_t));

    ev->player_id = player_id;

    game_state_machine_push_event(EVENT_PLAYER_READY, ev);

}

void create_game_ready_event()
{
    game_state_machine_push_event(EVENT_GAME_READY, NULL);
}

void create_round_start_event()
{
    game_state_machine_push_event(EVENT_ROUND_START, NULL);
}

void create_round_over_event(unsigned int* scores, int winner_player_id)
{
    round_over_event_t* ev = malloc(sizeof(round_over_event_t));

    ev->winner_player_id = winner_player_id;
    memcpy(ev->scores, scores, sizeof(unsigned int) * MAX_PLAYERS);

    game_state_machine_push_event(EVENT_ROUND_OVER, ev);
}

void create_player_update_position_event(int player_id, int x, int y)
{
    player_update_position_event_t* ev = malloc(sizeof(player_update_position_event_t));

    ev->player_id = player_id;
    ev->x = x;
    ev->y = y;

    game_state_machine_push_event(EVENT_PLAYER_UPDATE_POSITION, ev);
}

void create_player_drop_bomb_event(int player_id, unsigned int bomb_id, Vector2 bomb_position)
{
    player_drop_bomb_event_t* ev = malloc(sizeof(player_drop_bomb_event_t));

    ev->player_id = player_id;
    ev->bomb_position = bomb_position;
    ev->bomb_id = bomb_id;

    game_state_machine_push_event(EVENT_PLAYER_DROP_BOMB, ev);
}

void create_player_exploded_event(int player_id)
{
    player_exploded_event_t* ev = malloc(sizeof(player_exploded_event_t));

    ev->player_id = player_id;

    game_state_machine_push_event(EVENT_PLAYER_EXPLODED, ev);
}

void create_bonus_spawned_event(int bonus_type, int x, int y)
{
    bonus_spawned_event_t* ev = malloc(sizeof(bonus_spawned_event_t));

    ev->bonus_type = bonus_type;
    ev->x = x;
    ev->y = y;

    game_state_machine_push_event(EVENT_BONUS_SPAWNED, ev);
}

void create_player_picked_up_bonus_event(int player_id, int bonus_type)
{
    player_picked_up_bonus_event_t* ev = malloc(sizeof(player_picked_up_bonus_event_t));

    ev->player_id = player_id;
    ev->bonus_type = bonus_type;

    game_state_machine_push_event(EVENT_PLAYER_PICKED_UP_EVENT, ev);
}

void create_ui_update_event(ui_event_type_t ui_event_type, ...)
{
    va_list args;

    va_start(args, ui_event_type);

    update_ui_event_t* ev = malloc(sizeof(update_ui_event_t));

    ev->type = ui_event_type;

    switch (ui_event_type)
    {
    case UI_MAX_BOMBS_COUNT:
        ev->max_bombs_count = va_arg(args, unsigned int);
        break;

    case UI_REMAINING_BOMBS_COUNT:
        ev->remaining_bombs_count = va_arg(args, unsigned int);
        break;

    case UI_EXPLOSION_RANGE:
        ev->explosion_range = va_arg(args, unsigned int);
        break;
    }

    game_state_machine_push_event(EVENT_UPDATE_UI, ev);
}

void create_player_kick_bomb_event(Vector2 bomb_position, Vector2 kick_direction)
{
    player_kick_bomb_event_t* ev = malloc(sizeof(player_kick_bomb_event_t));

    ev->bomb_position = bomb_position;
    ev->kick_direction = kick_direction;

    game_state_machine_push_event(EVENT_PLAYER_KICK_BOMB, ev);
}