#include <stdlib.h>
#include <raylib.h>
#include <tdd_network.h>
#include <tdd_game1.h>

#include "network.h"
#include "game_state_machine.h"
#include "events.h"

#define PINGS_BUFFER_SIZE 5

static void send_ping_net_message(void*);

static void handle_pong_net_message(pong_net_message_t*);
static void handle_player_connected_net_message(player_connected_net_message_t*);
static void handle_player_ready_net_message(player_ready_net_message_t*);
static void handle_game_ready_net_message(game_ready_net_message_t*);
static void handle_round_start_net_message(round_start_net_message_t*);
static void handle_round_over_net_message(round_over_net_message_t*);
static void handle_player_position_net_message(player_position_net_message_t*);
static void handle_player_drop_bomb_net_message(player_drop_bomb_net_message_t*);
static void handle_player_exploded_net_message(player_exploded_net_message_t*);
static void handle_game_over_net_message(game_over_net_message_t*);
static void handle_bonus_map_net_message(bonus_map_net_message_t*);
static void handle_player_apply_bonus_net_message(player_apply_bonus_net_message_t*);
static void handle_player_kick_bomb_net_message(player_kick_bomb_net_message_t*);

static connection_t*    server_connection = NULL;
static unsigned long    pings_buffer[PINGS_BUFFER_SIZE] = { 0 };
static unsigned int     pings_count = 0;

void init_client_network()
{
    init_network(CLIENT_PACKETS_PER_SECOND);
    
    server_connection = connection_manager_new_connection(connection_manager, 0);

    tdd_timer_repeat(get_network_timer(), send_ping_net_message, 1000); // send ping every second
}

void destroy_client_network()
{
    destroy_network();
}

void enqueue_client_net_message(net_message_t* net_message, bool is_reliable)
{
    enqueue_net_message(net_message, server_connection, is_reliable);
}

unsigned long network_get_ping()
{
    unsigned long sum = 0;

    for (int i = 0; i < PINGS_BUFFER_SIZE; i++)
    {
        sum += pings_buffer[i];
    }

    return sum / PINGS_BUFFER_SIZE;
}

void handle_net_message_from_server(net_message_t* net_message)
{
    switch (net_message->type)
    {
        case NET_MESSAGE_PONG:
            handle_pong_net_message((pong_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_CONNECTED:
            handle_player_connected_net_message((player_connected_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_READY:
            handle_player_ready_net_message((player_ready_net_message_t*)net_message);
            break;

        case NET_MESSAGE_GAME_READY:
            handle_game_ready_net_message((game_ready_net_message_t*)net_message);
            break;

        case NET_MESSAGE_ROUND_START:
            handle_round_start_net_message((round_start_net_message_t*)net_message);
            break;

        case NET_MESSAGE_ROUND_OVER:
            handle_round_over_net_message((round_over_net_message_t*)net_message);
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

        case NET_MESSAGE_GAME_OVER:
            handle_game_over_net_message((game_over_net_message_t*)net_message);
            break;

        case NET_MESSAGE_BONUS_MAP:
            handle_bonus_map_net_message((bonus_map_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_APPLY_BONUS:
            handle_player_apply_bonus_net_message((player_apply_bonus_net_message_t*)net_message);
            break;

        case NET_MESSAGE_PLAYER_KICK_BOMB:
            handle_player_kick_bomb_net_message((player_kick_bomb_net_message_t*)net_message);
    
        default:
            break;
    }
}

static void send_ping_net_message(void* data)
{
    ping_net_message_t* ping_msg = ping_net_message_create();

    ping_msg->player_id = game_state_machine_get_data().local_player_id;
    ping_msg->time = get_network_timer()->elapsed_ms;

    SEND_UNRELIABLE(ping_msg)
}

static void handle_pong_net_message(pong_net_message_t *net_message)
{
    unsigned long ping = get_network_timer()->elapsed_ms - net_message->time;

    pings_buffer[pings_count % PINGS_BUFFER_SIZE] = ping;
    pings_count++;
}

static void handle_player_connected_net_message(player_connected_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player connected net message (player_id: %d, is_local: %d)", net_message->player_id, net_message->is_local);
    TraceLog(LOG_INFO, "Game settings: (players_count: %d)", net_message->game_settings.players_count);

    create_player_connected_event(
        net_message->player_id,
        net_message->is_local,
        net_message->ready_players_mask,
        net_message->game_settings,
        net_message->players_count,
        net_message->players_nicknames);
}

static void handle_player_ready_net_message(player_ready_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player ready net message (player_id: %d)", net_message->player_id);

    create_player_ready_event(net_message->player_id);
}

static void handle_game_ready_net_message(game_ready_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received game ready net message");

    create_game_ready_event();
}

static void handle_round_start_net_message(round_start_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received round start net message");

    create_round_start_event();
}

static void handle_round_over_net_message(round_over_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received round over net message (winner_player_id: %d)", net_message->winner_player_id);

    create_round_over_event(net_message->scores, net_message->winner_player_id);
}

static void handle_player_position_net_message(player_position_net_message_t *net_message)
{
    //printf("Received player position net message (player_id = %d, x = %d, y = %d)\n", net_message->player_id, net_message->x, net_message->y);
    create_player_update_position_event(net_message->player_id, net_message->x, net_message->y);
}

static void handle_player_drop_bomb_net_message(player_drop_bomb_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player drop bomb net message (player_id: %d)", net_message->player_id);

    create_player_drop_bomb_event(net_message->player_id, net_message->bomb_id, (Vector2){net_message->x, net_message->y});
}

static void handle_player_exploded_net_message(player_exploded_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player exploded net message (player_id: %d)", net_message->player_id);

    create_player_exploded_event(net_message->player_id);
}

static void handle_game_over_net_message(game_over_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received game over net message (winner_player_id: %d)", net_message->winner_player_id);
}

static void handle_bonus_map_net_message(bonus_map_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received bonus map net message:");

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map.bonuses[y][x] = net_message->bonus_map[y][x];
        }
    }

    map_print_bonuses();
}

static void handle_player_apply_bonus_net_message(player_apply_bonus_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player apply bonus net message (player_id: %d, bonus_type: %d)", net_message->player_id, net_message->bonus_type);

    create_player_picked_up_bonus_event(net_message->player_id, net_message->bonus_type);
}

static void handle_player_kick_bomb_net_message(player_kick_bomb_net_message_t *net_message)
{
    TraceLog(LOG_INFO, "Received player kick bomb net message (player_id: %d, bomb_x: %d, bomb_y: %d, dir_x: %d, dir_y: %d)",
             net_message->player_id, net_message->bomb_x, net_message->bomb_y, net_message->dir_x, net_message->dir_y);

    create_player_kick_bomb_event(
        (Vector2){net_message->bomb_x, net_message->bomb_y}, (Vector2){net_message->dir_x, net_message->dir_y});
}