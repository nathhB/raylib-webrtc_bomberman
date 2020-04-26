#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED

#include <stdbool.h>
#include <raylib.h>
#include <tdd_ecs.h>
#include <tdd_game1.h>

#define BOMB_LIFETIME 3000 // in ms
#define LOCAL_PLAYER_BOMBS_GROUP "LOCAL_PLAYER_BOMBS"
#define DESTRUCTIBLE_ENTITIES_GROUP "DESTRUCTIBLE_ENTITIES"
#define BOMBS_GROUP "BOMBS"
#define KICKED_BOMB_SPEED 3

void        create_bomb_entity(world_t*, Vector2, bool, unsigned int);
void        bomb_explode(world_t*, entity_t*);
entity_t*   find_bomb_entity_at(world_t*, Vector2);

#endif //BOMB_H_INCLUDED