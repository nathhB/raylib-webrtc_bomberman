#ifndef SERIALIZATION_H_INCLUDED
#define SERIALIZATION_H_INCLUDED

#include "tdd_network.h"

#define ASSERT_VALUE_IN_RANGE(v, min, max) assert(v >= min && v <= max)
#define ASSERTED_SERIALIZE(v, min, max, code) \
{ \
    if (stream->type == STREAM_WRITE) ASSERT_VALUE_IN_RANGE(v, min, max); \
    if (code < 0) return -1; \
    if (stream->type == STREAM_READ) ASSERT_VALUE_IN_RANGE(v, min, max); \
}

#define SERIALIZE_UINT(v, min, max) ASSERTED_SERIALIZE(v, min, max, stream->serialize_uint_func(stream, (unsigned int*)&(v), min, max))
#define SERIALIZE_INT(v, min, max) ASSERTED_SERIALIZE(v, min, max, stream->serialize_int_func(stream, &(v), min, max))
#define SERIALIZE_BOOL(v) ASSERTED_SERIALIZE(v, 0, 1, stream->serialize_bool_func(stream, &(v)))
#define SERIALIZE_STRING(v, length) stream->serialize_byte_array_func(stream, (uint8_t*)v, length)

#endif // SERIALIZATION_H_INCLUDED