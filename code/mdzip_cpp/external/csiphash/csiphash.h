/*
   SipHash C implementation
   https://github.com/majek/csiphash/
*/

#include <stdio.h>
#include <stdint.h>

uint64_t siphash24(const void *src, unsigned long src_sz, const char k[16]);
