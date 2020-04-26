#ifndef READ_STREAM_H_INCLUDED
#define READ_STREAM_H_INCLUDED

#include "tdd_network.h"

typedef struct read_stream
{
    stream_t        base;
    bit_reader_t*   b_reader;
} read_stream_t;

read_stream_t*  read_stream_create(unsigned char*, size_t);
void            read_stream_destroy(read_stream_t*);
int             read_stream_serialize_uint(read_stream_t*, unsigned int*, unsigned int, unsigned int);
int             read_stream_serialize_int(read_stream_t*, int*, int, int);
int             read_stream_serialize_bool(read_stream_t*, unsigned int*);
int             read_stream_serialize_word_padding(read_stream_t*);
int             read_stream_serialize_byte_array(read_stream_t*, uint8_t*, unsigned int);

#endif // READ_STREAM_H_INCLUDED