#include <string.h>
#include "tdd_network.h"

read_stream_t* read_stream_create(unsigned char* buffer, size_t size)
{
    read_stream_t* read_stream = malloc(sizeof(read_stream_t));

    read_stream->base.type = STREAM_READ;
    read_stream->base.serialize_uint_func = (serialize_uint_func_t)read_stream_serialize_uint;
    read_stream->base.serialize_int_func = (serialize_int_func_t)read_stream_serialize_int;
    read_stream->base.serialize_bool_func = (serialize_bool_func_t)read_stream_serialize_bool;
    read_stream->base.serialize_word_padding_func = (serialize_word_padding_func_t)read_stream_serialize_word_padding;
    read_stream->base.serialize_byte_array_func = (serialize_byte_array_func_t)read_stream_serialize_byte_array;

    read_stream->b_reader = bit_reader_create((word_t*)buffer, size / (WORD_SIZE / 8));

    return read_stream;
}

void read_stream_destroy(read_stream_t* read_stream)
{
    bit_reader_destroy(read_stream->b_reader);
    free(read_stream);
}

int read_stream_serialize_uint(read_stream_t* read_stream, unsigned int* value, unsigned int min, unsigned int max)
{
    assert(min <= max);

    if (bit_reader_read(read_stream->b_reader, value, BITS_REQUIRED(min, max)) < 0)
    {
        return -1;
    }

    *value += min;

    assert(*value >= min && *value <= max);
    return 0;
}

int read_stream_serialize_int(read_stream_t* read_stream, int* value, int min, int max)
{
    assert(min <= max);

    unsigned int isNegative = 0;
    unsigned int abs_min = MIN(abs(min), abs(max));
    unsigned int abs_max = MAX(abs(min), abs(max));

    isNegative = *value < 0;
    *value = abs(*value);

    if (read_stream_serialize_bool(read_stream, &isNegative) < 0) return -1;
    if (read_stream_serialize_uint(read_stream, (unsigned int*)value, (min < 0 && max > 0) ? 0 : abs_min, abs_max) < 0) return -1;
    if (isNegative) *value *= -1;

    return 0;
}

int read_stream_serialize_bool(read_stream_t* read_stream, unsigned int* value)
{
    return read_stream_serialize_uint(read_stream, (unsigned int*)value, 0, 1);
}

int read_stream_serialize_word_padding(read_stream_t* read_stream)
{
    if (read_stream->b_reader->scratchBitsCount == 0) // we are at the beginning of a new word, no need to pad
    {
        return 0;
    }

    unsigned int padding = read_stream->b_reader->scratchBitsCount;
    word_t value;

    return bit_reader_read(read_stream->b_reader, &value, padding);
}

int read_stream_serialize_byte_array(read_stream_t* read_stream, uint8_t* bytes, unsigned int length)
{
    if (read_stream_serialize_word_padding(read_stream) < 0)
    {
        return -1;
    }

    memcpy(bytes, (unsigned char*)(read_stream->b_reader->buffer + read_stream->b_reader->wordsCount), length);

    unsigned int number_of_words = length / (WORD_SIZE / 8) + 1;

    read_stream->b_reader->wordsCount += number_of_words;

    return 0;
}