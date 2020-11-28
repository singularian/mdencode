#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <ostream>
#include <openssl/sha.h>

// https://stackoverflow.com/questions/918676/generate-sha-hash-in-c-using-openssl-library
// https://stackoverflow.com/questions/918676/generate-sha-hash-in-c-using-openssl-library#919375
// https://github.com/openssl/openssl/blob/master/include/openssl/sha.h
// sudo apt-get install libssl-dev
int main()
{
    const unsigned char ibuf[] = "compute sha1";
    unsigned char obuf[20];
    // const char* t = (unsigned char) ibuf;
    // size_t length = (unsigned) strlen(ibuf);
    int s = strlen((char*)ibuf);

    // SHA1(ibuf, strlen(ibuf), obuf);
    // SHA1(ibuf, length, obuf);
    // SHA1(ibuf, 12, obuf);
    SHA1(ibuf, s, obuf);

    int i;
    for (i = 0; i < 20; i++) {
        printf("%02x ", obuf[i]);
    }
    printf("\n");

    // test2
    const unsigned char ibuf2[] = "higher higher higher";
    s = strlen((char*)ibuf2);
    SHA1(ibuf2, s, obuf);

    for (i = 0; i < 20; i++) {
        printf("%02x ", obuf[i]);
    }
    printf("\n");

    // test3
    s = strlen((char*)ibuf);
    SHA1(ibuf, s, obuf);
    for (i = 0; i < 20; i++) {
        printf("%02x ", obuf[i]);
    }
    printf("\n");


    return 0;
}
