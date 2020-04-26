#include "tdd_network.h"

measure_stream_t* measure_stream_create()
{
    measure_stream_t* measure_stream = malloc(sizeof(measure_stream_t));

    measure_stream->base.type = STREAM_MEASURE;
    measure_stream->base.serialize_uint_func = (serialize_uint_func_t)measure_stream_serialize_uint;
    measure_stream->base.serialize_int_func = (serialize_int_func_t)measure_stream_serialize_int;
    measure_stream->base.serialize_bool_func = (serialize_bool_func_t)measure_stream_serialize_bool;
    measure_stream->base.serialize_word_padding_func = (serialize_word_padding_func_t)measure_stream_serialize_word_padding;
    measure_stream->base.serialize_byte_array_func = (serialize_byte_array_func_t)measure_stream_serialize_byte_array;

    measure_stream->number_of_bits = 0;

    return measure_stream;
}

void measure_stream_destroy(measure_stream_t* measure_stream)
{
    free(measure_stream);
}

int measure_stream_serialize_uint(measure_stream_t* measure_stream, unsigned int* value, unsigned int min, unsigned int max)
{
    assert(min <= max);
    assert(*value >= min && *value <= max);

    unsigned int number_of_bits = BITS_REQUIRED(min, max);

    measure_stream->number_of_bits += number_of_bits;

    return number_of_bits;
}

int measure_stream_serialize_int(measure_stream_t* measure_stream, int* value, int min, int max)
{
    assert(min <= max);
    assert(*value >= min && *value <= max);

    unsigned int abs_min = MIN(abs(min), abs(max));
    unsigned int abs_max = MAX(abs(min), abs(max));
    unsigned int abs_value = abs(*value);
    unsigned number_of_bits = measure_stream_serialize_uint(measure_stream, &abs_value, (min < 0 && max > 0) ? 0 : abs_min, abs_max);

    measure_stream->number_of_bits++; // +1 for int sign

    return number_of_bits + 1;
}

int measure_stream_serialize_bool(measure_stream_t* measure_stream, unsigned int* value)
{
    measure_stream->number_of_bits++;

    return 1;
}

int measure_stream_serialize_word_padding(measure_stream_t* measure_stream)
{
    unsigned int offset = measure_stream->number_of_bits % WORD_SIZE;

    if (offset == 0)
    {
        return 0;
    }

    unsigned int padding = WORD_SIZE - offset;

    measure_stream->number_of_bits += padding;

    return padding;
}

int measure_stream_serialize_byte_array(measure_stream_t* measure_stream, uint8_t* bytes, unsigned int length)
{
    unsigned int number_of_bits = measure_stream_serialize_word_padding(measure_stream) + length * 8;

    measure_stream->number_of_bits += number_of_bits;

    return number_of_bits;
}

void measure_stream_reset(measure_stream_t* measure_stream)
{
    measure_stream->number_of_bits = 0;
}