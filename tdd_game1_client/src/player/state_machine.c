#include <raylib.h>
#include <tdd_game1.h>

#include "../components.h"
#include "../player.h"

typedef struct player_state_machine_data
{
    world_t*    world;
    entity_t*   player_entity;
} player_state_machine_data_t;

static Vector2 get_player_moving_direction(player_state_machine_data_t* state_machine_data)
{
    if (entity_tag_manager_is_entity_tagged(state_machine_data->world->entity_tag_manager, state_machine_data->player_entity, LOCAL_PLAYER_TAG))
    {
        movement_t* movement = get_movement_of_entity(state_machine_data->player_entity);

        return (Vector2){ movement->x_velocity, movement->y_velocity };
    }
    else
    {
        return get_remotely_controlled_of_entity(state_machine_data->player_entity)->moving_direction;
    }
}

// transitions...

static bool move_down_transition(void* state_machine_data, void* state_data)
{
    return get_player_moving_direction((player_state_machine_data_t*)state_machine_data).y > 0;
}

static bool move_up_transition(void* state_machine_data, void* state_data)
{
    return get_player_moving_direction((player_state_machine_data_t*)state_machine_data).y < 0;
}

static bool horizontal_idle_transition(void* state_machine_data, void* state_data)
{
    return (int)get_player_moving_direction((player_state_machine_data_t*)state_machine_data).y == 0;
}

static bool move_right_transition(void* state_machine_data, void* state_data)
{
    return get_player_moving_direction((player_state_machine_data_t*)state_machine_data).x > 0;
}

static bool move_left_transition(void* state_machine_data, void* state_data)
{
    return get_player_moving_direction((player_state_machine_data_t*)state_machine_data).x < 0;
}

static bool vertical_idle_transition(void* state_machine_data, void* state_data)
{
    return (int)get_player_moving_direction((player_state_machine_data_t*)state_machine_data).x == 0;
}

// state hooks...

static void set_player_animation(void* data, const char* animation_name)
{
    animated_t* animated = get_animated_of_entity(((player_state_machine_data_t*)data)->player_entity);

    animated_set_sprite_current_animation((animated_sprite_t*)list_get_at(animated->animated_sprites, 0), animation_name);
}

static void on_idle_down_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "idle_down");
}

static void on_idle_up_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "idle_up");
}

static void on_idle_right_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "idle_right");
}

static void on_idle_left_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "idle_left");
}

static void on_move_down_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "move_down");
}

static void on_move_up_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "move_up");
}

static void on_move_right_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "move_right");
}

static void on_move_left_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "move_left");
}

static void on_dead_state_entered(finite_state_machine_t* state_machine, state_t* state)
{
    set_player_animation(state_machine->data, "dead");

    /*movement_t* movement = get_movement_of_entity((entity_t*)state_machine->data);

    movement->x_velocity = 0;
    movement->y_velocity = 0;*/
}

void create_player_state_machine(world_t* world, entity_t* player_entity)
{
    player_state_machine_data_t* state_machine_data = malloc(sizeof(player_state_machine_data_t));

    state_machine_data->world = world;
    state_machine_data->player_entity = player_entity;

    finite_state_machine_t* state_machine = finite_state_machine_create(state_machine_data);

    // idle states
    state_t *idle_down_state = finite_state_machine_create_state(state_machine, "idle_down");
    state_t* idle_up_state = finite_state_machine_create_state(state_machine, "idle_up");
    state_t* idle_right_state = finite_state_machine_create_state(state_machine, "idle_right");
    state_t* idle_left_state = finite_state_machine_create_state(state_machine, "idle_left");

    idle_down_state->on_state_entered = on_idle_down_state_entered;
    idle_up_state->on_state_entered = on_idle_up_state_entered;
    idle_right_state->on_state_entered = on_idle_right_state_entered;
    idle_left_state->on_state_entered = on_idle_left_state_entered;

    // move states
    state_t* move_down_state = finite_state_machine_create_state(state_machine, "move_down");
    state_t* move_up_state = finite_state_machine_create_state(state_machine, "move_up");
    state_t *move_right_state = finite_state_machine_create_state(state_machine, "move_right");
    state_t *move_left_state = finite_state_machine_create_state(state_machine, "move_left");

    move_down_state->on_state_entered = on_move_down_state_entered;
    move_up_state->on_state_entered = on_move_up_state_entered;
    move_right_state->on_state_entered = on_move_right_state_entered;
    move_left_state->on_state_entered = on_move_left_state_entered;

    // dead state
    state_t* dead_state = finite_state_machine_create_state(state_machine, "dead");

    dead_state->on_state_entered = on_dead_state_entered;

    // idle to move transitions
    finite_state_machine_create_transition(state_machine, "idle_down", "move_down", move_down_transition);
    finite_state_machine_create_transition(state_machine, "idle_down", "move_up", move_up_transition);
    finite_state_machine_create_transition(state_machine, "idle_down", "move_left", move_left_transition);
    finite_state_machine_create_transition(state_machine, "idle_down", "move_right", move_right_transition);

    finite_state_machine_create_transition(state_machine, "idle_up", "move_down", move_down_transition);
    finite_state_machine_create_transition(state_machine, "idle_up", "move_up", move_up_transition);
    finite_state_machine_create_transition(state_machine, "idle_up", "move_left", move_left_transition);
    finite_state_machine_create_transition(state_machine, "idle_up", "move_right", move_right_transition);

    finite_state_machine_create_transition(state_machine, "idle_right", "move_down", move_down_transition);
    finite_state_machine_create_transition(state_machine, "idle_right", "move_up", move_up_transition);
    finite_state_machine_create_transition(state_machine, "idle_right", "move_left", move_left_transition);
    finite_state_machine_create_transition(state_machine, "idle_right", "move_right", move_right_transition);

    finite_state_machine_create_transition(state_machine, "idle_left", "move_down", move_down_transition);
    finite_state_machine_create_transition(state_machine, "idle_left", "move_up", move_up_transition);
    finite_state_machine_create_transition(state_machine, "idle_left", "move_left", move_left_transition);
    finite_state_machine_create_transition(state_machine, "idle_left", "move_right", move_right_transition);

    // move to move transitions
    finite_state_machine_create_transition(state_machine, "move_down", "move_up", move_up_transition);
    finite_state_machine_create_transition(state_machine, "move_up", "move_down", move_down_transition);
    finite_state_machine_create_transition(state_machine, "move_right", "move_left", move_left_transition);
    finite_state_machine_create_transition(state_machine, "move_left", "move_right", move_right_transition);

    // move to idle transitions
    finite_state_machine_create_transition(state_machine, "move_down", "idle_down", horizontal_idle_transition);
    finite_state_machine_create_transition(state_machine, "move_up", "idle_up", horizontal_idle_transition);
    finite_state_machine_create_transition(state_machine, "move_right", "idle_right", vertical_idle_transition);
    finite_state_machine_create_transition(state_machine, "move_left", "idle_left", vertical_idle_transition);

    state_machine->current_state = idle_down_state; // initial state

    add_state_machine_to_entity(player_entity, state_machine);
}