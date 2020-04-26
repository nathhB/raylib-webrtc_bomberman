#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <stdbool.h>

typedef struct animation
{
    const char*     atlas_name;
    const char**    frames;
    unsigned int    fps;
    unsigned int    current_frame;
    unsigned int    frames_count;
    bool            loop;
    bool            started;
    unsigned long   last_frame_time;
} animation_t;

animation_t*    animation_create(const char*, unsigned int, bool, unsigned int, ...);
void            animation_destroy(animation_t*);
void            animation_reset(animation_t*);

#endif // ANIMATION_H_INCLUDED