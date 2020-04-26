#include <raylib.h>
#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../bonus.h"
#include "../../../events.h"

static void handle_bonus_spawned_event(world_t*, void*);

void spawn_bonuses_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_BONUS_SPAWNED, handle_bonus_spawned_event);
}

static void handle_bonus_spawned_event(world_t* world, void* ev_data)
{
    bonus_spawned_event_t* ev = (bonus_spawned_event_t*)ev_data;

    TraceLog(LOG_INFO, "Spawn bonus %d at (%d, %d)", ev->bonus_type, ev->x, ev->y);

    create_bonus_entity(world, ev->bonus_type, ev->x * TILE_SIZE, ev->y * TILE_SIZE);
}