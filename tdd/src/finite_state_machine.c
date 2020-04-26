#include "finite_state_machine.h"

static void                 execute_state_transition(finite_state_machine_t*, state_transition_t*);
static state_transition_t*  get_state_transition(finite_state_machine_t*, state_t*);
static void                 clear_state(void**);
static void                 clear_state_transition(void**);
static void                 set_current_state(finite_state_machine_t*, state_t*);

finite_state_machine_t* finite_state_machine_create(void* data)
{
    finite_state_machine_t* state_machine = malloc(sizeof(finite_state_machine_t));

    state_machine->states = htable_create();
    state_machine->current_state = NULL;
    state_machine->data = data;

    return state_machine;
}

void finite_state_machine_destroy(finite_state_machine_t* state_machine)
{
    htable_destroy(&state_machine->states, true, clear_state, false);
    free(state_machine);
}

state_t* finite_state_machine_create_state(finite_state_machine_t* state_machine, const char* name)
{
    return finite_state_machine_create_state_with_data(state_machine, name, NULL, false);
}

state_t* finite_state_machine_create_state_with_data(finite_state_machine_t* state_machine, const char* name, void* data, bool free_data)
{
    state_t *state = malloc(sizeof(state_t));

    state->name = name;
    state->data = data;
    state->free_data = free_data;
    state->transitions = list_create();
    state->on_state_entered = NULL;
    state->on_state_exited = NULL;
    state->on_state_updated = NULL;

    htable_add(state_machine->states, name, state);

    return state;
}

void finite_state_machine_create_transition(
    finite_state_machine_t* state_machine, const char* start_state_name, const char* target_state_name, state_transition_func_t transition_func)
{
    state_transition_t* transition = malloc(sizeof(state_transition_t));

    transition->target_node = htable_get(state_machine->states, target_state_name);
    transition->transition_func = transition_func;

    list_push_back(((state_t*)htable_get(state_machine->states, start_state_name))->transitions, transition);
}

void finite_state_machine_update(finite_state_machine_t* state_machine)
{
    state_transition_t* transition = get_state_transition(state_machine, state_machine->current_state);

    if (transition)
    {
        execute_state_transition(state_machine, transition);
    }

    state_t* current_state = state_machine->current_state;

    if (current_state->on_state_updated)
    {
        current_state->on_state_updated(state_machine, current_state);
    }
}

void finite_state_machine_set_current_state(finite_state_machine_t* state_machine, const char* state_name)
{
    set_current_state(state_machine, htable_get(state_machine->states, state_name));
}

static void set_current_state(finite_state_machine_t* state_machine, state_t* state)
{
    if (state_machine->current_state->on_state_exited)
    {
        state_machine->current_state->on_state_exited(state_machine, state_machine->current_state);
    }

    state_machine->current_state = state;

    if (state_machine->current_state->on_state_entered)
    {
        state_machine->current_state->on_state_entered(state_machine, state_machine->current_state);
    }
}

static void execute_state_transition(finite_state_machine_t* state_machine, state_transition_t* transition)
{
    set_current_state(state_machine, transition->target_node);
}

static state_transition_t* get_state_transition(finite_state_machine_t* state_machine, state_t* state)
{
    list_node_t* current_node = state->transitions->head;

    while (current_node != NULL)
    {
        state_transition_t* state_transition = (state_transition_t*)current_node->data;

        if (state_transition->transition_func(state_machine->data, state->data))
        {
            return state_transition;
        }

        current_node = current_node->next;
    }

    return NULL;
}

static void clear_state(void** state_ptr)
{
    state_t* state = (state_t*)*state_ptr;

    list_destroy(&state->transitions, true, clear_state_transition);

    if (state->data && state->free_data)
    {
        free(state->data);
    }

    free(state);
}

static void clear_state_transition(void** state_transition_ptr)
{
    free(*state_transition_ptr);
}