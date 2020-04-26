#ifndef TDD_GAME1_SERVER_EVENTS_H_INCLUDED
#define TDD_GAME1_SERVER_EVENTS_H_INCLUDED

#include <tdd_network.h>

#include "game_manager.h"

enum
{
    EVENT_NEW_PLAYER,
    EVENT_PLAYER_EXPLODED,
    EVENT_WALL_DESTRUCTED
};

typedef struct new_player_event
{
    connection_t*   connection;
    char            player_nickname[PLAYER_NICKNAME_MAX_LENGTH];
} new_player_event_t;

typedef struct player_exploded_event
{
    player_t* player;
} player_exploded_event_t;

void create_new_player_event(world_t*, connection_t*, char*);
void create_player_exploded_event(world_t*, player_t*);

#endif // TDD_GAME1_SERVER_EVENTS_H_INCLUDED