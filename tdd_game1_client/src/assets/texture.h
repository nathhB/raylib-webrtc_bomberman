#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <raylib.h>

#include "../assets_manager.h"

#define LOAD_TEXTURE(path, name) \
{ \
    TraceLog(LOG_INFO, "Loading texture... (path: %s)", path); \
    assets_manager_load_asset(ASSET_TEXTURE, path, name); \
}

#define GET_TEXTURE(name) (Texture2D*)&(((texture_asset_t*)assets_manager_get_asset(name))->texture)

typedef struct texture_asset
{
    asset_t     base;
    Texture2D   texture;
} texture_asset_t;

asset_t* load_texture(const char*);

#endif // TEXTURE_H_INCLUDED