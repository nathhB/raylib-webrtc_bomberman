#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <raylib.h>

#include "../assets_manager.h"

#define LOAD_FONT(path, name) \
{ \
    TraceLog(LOG_INFO, "Loading font... (path: %s)", path); \
    assets_manager_load_asset(ASSET_FONT, path, name); \
}

#define GET_FONT(name) (Font*)&(((font_asset_t*)assets_manager_get_asset(name))->font)

typedef struct font_asset
{
    asset_t     base;
    Font        font;
} font_asset_t;

asset_t* load_font(const char*);

#endif