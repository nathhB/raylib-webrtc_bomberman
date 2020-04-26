#include <tdd_ecs.h>

#include "../network.h"
#include "../game_manager.h"

static void broadcast_player_position(player_t*);

static unsigned long last_send_time = 0;

void broadcast_players_positions_system(system_t* system, world_t* world, float delta)
{
    tdd_timer_t* timer = (tdd_timer_t*)world->user_data;

    if (timer->elapsed_ms - last_send_time >= 1000 / SERVER_PACKETS_PER_SECOND)
    {
        for (int player_id = 0; player_id < game_manager->players_count; player_id++)
        {
            broadcast_player_position(game_manager->players[player_id]);
        }

        last_send_time = timer->elapsed_ms;
    }
}

static void broadcast_player_position(player_t* player)
{
    for (int player_id = 0; player_id < game_manager->players_count; player_id++)
    {
        player_t* p = game_manager->players[player_id];

        if (p->id != player->id)
        {
            player_position_net_message_t *net_message = player_position_net_message_create();

            net_message->player_id = player->id;
            net_message->x = player->x;
            net_message->y = player->y;

            SEND_UNRELIABLE(net_message, p->connection);
        }
    }
}