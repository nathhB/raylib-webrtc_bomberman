#include <stdlib.h>
#include "texture.h"

static void unload_texture(asset_t*);

asset_t* load_texture(const char* path)
{
    texture_asset_t* texture_asset = malloc(sizeof(texture_asset_t));

    texture_asset->base.unload_func = unload_texture;
    texture_asset->texture = LoadTexture(path);

    return (asset_t*)texture_asset;
}

static void unload_texture(asset_t* asset)
{
    texture_asset_t* texture_asset = (texture_asset_t*)asset;

    UnloadTexture(texture_asset->texture);
}