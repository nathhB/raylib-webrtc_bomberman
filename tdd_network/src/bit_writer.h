#ifndef BITS_WRITER_H_INCLUDED
#define BITS_WRITER_H_INCLUDED

#include "tdd_network.h"

typedef struct bit_writer
{
    size_t              size;
    word_t*             buffer;
    uint64_t            scratch;
    unsigned int        scratchBitsCount;
    unsigned int        wordsCount;
} bit_writer_t;

bit_writer_t*   bit_writer_create(word_t*, size_t);
void            bit_writer_destroy(bit_writer_t*);
int             bit_writer_write(bit_writer_t*, word_t, unsigned int);
int             bit_writer_flush(bit_writer_t*);

#endif // BITS_WRITER_H_INCLUDED