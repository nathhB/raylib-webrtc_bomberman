#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include <raylib.h>
#include <tdd_ecs.h>

#define EXPLOSION_LIFETIME 500 // in ms

void create_explosion_entity(world_t*, Vector2, unsigned int);

#endif