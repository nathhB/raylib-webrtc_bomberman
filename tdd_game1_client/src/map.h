#ifndef TDD_GAME1_CLIENT_MAP_H_INCLUDED
#define TDD_GAME1_CLIENT_MAP_H_INCLUDED

#include <tdd_ecs.h>

void map_create_physics(world_t*);
void map_create_destructible_walls_entities(world_t*);
void map_save();
void map_reset();

#endif // TDD_GAME1_CLIENT_MAP_H_INCLUDED