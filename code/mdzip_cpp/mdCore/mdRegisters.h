struct hash_list 
{
    // city hash
    uint64_t city64i;
    uint64_t city64o;
    uint64_t city64seed = 100000;
    // crc32
    uint32_t crc32seed  = 100000;
    uint32_t crc32i;
    uint32_t crc32o;
    // crc64
    uint64_t crc64seed  = 100000;
    uint64_t crc64i;
    uint64_t crc64o;
    // rhash edon 224
    uint8_t edn224i[28];
    uint8_t edn224o[28];   
    // fasthash
    uint32_t fast32i;
    uint32_t fast32o;
    uint32_t fast32seed = 100000;
    uint64_t fast64i;
    uint64_t fast64o;
    uint64_t fast64seed = 100000;
    // fnv1
    uint32_t fnv32_1i; 
    uint32_t fnv32_1o;
    uint32_t fnv32a_1i;
    uint32_t fnv32a_1o;
    uint64_t fnv64_1i; 
    uint64_t fnv64_1o; 
    uint64_t fnv64a_1i; 
    uint64_t fnv64a_1o;
    // rhash has-160
    uint8_t has160i[20];
    uint8_t has160o[20];
    // highway hash 40
    uint64_t hw40i;
    uint64_t hw40o;
    uint8_t  hw40bi[8];
    uint8_t  hw40bo[8];
    uint64_t hw40key[4] = {1, 2, 3, 4};    
    // highway hash 64
    uint64_t hw64i;
    uint64_t hw64o;
    uint64_t hw64key[4] = {1, 2, 3, 4};
    // metro64hash
    uint8_t met641i[8];
    uint8_t met641o[8];
    uint32_t met641seed = 100000;
    uint8_t met642i[8];
    uint8_t met642o[8];
    uint32_t met642seed = 100000;
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
    uint64_t mx3seed    = 100000;
    // pengyhash
    uint64_t png64i;
    uint64_t png64o;
    uint32_t png64seed  = 100000;
    // ripe 128
    uint8_t ripe128i[16];
    uint8_t ripe128o[16];
    // ripe 160
    uint8_t ripe160i[21];
    uint8_t ripe160o[21];
    // ripe 256
    uint8_t ripe256i[32];
    uint8_t ripe256o[32];
    // ripe 320
    uint8_t ripe320i[40];
    uint8_t ripe320o[40];
    // seahash
    uint64_t sea64i;
    uint64_t sea64o;
    uint64_t sea64seed  = 100000;
    // sha1 family
    uint8_t sha1i[21];
    uint8_t sha1o[21];
    uint8_t sha256i[32];
    uint8_t sha256o[32];
    uint8_t sha384i[48];
    uint8_t sha384o[48];
    uint8_t sha512i[64];
    uint8_t sha512o[64];
    // siphash 32
    // uses the second siphash method
    uint8_t siphash32i[16];
    uint8_t siphash32o[16];
    uint8_t sipkey32[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // siphash 32 2
    uint8_t siphash322i[16];
    uint8_t siphash322o[16];
    uint8_t sipkey322[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // siphash40
    // currently uses the second siphash method
    uint8_t siphash40i[16];
    uint8_t siphash40o[16];
    // bool boolHash[16] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
    bool    boolHash[16] = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    uint8_t sipkey40[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // siphash40 test variant with a 64 byte key
    uint8_t siphash402i[16];
    uint8_t siphash402o[16];
    // siphash40 variant with 64 byte key boolean compare array
    bool    sipboolHash42[16] = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    // siphash40 64 byte key 
    uint8_t sipkey402[64] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,
                            0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,
                            0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,
                            0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf };
    // siphash48
    // uses the second siphash method
    uint8_t siphash48i[16];
    uint8_t siphash48o[16];
    uint8_t sipkey48[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // siphash 64
    uint64_t siphash64i;
    uint64_t siphash64o;
    char sipkey64[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // siphash 128
    uint8_t siphash128i[16];
    uint8_t siphash128o[16];
    uint8_t sipkey128[16]  = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // spooky 32 / 64
    uint32_t spooky32i;
    uint32_t spooky32o;
    uint32_t spookyseed32 = 100000;
    uint64_t spooky64i;
    uint64_t spooky64o;
    uint64_t spookyseed64 = 100000;
    // tiger 192
    unsigned char tiger192i[24];
    unsigned char tiger192o[24];
    // xxhash
    uint32_t xxhash32i;
    uint32_t xxhash32o;
    uint32_t xxseed32    = 100000;
    uint64_t xxhash64i;
    uint64_t xxhash64o;
    uint64_t xxseed64    = 100000;
    // Whirlpool
    uint8_t whp512i[64];
    uint8_t whp512o[64];
    // wyhash
    uint64_t wyhash64i;
    uint64_t wyhash64o;
    uint64_t wyseed64 = 100000;
    uint64_t wysecret64[5] = {0xa0761d6478bd642full, 0xe7037ed1a0b428dbull, 0x8ebc6af09c88c6e3ull, 0x589965cc75374cc3ull, 0x1d8e4e27c47d124full};

};


