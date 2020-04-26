#ifndef BITS_READER_H_INCLUDED
#define BITS_READER_H_INCLUDED

#include "tdd_network.h"

typedef struct bit_reader
{
    size_t              size;
    word_t*             buffer;
    uint64_t            scratch;
    unsigned int        scratchBitsCount;
    unsigned int        wordsCount;
} bit_reader_t;

bit_reader_t*   bit_reader_create(word_t*, size_t);
void            bit_reader_destroy(bit_reader_t*);
int             bit_reader_read(bit_reader_t*, word_t*, unsigned int);

#endif // BITS_READER_H_INCLUDED
