#include <tdd_ecs.h>
#include <tdd.h>
#include <tdd_game1.h>

#include "../../../components.h"


void render_map_system(system_t* system, world_t* world, float delta)
{
    atlas_region_t* wall_region = GET_TEXTURE_ATLAS_REGION("props_atlas", "wall");

    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            switch (map_get_cell(x, y))
            {
            case MAP_WALL:
                DrawTextureRec(*wall_region->atlas_texture, wall_region->rect, (Vector2){ x * 32, y * 32 }, WHITE);
                break;
            }
        }
    }
}