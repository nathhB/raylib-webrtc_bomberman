#ifndef TDD_GAME1_H_INCLUDED
#define TDD_GAME1_H_INCLUDED

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <tdd_ecs.h>
#include <tdd_network.h>

#define MAX_PLAYERS 4
#define PLAYER_NICKNAME_MAX_LENGTH 12
#define NEXT_ROUND_DELAY_MS 5000
#define ROUND_TIME_SEC 120
#define MAX_REQUIRED_ROUNDS 5

typedef struct game_settings
{
    unsigned int    players_count;
    unsigned int    round_time; // in seconds
    unsigned int    required_rounds_to_win;
} game_settings_t;

#include "src/map.h"
#include "src/network/net_messages.h"
#include "src/network/net_messages_factory.h"
#include "src/network.h"

#endif //TDD_GAME1_H_INCLUDED