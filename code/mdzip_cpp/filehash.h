#include <cstring>
#include <stdio.h>
#include <openssl/sha.h>

static const int K_READ_BUF_SIZE = { 1024 * 16 };
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

