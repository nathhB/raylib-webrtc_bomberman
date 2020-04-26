#include <raylib.h>
#include <tdd_ecs.h>

#include "../../../components.h"
#include "../../../network.h"
#include "../../../game_states.h"

static bool get_two_interpolation_states(circular_buffer_t*, long, interpolation_state_t**, interpolation_state_t**);

void position_interpolation_entity_system(entity_system_t* system, world_t* world, float delta, entity_t* entity)
{
    circular_buffer_t* buffer = get_interpolated_position_of_entity(entity)->buffer;
    interpolation_state_t* state1 = NULL;
    interpolation_state_t* state2 = NULL;
    tdd_timer_t* timer = ((game_state_data_t*)world->user_data)->timer;

    if (timer->elapsed_ms < INTERPOLATION_TIME_MS) return;

    unsigned long interp_time = timer->elapsed_ms - INTERPOLATION_TIME_MS;

    if (get_two_interpolation_states(buffer, interp_time, &state1, &state2))
    {
        long length = state2->time - state1->time;
        long window = interp_time - state1->time;
        float alpha = (length > 0) ? (float)window / length : 0;

        // printf("interp_time = %lu / L = %ld / W = %ld (s1: %p / s2: %p)\n", interp_time, length, window, state1, state2);
        // printf("(%f, %f) -> (%f, %f) a: %f\n", state1->position.x, state1->position.y, state2->position.x, state2->position.y, alpha);
        Vector2 position = Vector2Lerp(state1->position, state2->position, alpha);

        get_transform_of_entity(entity)->root->position = (Vector2) { (int)position.x, (int)position.y };

        get_remotely_controlled_of_entity(entity)->moving_direction =
            (state1->position.x == state2->position.x && state1->position.y == state2->position.y) ? VEC2_ZERO : Vector2Normalize(Vector2Subtract(state2->position, state1->position));
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to find 2 states to run interpolation between");
    }
    
}

static bool get_two_interpolation_states(circular_buffer_t* buffer, long interp_time, interpolation_state_t** state1, interpolation_state_t** state2)
{
    interpolation_state_t* _state1 = NULL;
    unsigned int state1_index = 0;

    for (unsigned int i = 0; i < buffer->capacity; i++)
    {
        interpolation_state_t* state = (interpolation_state_t*)buffer->internal_array[i];

        if (state && state->time <= interp_time)
        {
            if (_state1 == NULL || state->time > _state1->time)
            {
                _state1 = state;
                state1_index = i;
            }
        }
    }

    if (_state1)
    {
        interpolation_state_t *_state2 = NULL;

        for (unsigned int i = 0; i < buffer->capacity; i++)
        {
            interpolation_state_t *state = (interpolation_state_t *)buffer->internal_array[i];

            if (state && state->time > _state1->time)
            {
                if (_state2 == NULL || state->time < _state2->time)
                {
                    _state2 = state;
                }
            }
        }

        if (_state2)
        {
            *state1 = _state1;
            *state2 = _state2;

            return true;
        }
    }

    return false;
}