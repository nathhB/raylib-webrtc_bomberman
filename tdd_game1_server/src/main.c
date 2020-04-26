#include <emscripten/emscripten.h>
#include <tdd_ecs.h>
#include <tdd.h>

#include "game_manager.h"
#include "network.h"
#include "systems.h"

world_t* world = NULL;
static tdd_timer_t* exit_timer = NULL;

static void simulate_server_tick()
{
    process_pending_packets(server_dequeue_packet);
    tdd_timer_update((tdd_timer_t*)world->user_data);
    world_update(world, 0);
    game_manager_update_game_state();
    update_network_timer();
}

static void init_tdd_game1_world()
{
    tdd_timer_t* timer = tdd_timer_create();

    world = world_create();

    world->user_data = timer;

    world_register_system(world, spawn_players_system);
    world_register_system(world, explode_players_system);
    world_register_system(world, broadcast_players_positions_system);

    tdd_timer_start(timer);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Number of expected players argument is missing. Exit.\n");

        return 1;
    }

    init_network(SERVER_PACKETS_PER_SECOND);
    game_manager_init(atoi(argv[1]), atoi(argv[2]));
    init_tdd_game1_world();

    exit_timer = tdd_timer_create();

    emscripten_set_main_loop(simulate_server_tick, 60, 1);

    return 0;
}