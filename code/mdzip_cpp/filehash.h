#include <cstring>
#include <stdio.h>
#include "../testdecode_cpp/external/cityhash/cityhash.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>

static const int K_READ_BUF_SIZE = { 1024 * 16 };

// create a cityhash64 file signature
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

    unsigned char buf[K_READ_BUF_SIZE] = { };
    while (!feof(fp))
    {
        size_t total_read = fread(buf, 1, sizeof(buf), fp);
        // city64i += cityhash64_with_seed(byteblock, blocksize, city64seed);
        city64i += cityhash64_with_seed(buf, total_read, city64seed);
    }
    fclose(fp);

    return city64i;
}

// create a openssl ripe160 file signature
int calculateRipe160(char *filename, unsigned char *digest)
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

// create a openssl SHA1 file signature
int calculateSHA1(char *filename, unsigned char *sha1_digest)
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

    if(!SHA1_Final(sha1_digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl SHA256 file signature
int calculateSHA256(char *filename, unsigned char *sha1_digest)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }

    // unsigned char* sha1_digest = malloc(sizeof(char)*SHA_DIGEST_LENGTH);
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

    if(!SHA256_Final(sha1_digest, &context))
        return 0;

    // return true;
    return 1;
}

// create a openssl SHA384 file signature
int calculateSHA384(char *filename, unsigned char *digest)
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
int calculateSHA512(char *filename, unsigned char *digest)
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


