#ifndef TEXTURE_ATLAS_H_INCLUDED
#define TEXTURE_ATLAS_H_INCLUDED

#include <raylib.h>

#include "../assets_manager.h"

#define LOAD_TEXTURE_ATLAS(path, name) \
{ \
    TraceLog(LOG_INFO, "Loading texture atlas... (path: %s)", path); \
    assets_manager_load_asset(ASSET_TEXTURE_ATLAS, path, name); \
}

#define GET_TEXTURE_ATLAS(atlas_name) ((texture_atlas_asset_t*)assets_manager_get_asset(atlas_name))
#define GET_TEXTURE_ATLAS_REGION(atlas_name, region_name) ((atlas_region_t*)htable_get(GET_TEXTURE_ATLAS(atlas_name)->regions, region_name))

typedef struct texture_atlas_asset
{
    asset_t         base;
    Texture2D       texture;
    htable_t*       regions;
} texture_atlas_asset_t;

typedef struct atlas_region
{
    Texture2D*  atlas_texture;
    Rectangle   rect;
} atlas_region_t;

asset_t* load_texture_atlas(const char*);

#endif // TEXTURE_ATLAS_H_INCLUDED