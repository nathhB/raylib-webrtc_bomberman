#include <stdlib.h>
#include <tdd.h>

#include "assets_manager.h"

static assets_manager_t* assets_manager = NULL;

void assets_manager_init()
{
    assets_manager = malloc(sizeof(assets_manager_t));

    assets_manager->loaders = bag_create(MAX_ASSET_TYPES);
    assets_manager->assets = htable_create();
}

void assets_manager_register_loader(asset_loader_func_t load_func, unsigned int asset_type)
{
    bag_set_at(assets_manager->loaders, asset_type, load_func);
}

void assets_manager_load_asset(unsigned int asset_type, const char* path, const char* name)
{
    asset_loader_func_t loader = bag_get_at(assets_manager->loaders, asset_type);

    htable_add(assets_manager->assets, name, loader(path));
}

void assets_manager_unload_asset(const char* name)
{
    asset_t* asset = (asset_t*)htable_remove(assets_manager->assets, name);

    if (asset)
    {
        asset->unload_func(asset);

        free(asset);
    }
}

asset_t* assets_manager_get_asset(const char* name)
{
    return htable_get(assets_manager->assets, name);
}