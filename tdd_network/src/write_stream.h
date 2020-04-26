#ifndef WRITE_STREAM_H_INCLUDED
#define WRITE_STREAM_H_INCLUDED

#include "tdd_network.h"

typedef struct write_stream
{
    stream_t        base;
    bit_writer_t*   b_writer;
} write_stream_t;

write_stream_t* write_stream_create(unsigned char*, size_t);
void            write_stream_destroy(write_stream_t*);
int             write_stream_serialize_uint(write_stream_t*, unsigned int*, unsigned int, unsigned int);
int             write_stream_serialize_int(write_stream_t*, int*, int, int);
int             write_stream_serialize_bool(write_stream_t*, unsigned int*);
int             write_stream_serialize_word_padding(write_stream_t*);
int             write_stream_serialize_byte_array(write_stream_t*, uint8_t*, unsigned int);
int             write_stream_flush(write_stream_t*);

#endif // WRITE_STREAM_H_INCLUDED