#ifndef RENDERING_H_INCLUDED
#define RENDERING_H_INCLUDED

#include <raylib.h>
#include <tdd_game1.h>

#include "assets.h"
#include "transform.h"

typedef enum
{
    RENDERABLE_SHAPE,
    RENDERABLE_SPRITE
} renderable_type_t;

typedef enum
{
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE
} shape_type_t;

typedef struct renderable
{
    renderable_type_t   type;
    Color               color;
    transform_node_t*   transform_node;
    bool                is_rendering_enabled;
} renderable_t;

typedef struct renderable_shape
{
    renderable_t    base;
    shape_type_t    type;
} renderable_shape_t;

typedef struct rect_data
{
    renderable_shape_t  shape_base;
    unsigned int        width;
    unsigned int        height;
} rect_data_t;

typedef struct circle_data
{
    renderable_shape_t  shape_base;
    unsigned int        radius;
} circle_data_t;

typedef struct sprite_data
{
    renderable_t    base;
    Texture2D*      texture;
    Rectangle*      atlas_region_rect;
} sprite_data_t;

rect_data_t*    create_rectangle(int, int, Color);
circle_data_t*  create_circle(int, Color);
sprite_data_t*  create_sprite(Texture2D*);
sprite_data_t*  create_sprite_from_atlas(atlas_region_t*);

void            render_shape(Vector2, renderable_shape_t*);
void            render_rectangle(Vector2, rect_data_t*);
void            render_circle(Vector2, circle_data_t*);
void            render_sprite(Vector2, sprite_data_t*);

#endif // RENDERING_H_INCLUDED