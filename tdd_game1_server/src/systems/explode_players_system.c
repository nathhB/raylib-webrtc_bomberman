#include <tdd_ecs.h>

#include "../events.h"

static void handle_player_exploded_event(world_t*, void*);

void explode_players_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_EXPLODED, handle_player_exploded_event);
}

static void handle_player_exploded_event(world_t* world, void* ev_data)
{
    player_exploded_event_t* player_exploded_ev = (player_exploded_event_t*)ev_data;

    printf("Player exploded ! (player_id: %d)\n", player_exploded_ev->player->id);

    player_exploded_ev->player->is_dead = true;
}