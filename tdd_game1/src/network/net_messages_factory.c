#include "../../tdd_game1.h"

net_message_t* net_messages_factory(unsigned int net_message_type)
{
    switch (net_message_type)
    {
        case NET_MESSAGE_PING:
            return (net_message_t*)ping_net_message_create();
        case NET_MESSAGE_PONG:
            return (net_message_t*)pong_net_message_create();
        case NET_MESSAGE_PLAYER_CONNECTED:
            return (net_message_t*)player_connected_net_message_create();
        case NET_MESSAGE_PLAYER_READY:
            return (net_message_t*)player_ready_net_message_create();
        case NET_MESSAGE_GAME_READY:
            return (net_message_t*)game_ready_net_message_create();

        case NET_MESSAGE_ROUND_START:
            return (net_message_t*)round_start_net_message_create();

        case NET_MESSAGE_ROUND_OVER:
            return (net_message_t*)round_over_net_message_create();

        case NET_MESSAGE_PLAYER_POSITION:
            return (net_message_t*)player_position_net_message_create();

        case NET_MESSAGE_PLAYER_DROP_BOMB:
            return (net_message_t*)player_drop_bomb_net_message_create();

        case NET_MESSAGE_PLAYER_EXPLODED:
            return (net_message_t*)player_exploded_net_message_create();

        case NET_MESSAGE_GAME_OVER:
            return (net_message_t*)game_over_net_message_create();

        case NET_MESSAGE_BONUS_MAP:
            return (net_message_t*)bonus_map_net_message_create();

        case NET_MESSAGE_PLAYER_PICKUP_BONUS:
            return (net_message_t*)player_pickup_bonus_net_message_create();

        case NET_MESSAGE_PLAYER_APPLY_BONUS:
            return (net_message_t*)player_apply_bonus_net_message_create();

        case NET_MESSAGE_PLAYER_KICK_BOMB:
            return (net_message_t*)player_kick_bomb_net_message_create();
    }

    return NULL;
}