#include <cstring>
#include <stdio.h>
#include "../testdecode_cpp/external/cityhash/cityhash.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>

static const int K_READ_BUF_SIZE = { 1024 * 16 };

// Add each block to the uinti64_t result
// this is a workaround since I don't think it has a update/finalize method
// then return the result
uint64_t calculateCityhashFile(char *filename, uint64_t city64seed)
{

    uint64_t city64i = 0;    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    unsigned char buf[K_READ_BUF_SIZE];
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        // city64i += cityhash64_with_seed(byteblock, blocksize, city64seed);
        city64i += cityhash64_with_seed(buf, total_read, city64seed);
    }
    fclose(fp);



    return city64i;
}

unsigned char* calculateRipe160(char *filename, unsigned char *digest)
{
    if (!filename) {
        return NULL;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    RIPEMD160_CTX context;

    if(!RIPEMD160_Init(&context))
        return NULL;

    unsigned char buf[K_READ_BUF_SIZE];
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!RIPEMD160_Update(&context, buf, total_read))
        {
            return NULL;
        }
    }
    fclose(fp);

    if(!RIPEMD160_Final(digest, &context))
        return NULL;

    // return sha1_digest;
    return NULL;
}


unsigned char* calculateSHA1(char *filename, unsigned char *sha1_digest)
{
    if (!filename) {
        return NULL;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // unsigned char* sha1_digest = malloc(sizeof(char)*SHA_DIGEST_LENGTH);
    SHA_CTX context;

    if(!SHA1_Init(&context))
        return NULL;

    unsigned char buf[K_READ_BUF_SIZE];
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        if(!SHA1_Update(&context, buf, total_read))
        {
            return NULL;
        }
    }
    fclose(fp);

    if(!SHA1_Final(sha1_digest, &context))
        return NULL;

    // return sha1_digest;
    return NULL;
}

