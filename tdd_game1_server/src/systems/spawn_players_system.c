#include <string.h>
#include <tdd_ecs.h>

#include "../game_manager.h"
#include "../events.h"

static void handle_new_player_event(world_t*, void*);
static void send_player_connected_net_message(connection_t*, player_t*, bool);

void spawn_players_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_NEW_PLAYER, handle_new_player_event);
}

static void handle_new_player_event(world_t* world, void* ev_data)
{
    new_player_event_t* new_player_ev = (new_player_event_t*)ev_data;

    printf("Received new player event (id: %d, nickname: %s)\n", new_player_ev->connection->unique_id, new_player_ev->player_nickname);

    player_t* player = game_manager_create_player(new_player_ev->connection, new_player_ev->player_nickname);

    send_player_connected_net_message(player->connection, player, true);

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* p = game_manager->players[i];

        if (p->id != player->id)
        {
            send_player_connected_net_message(p->connection, player, false);
        }        
    }
}

static void send_player_connected_net_message(connection_t* connection, player_t* player, bool is_local)
{
    player_connected_net_message_t* net_message = player_connected_net_message_create();

    net_message->player_id = player->id;
    net_message->is_local = is_local;
    net_message->ready_players_mask = 0;
    net_message->game_settings = game_manager->game_settings;
    net_message->players_count = game_manager->players_count;

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* player = game_manager->players[i];

        if (player->is_ready)
        {
            B_SET(net_message->ready_players_mask, i);
        }

        strncpy(net_message->players_nicknames[i], player->nickname, PLAYER_NICKNAME_MAX_LENGTH);
    }

    printf("Send player connected message (player_id = %d, is_local = %d)\n", net_message->player_id, net_message->is_local);
    
    SEND_RELIABLE(net_message, connection);
}