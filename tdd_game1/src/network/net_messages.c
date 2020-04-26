#include <limits.h>
#include "../../tdd_game1.h"

ping_net_message_t* ping_net_message_create()
{
    ping_net_message_t* msg = malloc(sizeof(ping_net_message_t));

    msg->base.type = NET_MESSAGE_PING;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)ping_net_message_serialize_members;

    return msg;
}

int ping_net_message_serialize_members(ping_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_INT(msg->player_id, 0, MAX_PLAYERS);
    SERIALIZE_UINT(msg->time, 0, INT_MAX);

    return 0;
}

pong_net_message_t* pong_net_message_create()
{
    pong_net_message_t* msg = malloc(sizeof(pong_net_message_t));

    msg->base.type = NET_MESSAGE_PONG;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)pong_net_message_serialize_members;

    return msg;
}

int pong_net_message_serialize_members(pong_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->time, 0, INT_MAX);

    return 0;
}

player_connected_net_message_t* player_connected_net_message_create()
{
    player_connected_net_message_t* msg = malloc(sizeof(player_connected_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_CONNECTED;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_connected_net_message_serialize_members;

    return msg;
}

int serialize_game_settings(game_settings_t* game_settings, stream_t* stream)
{
    SERIALIZE_UINT(game_settings->players_count, 0, MAX_PLAYERS);
    SERIALIZE_UINT(game_settings->round_time, 0, 350); // 5 minutes max round time
    SERIALIZE_UINT(game_settings->required_rounds_to_win, 0, MAX_REQUIRED_ROUNDS);

    return 0;
}

int player_connected_net_message_serialize_members(player_connected_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_INT(msg->player_id, 0, MAX_PLAYERS);
    SERIALIZE_UINT(msg->ready_players_mask, 0, 15); // 15 = 1111 (which means all players ready)
    SERIALIZE_BOOL(msg->is_local);

    if (serialize_game_settings(&msg->game_settings, stream) != 0)
    {
        return 1;
    }

    SERIALIZE_UINT(msg->players_count, 0, MAX_PLAYERS);

    for (int i = 0; i < msg->players_count; i++)
    {
        SERIALIZE_STRING(msg->players_nicknames[i], PLAYER_NICKNAME_MAX_LENGTH);
    }

    return 0;
}

player_ready_net_message_t* player_ready_net_message_create()
{
    player_ready_net_message_t* msg = malloc(sizeof(player_ready_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_READY;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_ready_net_message_serialize_members;

    return msg;
}

int player_ready_net_message_serialize_members(player_ready_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_INT(msg->player_id, 0, MAX_PLAYERS);

    return 0;
}

game_ready_net_message_t* game_ready_net_message_create()
{
    game_ready_net_message_t* msg = malloc(sizeof(game_ready_net_message_t));

    msg->base.type = NET_MESSAGE_GAME_READY;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)game_ready_net_message_serialize_members;

    return msg;
}

int game_ready_net_message_serialize_members(game_ready_net_message_t* msg, stream_t* stream)
{
    return 0;
}

round_over_net_message_t* round_over_net_message_create()
{
    round_over_net_message_t* msg = malloc(sizeof(round_over_net_message_t));

    msg->base.type = NET_MESSAGE_ROUND_OVER;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)round_over_net_message_serialize_members;

    return msg;
}

int round_over_net_message_serialize_members(round_over_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_INT(msg->winner_player_id, -1, MAX_PLAYERS);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        SERIALIZE_UINT(msg->scores[i], 0, 5);
    }

    return 0;
}

round_start_net_message_t* round_start_net_message_create()
{
    round_start_net_message_t* msg = malloc(sizeof(round_start_net_message_t));

    msg->base.type = NET_MESSAGE_ROUND_START;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)round_start_net_message_serialize_members;

    return msg;
}

int round_start_net_message_serialize_members(round_start_net_message_t* msg, stream_t* stream)
{
    return 0;
}

player_position_net_message_t* player_position_net_message_create()
{
    player_position_net_message_t* msg = malloc(sizeof(player_position_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_POSITION;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_position_net_message_serialize_members;

    return msg;
}

int player_position_net_message_serialize_members(player_position_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS)
    SERIALIZE_UINT(msg->x, 0, MAP_WIDTH * TILE_SIZE)
    SERIALIZE_UINT(msg->y, 0, MAP_HEIGHT * TILE_SIZE)
    
    return 0;
}

player_drop_bomb_net_message_t* player_drop_bomb_net_message_create()
{
    player_drop_bomb_net_message_t* msg = malloc(sizeof(player_drop_bomb_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_DROP_BOMB;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_drop_bomb_net_message_serialize_members;

    return msg;
}

int player_drop_bomb_net_message_serialize_members(player_drop_bomb_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS)
    SERIALIZE_UINT(msg->x, 0, MAP_WIDTH)
    SERIALIZE_UINT(msg->y, 0, MAP_HEIGHT)
    
    return 0;
}

player_exploded_net_message_t*  player_exploded_net_message_create()
{
    player_exploded_net_message_t *msg = malloc(sizeof(player_exploded_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_EXPLODED;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_exploded_net_message_serialize_members;

    return msg;
}

int player_exploded_net_message_serialize_members(player_exploded_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS);

    return 0;
}

game_over_net_message_t* game_over_net_message_create()
{
    game_over_net_message_t* msg = malloc(sizeof(game_over_net_message_t));

    msg->base.type = NET_MESSAGE_GAME_OVER;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)game_over_net_message_serialize_members;

    return msg;
}

int game_over_net_message_serialize_members(game_over_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_INT(msg->winner_player_id, -1, MAX_PLAYERS); // -1 in case of tie

    return 0;
}

bonus_map_net_message_t* bonus_map_net_message_create()
{
    bonus_map_net_message_t* msg = malloc(sizeof(bonus_map_net_message_t));

    msg->base.type = NET_MESSAGE_BONUS_MAP;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)bonus_map_net_message_serialize_members;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            msg->bonus_map[y][x] = 0;
        }
    }

    return msg;
}

int bonus_map_net_message_serialize_members(bonus_map_net_message_t* msg, stream_t* stream)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            SERIALIZE_UINT(msg->bonus_map[y][x], 0, BONUS_TYPES_COUNT);
        }
    }

    return 0;
}

player_pickup_bonus_net_message_t* player_pickup_bonus_net_message_create()
{
    player_pickup_bonus_net_message_t* msg = malloc(sizeof(player_pickup_bonus_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_PICKUP_BONUS;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_pickup_bonus_net_message_serialize_members;

    return msg;
}

int player_pickup_bonus_net_message_serialize_members(player_pickup_bonus_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS);
    SERIALIZE_UINT(msg->x, 0, MAP_WIDTH);
    SERIALIZE_UINT(msg->y, 0, MAP_HEIGHT);

    return 0;
}

player_apply_bonus_net_message_t* player_apply_bonus_net_message_create()
{
    player_apply_bonus_net_message_t* msg = malloc(sizeof(player_apply_bonus_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_APPLY_BONUS;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_apply_bonus_net_message_serialize_members;

    return msg;
}

int player_apply_bonus_net_message_serialize_members(player_apply_bonus_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS);
    SERIALIZE_UINT(msg->bonus_type, 0, BONUS_TYPES_COUNT);

    return 0;
}

player_kick_bomb_net_message_t* player_kick_bomb_net_message_create()
{
    player_kick_bomb_net_message_t* msg = malloc(sizeof(player_kick_bomb_net_message_t));

    msg->base.type = NET_MESSAGE_PLAYER_KICK_BOMB;
    msg->base.serialize_members_func = (net_message_serialize_members_func_t)player_kick_bomb_net_message_serialize_members;

    return msg;
}

int player_kick_bomb_net_message_serialize_members(player_kick_bomb_net_message_t* msg, stream_t* stream)
{
    SERIALIZE_UINT(msg->player_id, 0, MAX_PLAYERS);
    SERIALIZE_UINT(msg->bomb_x, 0, MAP_WIDTH);
    SERIALIZE_UINT(msg->bomb_y, 0, MAP_HEIGHT);
    SERIALIZE_INT(msg->dir_x, -1, 1);
    SERIALIZE_INT(msg->dir_y, -1, 1);

    return 0;
}