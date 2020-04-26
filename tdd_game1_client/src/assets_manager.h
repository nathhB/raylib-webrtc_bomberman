#ifndef ASSETS_MANAGER_H_INCLUDED
#define ASSETS_MANAGER_H_INCLUDED

#include <tdd.h>

#define MAX_ASSET_TYPES 16

#define UNLOAD_ASSET(name) assets_manager_unload_asset(name);

typedef struct asset asset_t;

typedef asset_t*    (*asset_loader_func_t)(const char*);
typedef void        (*asset_unloader_func_t)(asset_t*);

struct asset
{
    asset_unloader_func_t unload_func;
};

typedef struct assets_manager
{
    bag_t*      loaders;
    htable_t*   assets;
} assets_manager_t;

static assets_manager_t* assets_manager;

void        assets_manager_init();
void        assets_manager_register_loader(asset_loader_func_t, unsigned int);
void        assets_manager_load_asset(unsigned int, const char*, const char*);
void        assets_manager_unload_asset(const char*);
asset_t*    assets_manager_get_asset(const char*);

#endif // ASSETS_MANAGER_H_INCLUDED