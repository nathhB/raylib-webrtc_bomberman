#include <raylib.h>
#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../events.h"
#include "../../../player.h"
#include "../../../bomb.h"
#include "../../../physics.h"
#include "../../../game_states.h"

static void handle_local_player_picked_up_event(world_t*, void*);
static void add_extra_bomb_to_player(player_gear_t*);
static void expand_explosion_range(player_gear_t*, unsigned int);
static void increase_player_speed(player_gear_t*);
static void equip_indestructible_armor(player_gear_t*, entity_t*, unsigned long);
static void equip_wall_passer(player_gear_t*, entity_t*);
static void enable_player_kick(player_gear_t*);

void apply_bonuses_system(system_t* system, world_t* world, float delta)
{
    world_consume_events(world, EVENT_PLAYER_PICKED_UP_EVENT, handle_local_player_picked_up_event);
}

static void handle_local_player_picked_up_event(world_t* world, void* ev_data)
{
    player_picked_up_bonus_event_t* ev = (player_picked_up_bonus_event_t*)ev_data;
    bool is_local_player = ((game_state_data_t*)world->user_data)->local_player_id == ev->player_id;

    TraceLog(LOG_INFO, "Apply bonus %d to player %d", ev->bonus_type, ev->player_id);

    entity_t* player_entity = entity_tag_manager_get_entity(world->entity_tag_manager, is_local_player ? LOCAL_PLAYER_TAG : get_player_entity_tag(ev->player_id));
    player_gear_t* player_gear = get_player_gear_of_entity(player_entity);

    switch (ev->bonus_type)
    {
    case BONUS_EXTRA_BOMB:
        add_extra_bomb_to_player(player_gear);

        if (is_local_player)
        {
            // update ui
            unsigned int remaining_bombs_count = player_gear->bombs_count - entity_group_manager_get_group(world->entity_group_manager, LOCAL_PLAYER_BOMBS_GROUP)->count;

            create_ui_update_event(UI_MAX_BOMBS_COUNT, player_gear->bombs_count);
            create_ui_update_event(UI_REMAINING_BOMBS_COUNT, remaining_bombs_count);

            TraceLog(LOG_INFO, "Added one extra bomb to player %d (count: %d)", ev->player_id, player_gear->bombs_count);
        }
        break;

    case BONUS_EXPLOSION_EXPANDER:
        expand_explosion_range(player_gear, 1);

        if (is_local_player)
        {
            // update ui
            create_ui_update_event(UI_EXPLOSION_RANGE, player_gear->explosion_range);
        }

        TraceLog(LOG_INFO, "Expand explosion range of player %d (range: %d)", ev->player_id, player_gear->explosion_range);
        break;

    case BONUS_ACCELERATOR:
        increase_player_speed(player_gear);
        break;

    case BONUS_MAX_EXPLOSION:
        expand_explosion_range(player_gear, PLAYER_MAX_EXPLOSION_RANGE);

        if (is_local_player)
        {
            // update ui
            create_ui_update_event(UI_EXPLOSION_RANGE, player_gear->explosion_range);
        }

        TraceLog(LOG_INFO, "Expand explosion range of player %d (range: %d)", ev->player_id, player_gear->explosion_range);
        break;

    case BONUS_INDESTRUCTIBLE_ARMOR:
        equip_indestructible_armor(player_gear, player_entity, ((game_state_data_t*)world->user_data)->timer->elapsed_ms);

        TraceLog(LOG_INFO, "Player %d is now indestructible for %d ms", ev->player_id, PLAYER_ARMOR_TIME);
        break;

    case BONUS_WALL_PASSER:
        if (entity_tag_manager_is_entity_tagged(world->entity_tag_manager, player_entity, LOCAL_PLAYER_TAG))
        {
            equip_wall_passer(player_gear, player_entity);

            TraceLog(LOG_INFO, "Local player can now walk through destructible walls");
        }
        break;

    case BONUS_KICK:
        enable_player_kick(player_gear);

        TraceLog(LOG_INFO, "Player %d can now kick bombs", ev->player_id);
        break;
    
    default:
        break;
    }
}

void add_extra_bomb_to_player(player_gear_t* player_gear)
{
    player_gear->bombs_count = MIN(player_gear->bombs_count + 1, PLAYER_MAX_BOMBS_COUNT);
}

void expand_explosion_range(player_gear_t* player_gear, unsigned int added_range)
{
    player_gear->explosion_range = MIN(player_gear->explosion_range + added_range, PLAYER_MAX_EXPLOSION_RANGE);
}

static void increase_player_speed(player_gear_t* player_gear)
{
    player_gear->speed = MIN(player_gear->speed + 1, PLAYER_MAX_SPEED);
}

static void equip_indestructible_armor(player_gear_t* player_gear, entity_t* player_entity, unsigned long equip_time)
{
    if (!player_gear->is_indestructible)
    {
        add_blink_rendering_to_entity(player_entity, 4);
    }

    player_gear->is_indestructible = true;
    player_gear->armor_equip_time = equip_time;
}

static void equip_wall_passer(player_gear_t* player_gear, entity_t* player_entity)
{
    ((collider_t*)list_get_at(get_physics_of_entity(player_entity)->colliders, 0))->collision_mask &= ~COLLIDER_DESTRUCTIBLE_WALL;
    player_gear->can_pass_through_walls = true;
}

static void enable_player_kick(player_gear_t* player_gear)
{
    player_gear->can_kick = true;
}