#include <cstring>
#include <stdio.h>
#include "external/cityhash/cityhash.h"
#include "external/crc32/crc32.h"
#include "external/crc64/crc64.h"
#include "external/csiphash/csiphash.h"
#include "external/fasthash/fasthash.h"
// #include "external/fnv/fnv.h"
#include "external/fnv2/fnv.h"
#include "external/highwayhash/highwayhash.h"
#include "external/xxhash/xxhash32.h"
#include "external/xxhash/xxhash64.h"
#include "external/metro64/metrohash64.h"
#include "external/mx3/mx3.h"
#include "external/pengyhash/pengyhash.h"
#include "external/seahash/seahash.h"
#include "external/md2/md2.h"
#include <openssl/md4.h>
#include <openssl/md5.h>
#include "external/md6/md6.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include "external/spooky/Spooky.h"
#include <openssl/whrlpool.h>
#include "external/wyhash/wyhash.h"


static const int K_READ_BUF_SIZE = { 1024 * 16 };

// create a cityhash64 file signature
// Add each block to the uinti64_t result
// this is a workaround since I don't think it has a update/finalize method
// then return the result
uint64_t getFileHashCityhash(char *filename, uint64_t city64seed)
{
    uint64_t city64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        city64i += cityhash64_with_seed(buf, total_read, city64seed);
    }
    fclose(fp);

    return city64i;
}

// get the CRC32 file hash
uint32_t getFileHashCRC32(char *filename, uint32_t crc32seed)
{
    uint32_t crc32i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        crc32i += crc32(crc32seed, buf, total_read);
    }
    fclose(fp);

    return crc32i;
}

// get the CRC64 file hash
uint64_t getFileHashCRC64(char *filename, uint64_t crc64seed)
{
    uint64_t crc64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        crc64i += crc64(crc64seed, buf, total_read);
    }
    fclose(fp);

    return crc64i;
}

// fasthash32 file
uint32_t getFileHashFast32(char *filename, uint32_t fast32seed)
{
    uint32_t fast32i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fast32i += fasthash32(buf, total_read, fast32seed);
    }
    fclose(fp);

    return fast32i;
}

// fasthash64
uint64_t getFileHashFast64(char *filename, uint64_t fast64seed)
{
    uint64_t fast64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fast64i += fasthash64(buf, total_read, fast64seed);
    }
    fclose(fp);

    return fast64i;
}

// FNV32
uint32_t getFileHashFNV32(char *filename)
{
    uint32_t fnv32_1i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fnv32_1i += fnv32_1(buf, total_read, FNV1_32_INIT);
    }
    fclose(fp);

    return fnv32_1i;
}

// FNV32A
uint32_t getFileHashFNV32A(char *filename)
{
    uint32_t fnv32a_1i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fnv32a_1i += fnv32_1a(buf, total_read, FNV1_32A_INIT);
    }
    fclose(fp);

    return fnv32a_1i;
}

// FNV64
uint64_t getFileHashFNV64(char *filename)
{
    uint64_t fnv64_1i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fnv64_1i += fnv64_1(buf, total_read, FNV1_64_INIT);
    }
    fclose(fp);

    return  fnv64_1i;
}

// FNV64A
uint64_t getFileHashFNV64A(char *filename)
{
    uint64_t fnv64a_1i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        fnv64a_1i += fnv64_1a(buf, total_read, FNV1A_64_INIT);
    }
    fclose(fp);

    return fnv64a_1i;
}

// Highway Hash 64
uint64_t getFileHashHW64(char *filename, uint64_t *hw64key)
{
    uint64_t hw64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        hw64i += HighwayHash64(buf, total_read, hw64key);
    }
    fclose(fp);

    return hw64i;
}




// create a MD2 file signature
int getFileHashMD2(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    MD2_CTX context;
    md2_init(&context);

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        md2_update(&context, buf, total_read);
    }
    fclose(fp);

    md2_final(&context, digest);

    // return true;
    return 1;
}


// create a openssl MD4 file signature
int getFileHashMD4(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    MD4_CTX context;

    if(!MD4_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!MD4_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!MD4_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl MD5 file signature
int getFileHashMD5(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    MD5_CTX context;

    if(!MD5_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!MD5_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!MD5_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a metro64_1 file signature
int getFileHashMetro64_1(char *filename, unsigned char *digest, uint32_t met641seed)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    MetroHash64 metro;
    metro.Initialize(met641seed);

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        metro.Update(reinterpret_cast<const uint8_t *>(buf), total_read);
    }
    fclose(fp);

    metro.Finalize(digest);

    // return true;
    return 1;
}

// create a metro64_2 file signature
int getFileHashMetro64_2(char *filename, unsigned char *digest, uint32_t met642seed)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    MetroHash64 metro;
    metro.Initialize(met642seed);

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        metro.Update(reinterpret_cast<const uint8_t *>(buf), total_read);
    }
    fclose(fp);

    metro.Finalize(digest);

    // return true;
    return 1;
}

// create a MX3 file signature 
uint64_t getFileHashMX3(char *filename, uint64_t mx3seed)
{
    uint64_t mx3i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        mx3i += mx3::hash(buf, total_read, mx3seed);
    }
    fclose(fp);

    return mx3i;
}

// create a Pengy file signature 
uint64_t getFileHashPNG(char *filename, uint64_t png64seed)
{
    uint64_t png64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        png64i += pengyhash(buf, (size_t) total_read, png64seed);
    }
    fclose(fp);

    return png64i;
}

// create a openssl ripe160 file signature
int getFileHashRipe160(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    RIPEMD160_CTX context;

    if(!RIPEMD160_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        RIPEMD160_Update(&context, buf, total_read);
        // if(!RIPEMD160_Update(&context, buf, total_read))
        //{
        //    fclose(fp);
        //    return 0;
        //}
    }
    fclose(fp);

    if(!RIPEMD160_Final(digest, &context))
        return 0;

    // return true
    return 1;
}

// create a Seahash file signature 
uint64_t getFileHashSeahash(char *filename, uint64_t sea64seed)
{
    uint64_t sea64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        sea64i += seahash((const char*)buf, total_read, sea64seed);
    }

    fclose(fp);

    return sea64i;
}

// create a Siphash file signature 
uint64_t getFileHashSiphash(char *filename, char *sipkey)
{
    uint64_t siphash64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        siphash64i += siphash24(buf,  total_read, sipkey);
    }
    fclose(fp);

    return siphash64i;
}

// create a openssl SHA1 file signature
int getFileHashSHA1(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    // unsigned char* sha1_digest = malloc(sizeof(char)*SHA_DIGEST_LENGTH);
    SHA_CTX context;

    if(!SHA1_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!SHA1_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!SHA1_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl SHA256 file signature
int getFileHashSHA256(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    SHA256_CTX context;

    if(!SHA256_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
/*    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!SHA256_Update(&context, buf, total_read))
        {   
            fclose(fp);
            return 0;
        }
    }
*/
    int bytesRead = 0;
    while((bytesRead = fread(buf, 1, sizeof(buf), fp)))
    {
        SHA256_Update(&context, buf, bytesRead);
    }
    fclose(fp);

    if(!SHA256_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl SHA384 file signature
int getFileHashSHA384(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    SHA512_CTX context;

    if(!SHA384_Init(&context))
        return 0; 

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!SHA384_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!SHA384_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl SHA512 file signature
int getFileHashSHA512(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    SHA512_CTX context;

    if(!SHA512_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!SHA512_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!SHA512_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a Spooky32 file signature 
uint32_t getFileHashSpooky32(char *filename, uint32_t spookyseed32)
{
    uint32_t spooky32i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        spooky32i += SpookyHash::Hash32(buf, total_read, spookyseed32);
    }
    fclose(fp);

    return spooky32i;
}


// create a Spooky64 file signature 
uint64_t getFileHashSpooky64(char *filename, uint64_t spookyseed64)
{
    uint64_t spooky64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        spooky64i += SpookyHash::Hash64(buf, total_read, spookyseed64);
    }
    fclose(fp);

    return spooky64i;
}


// create a XXH32 file signature 
uint32_t getFileHashXXH32(char *filename, uint32_t xxseed32)
{
    uint32_t xxhash32i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        xxhash32i += XXHash32::hash(buf, total_read, xxseed32);
    }
    fclose(fp);

    return xxhash32i;
}

// create a XXH64 file signature 
uint64_t getFileHashXXH64(char *filename, uint64_t xxseed64)
{
    uint64_t xxhash64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        xxhash64i += XXHash64::hash(buf, total_read, xxseed64);
    }
    fclose(fp);

    return xxhash64i;
}

// create a openssl WP file signature
int getFileHashWP(char *filename, unsigned char *digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    WHIRLPOOL_CTX context;

    if(!WHIRLPOOL_Init(&context))
        return 0;

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!WHIRLPOOL_Update(&context, buf, total_read))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);

    if(!WHIRLPOOL_Final(digest, &context))
        return 0;

    // return true;
    return 1;
}


// create a Wyhash file signature 
uint64_t getFileHashWyhash(char *filename, uint64_t wyseed64, uint64_t *wysecret64)
{
    uint64_t wyhash64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        wyhash64i += wyhash(buf, total_read, wyseed64, (const uint64_t*) wysecret64);

    }
    fclose(fp);

    return wyhash64i;
}