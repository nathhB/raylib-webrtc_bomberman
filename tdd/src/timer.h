#ifndef TDD_TIMER_H_INCLUDED
#define TDD_TIMER_H_INCLUDED

typedef struct tdd_timer
{
    unsigned long   elapsed_ms;
    unsigned long   current_ms;
    bool            running;
    void*           data;
    list_t*         hooks;
} tdd_timer_t;

typedef void (*tdd_timer_hook_func_t)(void*);

typedef enum
{
    TDD_TIMER_WAIT,
    TDD_TIMER_REPEAT
} tdd_timer_hook_type_t;

typedef struct tdd_timer_hook
{
    tdd_timer_hook_type_t   type;
    tdd_timer_hook_func_t   hook_func;
    unsigned long           ms;
    unsigned long           last_trigger_time;
    bool                    enabled;
} tdd_timer_hook_t;

tdd_timer_t*    tdd_timer_create();
void            tdd_timer_destroy(tdd_timer_t*);
void            tdd_timer_start(tdd_timer_t*);
void            tdd_timer_stop(tdd_timer_t*);
void            tdd_timer_update(tdd_timer_t*);
void            tdd_timer_wait(tdd_timer_t*, tdd_timer_hook_func_t, unsigned long);
void            tdd_timer_repeat(tdd_timer_t*, tdd_timer_hook_func_t, unsigned long);

#endif // TDD_TIMER_H_INCLUDED