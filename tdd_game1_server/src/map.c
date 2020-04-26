#include <tdd_game1.h>

#include "map.h"

static int  get_random_bonus();
static void spawn_bonus_at(int, int, int);

void map_spawn_bonuses()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (map_get_cell(x, y) == MAP_DEST_WALL)
            {
                int bonus_type = get_random_bonus();

                if (bonus_type)
                {
                    spawn_bonus_at(bonus_type, x, y);
                }
            }
        }
    }

    printf("Spawned bonuses:\n");
    map_print_bonuses();
}

int map_consume_bonus(int x, int y)
{
    int bonus_type = map_get_bonus(x, y);

    map.bonuses[y][x] = 0;

    return bonus_type;
}

static void spawn_bonus_at(int bonus_type, int x, int y)
{
    map.bonuses[y][x] = bonus_type;
}

static int get_random_bonus()
{
    return (rand() % BONUS_TYPES_COUNT) + 1;
}