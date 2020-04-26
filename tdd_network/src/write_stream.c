#include <string.h>
#include "tdd_network.h"

write_stream_t* write_stream_create(unsigned char* buffer, size_t size)
{
    write_stream_t* write_stream = malloc(sizeof(write_stream_t));
    
    write_stream->base.type = STREAM_WRITE;
    write_stream->base.serialize_uint_func = (serialize_uint_func_t)write_stream_serialize_uint;
    write_stream->base.serialize_int_func = (serialize_int_func_t)write_stream_serialize_int;
    write_stream->base.serialize_bool_func = (serialize_bool_func_t)write_stream_serialize_bool;
    write_stream->base.serialize_word_padding_func = (serialize_word_padding_func_t)write_stream_serialize_word_padding;
    write_stream->base.serialize_byte_array_func = (serialize_byte_array_func_t)write_stream_serialize_byte_array;

    write_stream->b_writer = bit_writer_create((word_t*)buffer, size / (WORD_SIZE / 8));

    return write_stream;
}

void write_stream_destroy(write_stream_t* write_stream)
{
    bit_writer_destroy(write_stream->b_writer);
    free(write_stream);
}

int write_stream_serialize_uint(write_stream_t* write_stream, unsigned int* value, unsigned int min, unsigned int max)
{
    assert(min <= max);
    assert(*value >= min && *value <= max);

    if (bit_writer_write(write_stream->b_writer, *value - min, BITS_REQUIRED(min, max)) < 0)
    {
        return -1;
    }

    return 0;
}

int write_stream_serialize_int(write_stream_t* write_stream, int* value, int min, int max)
{
    assert(min <= max);

    unsigned int isNegative = 0;
    unsigned int abs_min = MIN(abs(min), abs(max));
    unsigned int abs_max = MAX(abs(min), abs(max));

    isNegative = *value < 0;
    *value = abs(*value);

    if (write_stream_serialize_bool(write_stream, &isNegative) < 0) return -1;
    if (write_stream_serialize_uint(write_stream, (unsigned int*)value, (min < 0 && max > 0) ? 0 : abs_min, abs_max) < 0) return -1;
    if (isNegative) *value *= -1;

    return 0;
}

int write_stream_serialize_bool(write_stream_t* write_stream, unsigned int* value)
{
    return write_stream_serialize_uint(write_stream, value, 0, 1);
}

int write_stream_serialize_word_padding(write_stream_t* write_stream)
{
    if (write_stream->b_writer->scratchBitsCount == 0) // we are at the beginning of a new word, no need to pad
    {
        return 0;
    }

    unsigned int padding = WORD_SIZE - write_stream->b_writer->scratchBitsCount;

    return bit_writer_write(write_stream->b_writer, 0, padding);
}

int write_stream_serialize_byte_array(write_stream_t* write_stream, uint8_t* bytes, unsigned int length)
{
    if (write_stream_serialize_word_padding(write_stream) < 0)
    {
        return -1;
    }

    memcpy((unsigned char*)(write_stream->b_writer->buffer + write_stream->b_writer->wordsCount), bytes, length);

    unsigned int number_of_words = length / (WORD_SIZE / 8) + 1;

    write_stream->b_writer->wordsCount += number_of_words;

    return 0;
}

int write_stream_flush(write_stream_t* write_stream)
{
    return bit_writer_flush(write_stream->b_writer);
}