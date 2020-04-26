#include "tdd_network.h"

static int getNextWordFromBuffer(bit_reader_t*);

bit_reader_t* bit_reader_create(word_t* buffer, size_t size)
{
    bit_reader_t* bit_reader = malloc(sizeof(bit_reader_t));

    bit_reader->size = size;
    bit_reader->buffer = buffer;
    bit_reader->scratch = 0;
    bit_reader->scratchBitsCount = 0;
    bit_reader->wordsCount = 0;

    return bit_reader;
}

void bit_reader_destroy(bit_reader_t* bit_reader)
{
    free(bit_reader);
}

int bit_reader_read(bit_reader_t* bit_reader, word_t* word, unsigned int numberOfBits)
{
    *word = 0;

    if (numberOfBits > bit_reader->scratchBitsCount)
    {
        if (getNextWordFromBuffer(bit_reader) < 0) return -1;
    }

    *word |= (bit_reader->scratch & (((uint64_t)1 << numberOfBits) - 1));
    bit_reader->scratch >>= numberOfBits;
    bit_reader->scratchBitsCount -= numberOfBits;
    return 0;
}

static int getNextWordFromBuffer(bit_reader_t* bit_reader)
{
    if (bit_reader->wordsCount + 1 > bit_reader->size) return -1;

    bit_reader->scratch |= (uint64_t)bit_reader->buffer[bit_reader->wordsCount] << bit_reader->scratchBitsCount;
    bit_reader->scratchBitsCount += WORD_SIZE;
    bit_reader->wordsCount++;
    return 0;
}
