#include <raylib.h>
#include <tdd_ecs.h>

#include "../../components.h"

void create_scoreboard_cup_entity(world_t* world, Vector2 position, bool is_blinking)
{
    TraceLog(LOG_INFO, "Create scoreboard cup entity");

    entity_t* scoreboard_cup_entity = world_create_entity(world);
    transform_t* transform = add_transform_to_entity(scoreboard_cup_entity, (int)position.x, (int)position.y);
    render_t* render = add_render_to_entity(scoreboard_cup_entity);

    if (is_blinking)
    {
        add_blink_rendering_to_entity(scoreboard_cup_entity, 3);
    }

    sprite_data_t* sprite = create_sprite_from_atlas(GET_TEXTURE_ATLAS_REGION("ui_atlas", "cup"));

    render_add_renderable(render, transform->root, (renderable_t*)sprite);
}