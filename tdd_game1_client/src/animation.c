#include <stdlib.h>
#include <stdarg.h>

#include "animation.h"

animation_t* animation_create(const char* atlas_name, unsigned int fps, bool loop, unsigned int frames_count, ...)
{
    animation_t* animation = malloc(sizeof(animation_t));

    animation->atlas_name = atlas_name;
    animation->fps = fps;
    animation->loop = loop;
    animation->started = false;
    animation->frames_count = frames_count;
    animation->frames = malloc(sizeof(const char*) * frames_count);
    animation->current_frame = 0;
    animation->last_frame_time = 0;

    va_list frames;

    va_start(frames, frames_count);

    for (int i = 0; i < frames_count; i++)
    {
        animation->frames[i] = va_arg(frames, const char*);
    }

    return animation;
}

void animation_destroy(animation_t* animation)
{
    free(animation->frames);
    free(animation);
}

void animation_reset(animation_t* animation)
{
    animation->current_frame = 0;
    animation->started = false;
}