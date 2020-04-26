#include "rendering.h"

rect_data_t* create_rectangle(int width, int height, Color color)
{
    rect_data_t* rect_data = malloc(sizeof(rect_data_t));

    rect_data->shape_base.type = SHAPE_RECTANGLE;

    rect_data->shape_base.base.type = RENDERABLE_SHAPE;
    rect_data->shape_base.base.color = color;
    rect_data->shape_base.base.transform_node = NULL;
    rect_data->shape_base.base.is_rendering_enabled = true;

    rect_data->width = width;
    rect_data->height = height;

    return rect_data;
}

circle_data_t* create_circle(int radius, Color color)
{
    circle_data_t* circle_data = malloc(sizeof(circle_data_t));

    circle_data->shape_base.type = SHAPE_CIRCLE;

    circle_data->shape_base.base.type = RENDERABLE_SHAPE;
    circle_data->shape_base.base.color = color;
    circle_data->shape_base.base.transform_node = NULL;
    circle_data->shape_base.base.is_rendering_enabled = true;

    circle_data->radius = radius;

    return circle_data;
}

sprite_data_t* create_sprite(Texture2D* texture)
{
    sprite_data_t* sprite_data = malloc(sizeof(sprite_data_t));

    sprite_data->base.type = RENDERABLE_SPRITE;
    sprite_data->base.color = WHITE;
    sprite_data->base.transform_node = NULL;
    sprite_data->base.is_rendering_enabled = true;

    sprite_data->texture = texture;
    sprite_data->atlas_region_rect = NULL;

    return sprite_data;
}

sprite_data_t* create_sprite_from_atlas(atlas_region_t* atlas_region)
{
    if (atlas_region == NULL)
    {
        TraceLog(LOG_WARNING, "Failed to create sprite from atlas, region is NULL");
        return NULL;
    }

    sprite_data_t* sprite_data = create_sprite(atlas_region->atlas_texture);

    sprite_data->atlas_region_rect = &atlas_region->rect;

    return sprite_data;
}

void render_shape(Vector2 position, renderable_shape_t* shape_data)
{
    switch (shape_data->type)
    {
        case SHAPE_RECTANGLE:
            render_rectangle(position, (rect_data_t*)shape_data);
            break;

        case SHAPE_CIRCLE:
            render_circle(position, (circle_data_t*)shape_data);
            break;
        
        default:
            break;
    }
}

void render_rectangle(Vector2 position, rect_data_t* rect_data)
{
    DrawRectangle(position.x, position.y, rect_data->width, rect_data->height, rect_data->shape_base.base.color);
}

void render_circle(Vector2 position, circle_data_t* circle_data)
{
    DrawCircle(position.x + circle_data->radius, position.y + circle_data->radius, circle_data->radius, circle_data->shape_base.base.color);
}

void render_sprite(Vector2 position, sprite_data_t* sprite_data)
{
    if (sprite_data->atlas_region_rect)
    {
        DrawTextureRec(*sprite_data->texture, *sprite_data->atlas_region_rect, position, sprite_data->base.color);
    }
    else
    {
        DrawTexture(*sprite_data->texture, position.x, position.y, sprite_data->base.color);
    }
}