#include "tdd_network.h"

static int flushScratchBitsToBuffer(bit_writer_t*, unsigned int);

bit_writer_t* bit_writer_create(word_t* buffer, size_t size)
{
    bit_writer_t* bit_writer = malloc(sizeof(bit_writer_t));

    bit_writer->size = size;
    bit_writer->buffer = buffer;
    bit_writer->scratch = 0;
    bit_writer->scratchBitsCount = 0;
    bit_writer->wordsCount = 0;

    return bit_writer;
}

void bit_writer_destroy(bit_writer_t* bit_writer)
{
    free(bit_writer);
}

int bit_writer_write(bit_writer_t* bit_writer, word_t value, unsigned int numberOfBits)
{
    bit_writer->scratch |= ((uint64_t)value << bit_writer->scratchBitsCount);

    if ((bit_writer->scratchBitsCount += numberOfBits) >= WORD_SIZE)
    {
        return flushScratchBitsToBuffer(bit_writer, WORD_SIZE);
    }
    return 0;
}

int bit_writer_flush(bit_writer_t* bit_writer)
{
    return flushScratchBitsToBuffer(bit_writer, bit_writer->scratchBitsCount);
}

static int flushScratchBitsToBuffer(bit_writer_t* bit_writer, unsigned int numberOfBits)
{
    if (bit_writer->scratchBitsCount < 1) return 0;
    if (bit_writer->wordsCount >= bit_writer->size) return -1;

    bit_writer->buffer[bit_writer->wordsCount] = 0 | (bit_writer->scratch & (((uint64_t)1 << numberOfBits) - 1));
    bit_writer->scratch >>= numberOfBits;
    bit_writer->scratchBitsCount -= numberOfBits;
    bit_writer->wordsCount++;
    return 0;
}
