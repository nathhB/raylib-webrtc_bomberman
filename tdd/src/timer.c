#include "tdd.h"

static void handle_hooks(tdd_timer_t*);
static void handle_wait_hook(tdd_timer_t*, tdd_timer_hook_t*);
static void handle_repeat_hook(tdd_timer_t* timer, tdd_timer_hook_t* hook);
static void clear_timer_hook(void**);

tdd_timer_t* tdd_timer_create()
{
    tdd_timer_t* timer = malloc(sizeof(tdd_timer_t));

    timer->elapsed_ms = 0;
    timer->current_ms = 0;
    timer->running = false;
    timer->data = NULL;
    timer->hooks = list_create();

    return timer;
}

void tdd_timer_destroy(tdd_timer_t* timer)
{
    list_destroy(&timer->hooks, true, clear_timer_hook);
    free(timer);
}

void tdd_timer_start(tdd_timer_t* timer)
{
    timer->running = true;
}

void tdd_timer_stop(tdd_timer_t* timer)
{
    timer->current_ms = 0;
    timer->running = false;
}

void tdd_timer_update(tdd_timer_t* timer)
{
    if (!timer->running) return;

    struct timespec spec;

    clock_gettime(CLOCK_MONOTONIC, &spec);

    unsigned long ms = round(spec.tv_nsec / 1.0e6);

    if (ms < timer->current_ms) // handle wrap at 1000
    {
        unsigned int wrap_amount = (1000 - timer->current_ms) + ms;

        timer->elapsed_ms += wrap_amount;
    }
    else
    {
        timer->elapsed_ms += (ms - timer->current_ms);    
    }

    timer->current_ms = ms;
    
    handle_hooks(timer);
}

void tdd_timer_wait(tdd_timer_t* timer, tdd_timer_hook_func_t hook_func, unsigned long ms)
{
    tdd_timer_hook_t* hook = malloc(sizeof(tdd_timer_hook_t));

    hook->type = TDD_TIMER_WAIT;
    hook->hook_func = hook_func;
    hook->ms = timer->elapsed_ms + ms;
    hook->enabled = true;

    list_push_back(timer->hooks, hook);
}

void tdd_timer_repeat(tdd_timer_t* timer, tdd_timer_hook_func_t hook_func, unsigned long ms)
{
    tdd_timer_hook_t *hook = malloc(sizeof(tdd_timer_hook_t));

    hook->type = TDD_TIMER_REPEAT;
    hook->hook_func = hook_func;
    hook->ms = timer->elapsed_ms + ms;
    hook->last_trigger_time = 0;
    hook->enabled = true;

    list_push_back(timer->hooks, hook);
}

static void handle_hooks(tdd_timer_t* timer)
{
    list_node_t* current_node = timer->hooks->head;

    while (current_node)
    {
        tdd_timer_hook_t* hook = (tdd_timer_hook_t*)current_node->data;

        switch (hook->type)
        {
        case TDD_TIMER_WAIT:
            handle_wait_hook(timer, hook);
            break;

        case TDD_TIMER_REPEAT:
            handle_repeat_hook(timer, hook);
            break;
        
        default:
            break;
        }

        current_node = current_node->next;
    }
}

static void handle_wait_hook(tdd_timer_t* timer, tdd_timer_hook_t* hook)
{
    if (hook->enabled && timer->elapsed_ms >= hook->ms)
    {
        hook->hook_func(timer->data);
        hook->enabled = false;
    }
}

static void handle_repeat_hook(tdd_timer_t* timer, tdd_timer_hook_t* hook)
{
    if (hook->enabled && timer->elapsed_ms - hook->last_trigger_time >= hook->ms)
    {
        hook->hook_func(timer->data);
        hook->last_trigger_time = timer->elapsed_ms;
    }
}

static void clear_timer_hook(void** hook_ptr)
{
    free(*hook_ptr);
}