#include "external/fnv/fnv.h"

struct hash_list 
{
    // city hash
    uint64_t city64i;
    uint64_t city64o;
    uint64_t city64seed = 102922;
    // crc32
    uint32_t crc32seed = 12121221;
    uint32_t crc32i;
    uint32_t crc32o;
    // crc64
    uint64_t crc64seed = 12121221;
    uint64_t crc64i;
    uint64_t crc64o;
    // fasthash
    uint32_t fast32i;
    uint32_t fast32o;
    uint32_t fast32seed = 200;
    uint64_t fast64i;
    uint64_t fast64o;
    uint64_t fast64seed = 1;
    // fnv1
    Fnv32_t fnv32_1i; 
    Fnv32_t fnv32_1o;
    Fnv32_t fnv32a_1i;
    Fnv32_t fnv32a_1o;
    Fnv64_t fnv64_1i; 
    Fnv64_t fnv64_1o; 
    Fnv64_t fnv64a_1i; 
    Fnv64_t fnv64a_1o;
    // highway hash
    uint64_t hw64i;
    uint64_t hw64o;
    uint64_t hw64key[4] = {1, 2, 3, 4};
    // metro64hash
    uint8_t met641i[8];
    uint8_t met641o[8];
    uint32_t met641seed = 1237789;
    uint8_t met642i[8];
    uint8_t met642o[8];
    uint32_t met642seed = 1237789;
    // md2 to md6
    uint8_t md2i[16];
    uint8_t md2o[16];
    uint8_t md4i[16];
    uint8_t md4o[16];
    uint8_t md5i[16];
    uint8_t md5o[16];
    // md6
    uint8_t md6i[20];
    uint8_t md6o[20];
    uint8_t md62i[20];
    uint8_t md62o[20];
    unsigned char md62key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // mx3
    uint64_t mx3i;
    uint64_t mx3o;
    uint64_t mx3seed = 121212;
    // pengyhash
    uint64_t png64i;
    uint64_t png64o;
    uint32_t png64seed = 127982;
    // ripe 160
    uint8_t ripe160i[21];
    uint8_t ripe160o[21];
    // seahash
    uint64_t sea64i;
    uint64_t sea64o;
    uint64_t sea64seed = 12312;
    // sha1 family
    uint8_t sha1i[21];
    uint8_t sha1o[21];
    uint8_t sha256i[32];
    uint8_t sha256o[32];
    uint8_t sha384i[64];
    uint8_t sha384o[64];
    uint8_t sha512i[64];
    uint8_t sha512o[64];
    // siphash
    uint64_t siphash64i;
    uint64_t siphash64o;
    char sipkey[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // spooky 32 / 64
    uint32_t spooky32i;
    uint32_t spooky32o;
    uint32_t spookyseed32;
    uint64_t spooky64i;
    uint64_t spooky64o;
    uint64_t spookyseed64;
    // xxhash
    uint32_t xxhash32i;
    uint32_t xxhash32o;
    uint32_t xxseed32 = 0;
    uint64_t xxhash64i;
    uint64_t xxhash64o;
    uint64_t xxseed64 = 0;
    // Whirlpool
    uint8_t whp512i[64];
    uint8_t whp512o[64];
    // wyhash
    uint64_t wyhash64i;
    uint64_t wyhash64o;
    uint64_t wyseed64 = 10232123120;
    uint64_t wysecret64[5] = {0xa0761d6478bd642full, 0xe7037ed1a0b428dbull, 0x8ebc6af09c88c6e3ull, 0x589965cc75374cc3ull, 0x1d8e4e27c47d124full};
};

struct hash_key_list
{
    // city hash
    uint64_t city64seed = 102922;
    // crc32
    uint32_t crc32seed = 12121221;
    // crc64
    uint64_t crc64seed = 12121221;
    // fasthash
    uint32_t fast32seed = 200;
    uint64_t fast64seed = 1;
    // highway hash
    const uint64_t hw64key[4] = {1, 2, 3, 4};
    // metro64hash
    uint32_t met641seed = 1237789;
    uint32_t met642seed = 1237789;
    // md6
    unsigned char md62key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // mx3
    uint64_t mx3seed = 121212;
    // pengyhash
    uint32_t png64seed = 127982;
    // seahash
    uint64_t sea64seed = 12312;
    // siphash
    char sipkey[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // spooky 32 / 64
    uint32_t spookyseed32 = 12312;
    uint64_t spookyseed64 = 1232;
    // xxhash
    uint32_t xxseed32 = 0;
    uint64_t xxseed64 = 0;
    // wyhash
    uint64_t wyseed64 = 10232123120;
    uint64_t wysecret64[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
};
