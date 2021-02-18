/*
   SeaHash C implementation
*/

#include <stdio.h>
#include <stdint.h>

static uint64_t sea_swap64 (uint64_t x);
static inline uint64_t diffuse(uint64_t val);
uint64_t seahash(const char *key, int len, uint64_t seed);

