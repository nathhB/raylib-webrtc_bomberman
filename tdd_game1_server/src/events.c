#include <stdlib.h>
#include <string.h>
#include <tdd_game1.h>

#include "events.h"

void create_new_player_event(world_t* world, connection_t* connection, char* nickname)
{
    new_player_event_t* ev = malloc(sizeof(new_player_event_t));

    ev->connection = connection;

    strncpy(ev->player_nickname, nickname, PLAYER_NICKNAME_MAX_LENGTH);

    world_push_event(world, EVENT_NEW_PLAYER, ev);
}

void create_player_exploded_event(world_t* world, player_t* player)
{
    player_exploded_event_t* ev = malloc(sizeof(player_exploded_event_t));

    ev->player = player;

    world_push_event(world, EVENT_PLAYER_EXPLODED, ev);
}