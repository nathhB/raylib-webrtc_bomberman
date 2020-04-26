#include "../tdd_game1_client.h"

void update_players_map_positions_system(system_t* system, world_t* world, float delta)
{
    list_t* players_entities = entity_group_manager_get_group(world->entity_group_manager, PLAYERS_GROUP);

    if (players_entities == NULL)
    {
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (map_get_cell(x, y) == MAP_PLAYER)
            {
                map_set_cell(x, y, MAP_GROUND);
            }
        }
    }

    list_node_t* current_node = players_entities->head;

    while (current_node)
    {
        Vector2 player_position = Vector2Add(get_transform_of_entity((entity_t*)current_node->data)->root->position, PLAYER_TRANSFORM_OFFSET);
        Vector2 map_position = (Vector2){ (int)player_position.x / TILE_SIZE, (int)player_position.y / TILE_SIZE };

        if (map_get_cell((int)map_position.x, (int)map_position.y) == MAP_GROUND)
        {
            map_set_cell((int)map_position.x, (int)map_position.y, MAP_PLAYER);
        }

        current_node = current_node->next;
    }
}