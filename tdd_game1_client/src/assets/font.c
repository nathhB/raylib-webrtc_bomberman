#include <stdlib.h>
#include "font.h"

static void unload_font(asset_t*);

asset_t* load_font(const char* path)
{
    font_asset_t* font_asset = malloc(sizeof(font_asset_t));

    font_asset->base.unload_func = unload_font;
    font_asset->font = LoadFontEx(path, 32, 0, 255);

    return (asset_t*)font_asset;
}

static void unload_font(asset_t* asset)
{
    font_asset_t* font_asset = (font_asset_t*)asset;

    UnloadFont(font_asset->font);
}