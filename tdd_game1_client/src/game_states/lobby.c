#include <stdlib.h>
#include <raylib.h>

#include "../game_state_machine.h"
#include "../game_states.h"
#include "../events.h"
#include "../components.h"
#include "../screen.h"
#include "../network.h"
#include "../player.h"
#include "../systems.h"
#include "../families.h"

static world_t*     create_lobby_world();
static void         on_player_ready(int, bool);
static void         render_ui();

static world_t*     lobby_world = NULL;
static tdd_timer_t* lobby_timer = NULL;
static bool         is_local_player_ready = false;
static bool         is_game_ready = false;
static unsigned int players_in_lobby_count = 0;
static unsigned int ready_players_mask = 0;

static Vector2 lobby_players_nicknames_positions[] = {
    (Vector2){70, SCREEN_HEIGHT - 70},
    (Vector2){262, SCREEN_HEIGHT - 70},
    (Vector2){453, SCREEN_HEIGHT - 70},
    (Vector2){641, SCREEN_HEIGHT - 70}};

void on_lobby_state_entered(finite_state_machine_t* game_state_machine, state_t* lobby_state)
{
    TraceLog(LOG_INFO, "Entered lobby state");

    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    lobby_timer = tdd_timer_create();
    lobby_world = create_lobby_world();
    players_in_lobby_count = game_state_machine_data.local_player_id;
    ready_players_mask = game_state_machine_data.ready_players_mask;

    tdd_timer_start(lobby_timer);
}

void on_lobby_state_exited(finite_state_machine_t* game_state_machine, state_t* lobby_state)
{
    TraceLog(LOG_INFO, "Exited lobby state");

    world_destroy(lobby_world);
    tdd_timer_destroy(lobby_timer);
}

void on_lobby_state_updated(finite_state_machine_t* game_state_machine, state_t* lobby_state)
{
    atlas_region_t* background_region = GET_TEXTURE_ATLAS_REGION("ui_atlas", "lobby_background");

    BeginDrawing();

    DrawTextureRec(*background_region->atlas_texture, background_region->rect, VEC2_ZERO, WHITE);

    if (!is_local_player_ready && IsKeyDown(KEY_R))
    {
        TraceLog(LOG_INFO, "Local player is ready");

        on_player_ready(((game_state_machine_data_t*)game_state_machine->data)->local_player_id, true);

        is_local_player_ready = true;
    }

    tdd_timer_update(lobby_timer);
    world_update(lobby_world, 0);

    render_ui();

    EndDrawing();
}

void lobby_state_handle_event(finite_state_machine_t* game_state_machine, state_t* lobby_state, int event_type, void* event_data)
{
    game_state_machine_data_t* game_state_machine_data = (game_state_machine_data_t*)game_state_machine->data;

    switch (event_type)
    {
    case EVENT_PLAYER_CONNECTED:;
        player_connected_event_t* connected_event = (player_connected_event_t*)event_data;

        TraceLog(LOG_INFO, "game_state_machine: lobby state received EVENT_PLAYER_CONNECTED event (player_id: %d, is_local: %d)",
                 connected_event->player_id, connected_event->is_local);

        game_state_machine_register_players_nicknames(connected_event->players_nicknames, connected_event->players_count);
        create_lobby_player_entity(lobby_world, connected_event->player_id, false);

        players_in_lobby_count++;
        break;
    
    case EVENT_PLAYER_READY:;
        player_ready_event_t* ready_event = (player_ready_event_t*)event_data;

        TraceLog(LOG_INFO, "game_state_machine: lobby state received EVENT_PLAYER_READY event (player_id: %d)", ready_event->player_id);

        on_player_ready(ready_event->player_id, false);
        break;

    case EVENT_GAME_READY:
        TraceLog(LOG_INFO, "game_state_machine: lobby state received EVENT_GAME_READY event");
        TraceLog(LOG_INFO, "First round will start in %d seconds...", NEXT_ROUND_DELAY_MS / 1000);

        is_game_ready = true;
        break;

    case EVENT_ROUND_START:
        TraceLog(LOG_INFO, "game_state_machine: lobby state received EVENT_ROUND_START event");

        game_state_machine_data->is_round_playing = true;
        break;
    }

    free(event_data);
}

static world_t* create_lobby_world()
{
    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();
    world_t* world = world_create();
    int ready_players_mask = game_state_machine_data.ready_players_mask;

    for (int player_id = 0; player_id <= game_state_machine_data.local_player_id; player_id++)
    {
        create_lobby_player_entity(world, player_id, B_IS_SET(ready_players_mask, player_id));
    }

    world_register_entity_system(world, get_transform_family(), update_transforms_entity_system);
    world_register_animate_entity_system(world, lobby_timer);
    world_register_entity_system(world, get_render_family(), render_entity_system);

    return world;
}

static void on_player_ready(int player_id, bool is_local_player)
{
    entity_t* local_lobby_player_entity = entity_tag_manager_get_entity(lobby_world->entity_tag_manager, get_player_entity_tag(player_id));
    animated_sprite_t* lobby_player_animated_sprite = (animated_sprite_t*)list_get_at(get_animated_of_entity(local_lobby_player_entity)->animated_sprites, 0);

    animated_set_sprite_current_animation(lobby_player_animated_sprite, "ready");

    B_SET(ready_players_mask, player_id);

    if (is_local_player)
    {
        TraceLog(LOG_INFO, "Send player ready net message (player_id: %d)", player_id);

        player_ready_net_message_t* ready_net_message = player_ready_net_message_create();

        ready_net_message->player_id = player_id;

        SEND_RELIABLE(ready_net_message);
    }
}

static void render_ui()
{
    Font *ui_font = GET_FONT("font");
    game_state_machine_data_t game_state_machine_data = game_state_machine_get_data();

    // draw player nicknames
    int lobby_bomberman_region_width = GET_TEXTURE_ATLAS_REGION("ui_atlas", "lobby_bomberman_white1")->rect.width;

    for (int i = 0; i <= players_in_lobby_count; i++)
    {
        int nickname_text_width = (int)MeasureTextEx(*ui_font, game_state_machine_data.nicknames[i], ui_font->baseSize, 2).x;
        Vector2 nickname_offset = (Vector2){(lobby_bomberman_region_width / 2) - nickname_text_width / 2, 5};
        Vector2 nickname_position = Vector2Add(lobby_players_nicknames_positions[i], nickname_offset);

        DrawTextEx(*ui_font, game_state_machine_data.nicknames[i], nickname_position, ui_font->baseSize, 2, WHITE);

        if (B_IS_SET(ready_players_mask, i))
        {
            int ready_text_width = (int)MeasureTextEx(*ui_font, "Ready", ui_font->baseSize, 2).x;
            DrawTextEx(*ui_font, "Ready", Vector2Add(nickname_position, (Vector2){ nickname_text_width / 2 - ready_text_width / 2, 30}), ui_font->baseSize, 2, GREEN);
        }
    }

    // draw ping
    char ping_text[16];

    snprintf(ping_text, 16, "Ping: %lu", network_get_ping());

    DrawTextEx(*ui_font, ping_text, (Vector2){SCREEN_WIDTH - 160, SCREEN_HEIGHT - 35}, ui_font->baseSize, 2, WHITE);

    if (is_game_ready)
    {
        // TODO: display round start count down
    }
}