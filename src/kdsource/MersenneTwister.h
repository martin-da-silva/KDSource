#include <stdio.h>
#include <stdint.h>

#define MT_N 312
#define MT_M 156
#define MT_MATRIX_A 0xB5026F5AA96619E9ULL
#define MT_UPPER_MASK 0xFFFFFFFF80000000ULL
#define MT_LOWER_MASK 0x7FFFFFFFULL
#define	MT64_MAX    18446744073709551615 // 2^64 - 1


typedef struct {
    uint64_t state[MT_N];
    int index;
} MersenneTwister64;

MersenneTwister64* MT64_create(uint64_t* seed);
void initializeMersenneTwister64(MersenneTwister64* MT_, uint64_t* seed);
uint64_t MT64_rand();
void MT64_destroy(MersenneTwister64* MT_);
