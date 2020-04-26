#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define TILE_SIZE 32
#define BONUS_TYPES_COUNT 7

enum
{
    MAP_GROUND,
    MAP_WALL,
    MAP_DEST_WALL,
    MAP_BOMB,
    MAP_PLAYER
};

enum
{
    BONUS_EXTRA_BOMB = 1,
    BONUS_EXPLOSION_EXPANDER,
    BONUS_ACCELERATOR,
    BONUS_INDESTRUCTIBLE_ARMOR,
    BONUS_MAX_EXPLOSION,
    BONUS_WALL_PASSER,
    BONUS_KICK
};

typedef struct map
{
    int     cells[MAP_HEIGHT][MAP_WIDTH];
    int     bonuses[MAP_HEIGHT][MAP_WIDTH];
} map_t;

extern map_t map;

int     map_get_cell(int, int);
void    map_set_cell(int, int, int);
int     map_get_bonus(int, int);
void    map_print();
void    map_print_bonuses();

#endif