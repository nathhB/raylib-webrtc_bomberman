#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include "tdd_network.h"

typedef struct stream stream_t;

typedef int (*serialize_uint_func_t)(stream_t*, unsigned int*, unsigned int, unsigned int);
typedef int (*serialize_int_func_t)(stream_t*, int*, int, int);
typedef int (*serialize_bool_func_t)(stream_t*, unsigned int*);
typedef int (*serialize_word_padding_func_t)(stream_t*);
typedef int (*serialize_byte_array_func_t)(stream_t*, uint8_t*, unsigned int);

typedef enum
{
    STREAM_WRITE,
    STREAM_READ,
    STREAM_MEASURE
} stream_type_t;

struct stream
{
    stream_type_t                   type;
    serialize_uint_func_t           serialize_uint_func;
    serialize_int_func_t            serialize_int_func;
    serialize_bool_func_t           serialize_bool_func;
    serialize_word_padding_func_t   serialize_word_padding_func;
    serialize_byte_array_func_t     serialize_byte_array_func;
};

#endif // STREAM_H_INCLUDED