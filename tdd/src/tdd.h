#ifndef TDD_H_INCLUDED
#define TDD_H_INCLUDED

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ABS(v) ((v)<0?-(v):(v))

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "bag.h"
#include "list.h"
#include "htable.h"
#include "bits.h"
#include "timer.h"
#include "circular_buffer.h"
#include "finite_state_machine.h"

#endif // TDD_H_INCLUDED