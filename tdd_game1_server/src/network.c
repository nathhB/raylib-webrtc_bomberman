#include <emscripten/emscripten.h>
#include <tdd_game1.h>

#include "game_manager.h"
#include "map.h"
#include "world.h"
#include "events.h"

static void handle_ping_net_message(ping_net_message_t*);
static void handle_player_ready_net_message(player_ready_net_message_t*);
static void handle_player_position_net_message(player_position_net_message_t*);
static void handle_player_drop_bomb_net_message(player_drop_bomb_net_message_t*);
static void handle_player_exploded_net_message(player_exploded_net_message_t*);
static void handle_player_pickup_bonus_net_message(player_pickup_bonus_net_message_t*);
static void handle_player_kick_bomb_net_message(player_kick_bomb_net_message_t*);

void EMSCRIPTEN_KEEPALIVE on_player_session_ready(int connection_id, char* nickname)
{
    printf("Player session ready ! (id: %d, nickname: %s)\n", connection_id, nickname);
    
    create_new_player_event(world, connection_manager_new_connection(connection_manager, connection_id), nickname);
}

void send_game_over_net_message(int winner_player_id)
{
    printf("Send game over net message (winner player id: %d)\n", winner_player_id);

    for (int i = 0; i < game_manager->players_count; i++)
    {
        game_over_net_message_t* game_over_net_message = game_over_net_message_create();

        game_over_net_message->winner_player_id = winner_player_id;

        SEND_RELIABLE(game_over_net_message, game_manager->players[i]->connection);
    }
}

void handle_net_message_from_client(net_message_t* net_message)
{
    switch (net_message->type)
    {
        case NET_MESSAGE_PING:
            handle_ping_net_message((ping_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_READY:
            handle_player_ready_net_message((player_ready_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_POSITION:
            handle_player_position_net_message((player_position_net_message_t*)net_message);
            break;
        
        case NET_MESSAGE_PLAYER_DROP_BOMB:
            handle_player_drop_bomb_net_message((player_drop_bomb_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_EXPLODED:
            handle_player_exploded_net_message((player_exploded_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_PICKUP_BONUS:
            handle_player_pickup_bonus_net_message((player_pickup_bonus_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_KICK_BOMB:
            handle_player_kick_bomb_net_message((player_kick_bomb_net_message_t*)net_message);
            break;

        default:
            break;
    }
}

static void handle_ping_net_message(ping_net_message_t* net_message)
{
    pong_net_message_t* pong_msg = pong_net_message_create();

    pong_msg->time = net_message->time;

    SEND_UNRELIABLE(pong_msg, game_manager->players[net_message->player_id]->connection);
}

static void handle_player_ready_net_message(player_ready_net_message_t* net_message)
{
    printf("Received player ready net message (player_id: %d)\n", net_message->player_id);

    game_manager->players[net_message->player_id]->is_ready = true;

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* p = game_manager->players[i];

        if (p->id != net_message->player_id)
        {
            player_ready_net_message_t* net_message_dup = player_ready_net_message_create();

            // make a copy of the message for each receiver as the original message will be freed later on
            net_message_dup->player_id = net_message->player_id;

            SEND_UNRELIABLE(net_message_dup, p->connection);
        }
    }
}

static void handle_player_position_net_message(player_position_net_message_t* net_message)
{
    //printf("Received player position net message (x = %d, y = %d)\n", net_message->x, net_message->y);

    player_t* sender_player = game_manager->players[net_message->player_id];

    // make sure a player client can only move his own player entity
    if (!sender_player || (net_message->player_id != sender_player->connection->unique_id))
    {
        return;
    }

    sender_player->x = net_message->x;
    sender_player->y = net_message->y;
}

static void handle_player_drop_bomb_net_message(player_drop_bomb_net_message_t* net_message)
{
    printf("Received player drop bomb net message (x: %d, y: %d, player_id: %d)\n", net_message->x, net_message->y, net_message->player_id);

    player_t* sender_player = game_manager->players[net_message->player_id];

    // make sure a player client can only drop bombs from his own entity
    if (!sender_player || (net_message->player_id != sender_player->connection->unique_id))
    {
        return;
    }

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* p = game_manager->players[i];

        if (p->id != net_message->player_id)
        {
            player_drop_bomb_net_message_t* net_message_dup = player_drop_bomb_net_message_create();

            // make a copy of the message for each receiver as the original message will be freed later on
            net_message_dup->player_id = net_message->player_id;
            net_message_dup->x = net_message->x;
            net_message_dup->y = net_message->y;

            SEND_RELIABLE(net_message_dup, p->connection);
        }        
    }
}

static void handle_player_exploded_net_message(player_exploded_net_message_t* net_message)
{
    printf("Received player exploded net message (player_id: %d)\n", net_message->player_id);

    player_t *sender_player = game_manager->players[net_message->player_id];

    // make sure a player client can only explode from this own connection
    if (!sender_player || (net_message->player_id != sender_player->connection->unique_id))
    {
        return;
    }

    create_player_exploded_event(world, sender_player);

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* p = game_manager->players[i];

        if (p->id != net_message->player_id)
        {
            player_exploded_net_message_t* net_message_dup = player_exploded_net_message_create();

            // make a copy of the message for each receiver as the original message will be freed later on
            net_message_dup->player_id = net_message->player_id;

            SEND_RELIABLE(net_message_dup, p->connection);
        }
    }
}

static void handle_player_pickup_bonus_net_message(player_pickup_bonus_net_message_t* net_message)
{
    printf("Received player pickup bonus net message (player_id: %d, x: %d, y: %d)\n", net_message->player_id, net_message->x, net_message->y);

    int bonus_type = map_consume_bonus(net_message->x, net_message->y);

    if (bonus_type)
    {
        for (int i = 0; i < game_manager->players_count; i++)
        {
            player_t* p = game_manager->players[i];
            player_apply_bonus_net_message_t* bonus_apply_net_message = player_apply_bonus_net_message_create();

            bonus_apply_net_message->player_id = net_message->player_id;
            bonus_apply_net_message->bonus_type = bonus_type;

            SEND_RELIABLE(bonus_apply_net_message, p->connection);
        }
    }
}

static void handle_player_kick_bomb_net_message(player_kick_bomb_net_message_t* net_message)
{
    printf("Received player kick bomb net message (player_id: %d, bomb_x: %d, bomb_y: %d, dir_x: %d, dir_y: %d)\n",
        net_message->player_id, net_message->bomb_x, net_message->bomb_y, net_message->dir_x, net_message->dir_y);

    for (int i = 0; i < game_manager->players_count; i++)
    {
        player_t* p = game_manager->players[i];

        if (p->id != net_message->player_id)
        {
            player_kick_bomb_net_message_t* net_message_dup = player_kick_bomb_net_message_create();

            // make a copy of the message for each receiver as the original message will be freed later on
            net_message_dup->player_id = net_message->player_id;
            net_message_dup->bomb_x = net_message->bomb_x;
            net_message_dup->bomb_y = net_message->bomb_y;
            net_message_dup->dir_x = net_message->dir_x;
            net_message_dup->dir_y = net_message->dir_y;

            SEND_UNRELIABLE(net_message_dup, p->connection);
        }
    }
}