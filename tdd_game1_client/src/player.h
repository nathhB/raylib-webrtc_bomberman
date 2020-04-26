#ifndef PLAYER_H_INCLUDED_H
#define PLAYER_H_INCLUDED_H

#include <stdbool.h>
#include <tdd_ecs.h>

#define LOCAL_PLAYER_TAG "LOCAL_PLAYER"
#define PLAYERS_GROUP "PLAYERS"
#define PLAYER_TRANSFORM_OFFSET (Vector2){ 3, 30 }
#define PLAYER_BASE_SPEED 2
#define PLAYER_BASE_BOMBS_COUNT 1
#define PLAYER_MAX_BOMBS_COUNT 10
#define PLAYER_BASE_EXPLOSION_RANGE 2
#define PLAYER_MAX_EXPLOSION_RANGE 10
#define PLAYER_MAX_SPEED 4
#define PLAYER_ARMOR_TIME 5000

void        create_player_entity(world_t*, int, bool);
void        create_player_state_machine(world_t*, entity_t*);
const char* get_player_entity_tag(int);
bool        player_is_dead(world_t*, int);

#endif // PLAYER_H_INCLUDED_H