#ifndef MEASURE_STREAM_H_INCLUDED
#define MEASURE_STREAM_H_INCLUDED

#include "tdd_network.h"

typedef struct measure_stream
{
    stream_t    base;
    size_t      number_of_bits;
} measure_stream_t;

measure_stream_t*   measure_stream_create();
void                measure_stream_destroy(measure_stream_t*);
int                 measure_stream_serialize_uint(measure_stream_t*, unsigned int*, unsigned int, unsigned int);
int                 measure_stream_serialize_int(measure_stream_t*, int*, int, int);
int                 measure_stream_serialize_bool(measure_stream_t*, unsigned int*);
int                 measure_stream_serialize_word_padding(measure_stream_t*);
int                 measure_stream_serialize_byte_array(measure_stream_t*, uint8_t*, unsigned int);
void                measure_stream_reset(measure_stream_t*);

#endif // MEASURE_STREAM_H_INCLUDED