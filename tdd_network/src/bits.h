#ifndef NETWORK_BITS_H_INCLUDED
#define NETWORK_BITS_H_INCLUDED

#include <stdint.h>

typedef uint32_t word_t;

unsigned int number_of_bits_required_for(unsigned int);

#define WORD_SIZE (sizeof(word_t) * 8)
#define BITS_REQUIRED(min, max) (min == max) ? 0 : number_of_bits_required_for(max - min)

#endif // NETWORK_BITS_H_INCLUDED