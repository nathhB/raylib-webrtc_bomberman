#ifndef FINITE_STATE_MACHINE_H_INCLUDED
#define FINITE_STATE_MACHINE_H_INCLUDED

#include <tdd.h>

typedef struct state state_t;
typedef struct finite_state_machine finite_state_machine_t;

typedef void (*state_hook_func_t)(finite_state_machine_t*, state_t*);

struct state
{
    const char*             name;
    void*                   data;
    bool                    free_data;
    list_t*                 transitions;
    state_hook_func_t       on_state_entered;
    state_hook_func_t       on_state_exited;
    state_hook_func_t       on_state_updated;
};

typedef bool (*state_transition_func_t)(void*, void*);

typedef struct state_transition
{
    state_t*                target_node;
    state_transition_func_t transition_func;
} state_transition_t;

struct finite_state_machine
{
    htable_t*   states;
    state_t*    current_state;
    void*       data;   
};

finite_state_machine_t* finite_state_machine_create(void*);
void                    finite_state_machine_destroy(finite_state_machine_t*);
state_t*                finite_state_machine_create_state(finite_state_machine_t*, const char*);
state_t*                finite_state_machine_create_state_with_data(finite_state_machine_t*, const char*, void*, bool);
void                    finite_state_machine_create_transition(finite_state_machine_t*, const char*, const char*, state_transition_func_t);
void                    finite_state_machine_update(finite_state_machine_t*);
void                    finite_state_machine_set_current_state(finite_state_machine_t*, const char*);

#endif // FINITE_STATE_MACHINE_H_INCLUDED