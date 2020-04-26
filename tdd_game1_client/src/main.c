#include <tdd_game1.h>
#include <emscripten/emscripten.h>

#include "assets_manager.h"
#include "assets.h"
#include "game_state_machine.h"
#include "network.h"
#include "map.h"
#include "screen.h"

static void update_and_draw()
{
    process_pending_packets(client_dequeue_packet);
    game_state_machine_update();

    if (game_state_machine_get_data().is_connected)
    {
        update_network_timer();
    }
}

static void load_assets()
{
    assets_manager_init();
    assets_manager_register_loader(load_texture, ASSET_TEXTURE);
    assets_manager_register_loader(load_texture_atlas, ASSET_TEXTURE_ATLAS);
    assets_manager_register_loader(load_font, ASSET_FONT);

    LOAD_TEXTURE_ATLAS("spritesheets/bomberman.rtpa", "bomberman_atlas")
    LOAD_TEXTURE_ATLAS("spritesheets/props.rtpa", "props_atlas")
    LOAD_TEXTURE_ATLAS("spritesheets/ui.rtpa", "ui_atlas")
    LOAD_FONT("fonts/font.ttf", "font")
}

static void unload_assets()
{
    UNLOAD_ASSET("bomberman_atlas")
    UNLOAD_ASSET("props_atlas")
    UNLOAD_ASSET("font")
}

int main(void)
{
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "tdd_game1");

    load_assets();
    create_game_state_machine();
    init_client_network();
    map_save();

    emscripten_set_main_loop(update_and_draw, 60, 1);

    unload_assets();

    destroy_client_network();
    destroy_game_state_machine();

    CloseWindow();

    return 0;
}