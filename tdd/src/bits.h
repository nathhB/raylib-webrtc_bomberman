#ifndef BITS_H_INCLUDED
#define BITS_H_INCLUDED

#define B_MASK(n) (1 << n)
#define B_SET(mask, n) mask |= B_MASK(n)
#define B_UNSET(mask, n) mask &= ~B_MASK(n)
#define B_IS_SET(mask, n) (B_MASK(n) & mask) == B_MASK(n)
#define B_IS_UNSET(mask, n) (B_MASK(n) & mask) == 0

#endif // BITS_H_INCLUDED
