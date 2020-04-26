#ifndef TDD_GAME1_CLIENT_EVENTS_H_INCLUDED
#define TDD_GAME1_CLIENT_EVENTS_H_INCLUDED

#include <stdbool.h>
#include <raylib.h>
#include <tdd_game1.h>

enum
{
    EVENT_PLAYER_CONNECTED,
    EVENT_PLAYER_READY,
    EVENT_GAME_READY,
    EVENT_ROUND_START,
    EVENT_ROUND_OVER,
    EVENT_PLAYER_UPDATE_POSITION,
    EVENT_PLAYER_DROP_BOMB,
    EVENT_PLAYER_EXPLODED,
    EVENT_BONUS_SPAWNED,
    EVENT_PLAYER_PICKED_UP_EVENT,
    EVENT_UPDATE_UI,
    EVENT_PLAYER_KICK_BOMB
};

typedef struct player_connected_event
{
    int             player_id;
    bool            is_local;
    int             ready_players_mask;
    game_settings_t game_settings;
    unsigned int    players_count;
    char            players_nicknames[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH];
} player_connected_event_t;

typedef struct player_ready_event
{
    int player_id;
} player_ready_event_t;

typedef struct round_over_event
{
    int             winner_player_id;
    unsigned int    scores[MAX_PLAYERS];
} round_over_event_t;

typedef struct player_update_position_event
{
    int player_id;
    int x;
    int y;
} player_update_position_event_t;

typedef struct player_drop_bomb_event
{
    int             player_id;
    unsigned int    bomb_id;
    Vector2         bomb_position;
} player_drop_bomb_event_t;

typedef struct player_exploded_event
{
    int player_id;
} player_exploded_event_t;

typedef struct bonus_spawned_event
{
    int bonus_type;
    int x;
    int y;
} bonus_spawned_event_t;

typedef struct player_picked_up_bonus_event
{
    int player_id;
    int bonus_type;
} player_picked_up_bonus_event_t;

typedef enum
{
    UI_MAX_BOMBS_COUNT,
    UI_REMAINING_BOMBS_COUNT,
    UI_EXPLOSION_RANGE
} ui_event_type_t;

typedef struct update_ui_event
{
    ui_event_type_t type;
    unsigned int    max_bombs_count;
    unsigned int    remaining_bombs_count;
    unsigned int    explosion_range;
} update_ui_event_t;

typedef struct player_kick_bomb_event
{
    Vector2 bomb_position;
    Vector2 kick_direction;
} player_kick_bomb_event_t;

void create_player_connected_event(int, bool, int, game_settings_t, unsigned int, char[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH]);
void create_player_ready_event(int);
void create_game_ready_event();
void create_round_start_event();
void create_round_over_event(unsigned int*, int);
void create_player_update_position_event(int, int, int);
void create_player_drop_bomb_event(int, unsigned int, Vector2);
void create_player_exploded_event(int);
void create_bonus_spawned_event(int, int, int);
void create_player_picked_up_bonus_event(int, int);
void create_ui_update_event(ui_event_type_t, ...);
void create_player_kick_bomb_event(Vector2, Vector2);

#endif // TDD_GAME1_CLIENT_EVENTS_H_INCLUDED