#ifndef NET_MESSAGES_H_INCLUDED
#define NET_MESSAGES_H_INCLUDED

enum
{
    NET_MESSAGE_PING,
    NET_MESSAGE_PONG,
    NET_MESSAGE_PLAYER_CONNECTED,
    NET_MESSAGE_PLAYER_READY,
    NET_MESSAGE_GAME_READY,
    NET_MESSAGE_ROUND_START,
    NET_MESSAGE_ROUND_OVER,
    NET_MESSAGE_PLAYER_POSITION,
    NET_MESSAGE_PLAYER_DROP_BOMB,
    NET_MESSAGE_PLAYER_EXPLODED,
    NET_MESSAGE_GAME_OVER,
    NET_MESSAGE_BONUS_MAP,
    NET_MESSAGE_PLAYER_PICKUP_BONUS,
    NET_MESSAGE_PLAYER_APPLY_BONUS,
    NET_MESSAGE_PLAYER_KICK_BOMB
};

typedef struct ping_net_message
{
    net_message_t   base;
    int             player_id;
    unsigned int    time;
} ping_net_message_t;

typedef struct pong_net_message
{
    net_message_t   base;
    unsigned int    time;
} pong_net_message_t;

typedef struct player_connected_net_message
{
    net_message_t   base;
    int             player_id;
    unsigned int    is_local;
    unsigned int    ready_players_mask;
    game_settings_t game_settings;
    unsigned int    players_count;
    char            players_nicknames[MAX_PLAYERS][PLAYER_NICKNAME_MAX_LENGTH];
} player_connected_net_message_t;

typedef struct player_ready_net_message
{
    net_message_t   base;
    int             player_id;
} player_ready_net_message_t;

typedef struct game_ready_net_message
{
    net_message_t   base;
} game_ready_net_message_t;

typedef struct round_start_net_message
{
    net_message_t   base;
} round_start_net_message_t;

typedef struct round_over_net_message
{
    net_message_t   base;
    int             winner_player_id;
    unsigned int    scores[MAX_PLAYERS];
} round_over_net_message_t;

typedef struct player_position_net_message
{
    net_message_t   base;
    int             player_id;
    int             x;
    int             y;
} player_position_net_message_t;

typedef struct player_drop_bomb_net_message
{
    net_message_t   base;
    int             player_id;
    unsigned int    bomb_id;
    int             x;
    int             y;
} player_drop_bomb_net_message_t;

typedef struct bomb_explode_net_message
{
    net_message_t base;
} bomb_explode_net_message_t;

typedef struct player_exploded_net_message
{
    net_message_t   base;
    int             player_id;
} player_exploded_net_message_t;

typedef struct game_over_net_message
{
    net_message_t   base;
    int             winner_player_id;
} game_over_net_message_t;

typedef struct bonus_map_net_message 
{
    net_message_t   base;
    int             bonus_map[MAP_HEIGHT][MAP_WIDTH];
} bonus_map_net_message_t;

typedef struct player_pickup_bonus_net_message
{
    net_message_t   base;
    int             player_id;
    int             x;
    int             y;
} player_pickup_bonus_net_message_t;

typedef struct player_apply_bonus_net_message
{
    net_message_t   base;
    int             player_id;
    int             bonus_type;
} player_apply_bonus_net_message_t;

typedef struct player_kick_bomb_net_message
{
    net_message_t   base;
    int             player_id;
    int             bomb_x;
    int             bomb_y;
    int             dir_x;
    int             dir_y;
} player_kick_bomb_net_message_t;


ping_net_message_t*                 ping_net_message_create();
int                                 ping_net_message_serialize_members(ping_net_message_t*, stream_t*);

pong_net_message_t*                 pong_net_message_create();
int                                 pong_net_message_serialize_members(pong_net_message_t*, stream_t*);

player_connected_net_message_t*     player_connected_net_message_create();
int                                 player_connected_net_message_serialize_members(player_connected_net_message_t*, stream_t*);

player_ready_net_message_t*         player_ready_net_message_create();
int                                 player_ready_net_message_serialize_members(player_ready_net_message_t*, stream_t*);

game_ready_net_message_t*           game_ready_net_message_create();
int                                 game_ready_net_message_serialize_members(game_ready_net_message_t*, stream_t*);

round_start_net_message_t*          round_start_net_message_create();
int                                 round_start_net_message_serialize_members(round_start_net_message_t*, stream_t*);

round_over_net_message_t*           round_over_net_message_create();
int                                 round_over_net_message_serialize_members(round_over_net_message_t*, stream_t*);

player_position_net_message_t*      player_position_net_message_create();
int                                 player_position_net_message_serialize_members(player_position_net_message_t*, stream_t*);

player_drop_bomb_net_message_t*     player_drop_bomb_net_message_create();
int                                 player_drop_bomb_net_message_serialize_members(player_drop_bomb_net_message_t*, stream_t*);

player_exploded_net_message_t*      player_exploded_net_message_create();
int                                 player_exploded_net_message_serialize_members(player_exploded_net_message_t*, stream_t*);

game_over_net_message_t*            game_over_net_message_create();
int                                 game_over_net_message_serialize_members(game_over_net_message_t*, stream_t*);

bonus_map_net_message_t*            bonus_map_net_message_create();
int                                 bonus_map_net_message_serialize_members(bonus_map_net_message_t*, stream_t*);

player_pickup_bonus_net_message_t*  player_pickup_bonus_net_message_create();
int                                 player_pickup_bonus_net_message_serialize_members(player_pickup_bonus_net_message_t*, stream_t*);

player_apply_bonus_net_message_t*   player_apply_bonus_net_message_create();
int                                 player_apply_bonus_net_message_serialize_members(player_apply_bonus_net_message_t*, stream_t*);

player_kick_bomb_net_message_t*     player_kick_bomb_net_message_create();
int                                 player_kick_bomb_net_message_serialize_members(player_kick_bomb_net_message_t*, stream_t*);

#endif // NET_MESSAGES_H_INCLUDED