#include <stdlib.h>
#include <iostream>
#include <gmp.h>
#include <time.h> 
#include "modscan.h"
#include "sha1.h"
#include "string.h"
#include "stdio.h"

using namespace std;

uint8_t sha1[40];

int genSHA1(unsigned char *byteblock, int blocksize);
unsigned char *genRandomByteBlock(size_t num_bytes);
unsigned char *setByteBlock(size_t num_bytes);
int calcExponent (mpz_t blockint);
int calcExponentModulus (mpz_t modulus, mpz_t blockint);
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize );
void usage();

/* 
   This is a C++ GMP modscan test program 
   MDencode GMP requires the GMP Library to build https://gmplib.org/
   It runs with one process and is not currently multithreaded.
   This program uses one signature SHA1 to bootstrap the testing.
   In the future it will use a C++ Hash Context list with more than one signature
*/
int main (int argc, char **argv) {

     size_t blocksize = 12;
     int modsize      = 64;

     if (argc < 3)  
     { 
         usage();
         return 0; 
     } 

     blocksize = atoi(argv[1]);
     modsize   = atoi(argv[2]); 

     // generate a random n byte byteblock
     unsigned char *byteblock;
     byteblock = genRandomByteBlock(blocksize);

     // set a predefined byte block for testing
     // byteblock = setByteBlock(blocksize);
       

     mpz_t remainder, modulusInt, byteblockInt;

     mpz_init_set_str(remainder, "0", 10);
     mpz_init_set_str(modulusInt, "1", 10);
     mpz_init_set_str(byteblockInt, "0", 10);
     // mpz_init_set_str(byteblockInt, "168873676072691430781078090", 2);

     // create the byteblock bigint
     // void mpz_import (mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op) 
     // order very important
     // within each word endian can be 1 for most significant byte first, -1 for least significant first, or 0 for the native endianness of the host CPU
     // order has to match the modulus scan mpz_export
     mpz_import (byteblockInt, blocksize, 0, sizeof(byteblock[0]), 0, 0, byteblock);

     // export the gmp bigint
     // void * mpz_export (void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)
     /* unsigned char test [11]; // need to make sure this is the size of the byte block
     size_t count;
     printf("test export\n");
     mpz_export(test, &count, 0, sizeof(byteblock[0]), 0, 0, byteblockInt);
     for (int i = 0; i < blocksize; i++) {
           printf("%d ", byteblock[i]);
     }
     printf("\n");
     */

     // calculate the modulus 2 ^ modsize 
     mpz_ui_pow_ui (modulusInt, 2, modsize);

     // calculate the modulus remainder 
     mpz_mod (remainder, byteblockInt, modulusInt); 

     // calculate the modulus exponent with two
     int exp = calcExponent(byteblockInt);

     // calculate the modulus exponent with the modulus
     int expmod = calcExponentModulus(modulusInt, byteblockInt);

     // compute the SHA1 hash of the byteblock
     // this currently uses one signature and not a hash context list
     // others will be added in the future
     genSHA1(byteblock, blocksize);

     // display the current block stats
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize );

     // initialize the modulus scan object
     modscan ms;
     ms.setModscan(remainder, modulusInt, exp, expmod, blocksize, sha1);

     // run the modulus scan
     ms.decode();

     // check the modulus scan results
     unsigned char *modbyteblock;
     modbyteblock = ms.getModscanByteBlock();
     if (memcmp(modbyteblock, byteblock, blocksize) == 0) {
          cout << "Modulus Scan and Random byteblock match" << endl;
          for (int i = 0; i < blocksize; i++) {
               printf("%d ", byteblock[i]);
          }
          printf("\n");

          for (int i = 0; i < blocksize; i++) {
               printf("%d ", modbyteblock[i]);
          }
          printf("\n");
     } else {
          cout << "Modulus Scan and Random byteblock don't match" << endl;
     }


     /* free used memory */
     free (byteblock);
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);
     // mpz_clears(remainder, modulusInt, byteblockInt, NULL);



    return 0;
}

// calculate test signature
int genSHA1(unsigned char *byteblock, int blocksize) {
       SHA1_CTX sha; 
       // uint8_t results[20]; 
       SHA1Init(&sha);
       SHA1Update(&sha, (uint8_t *)byteblock, blocksize);
       SHA1Final(sha1, &sha);

       return 0;
}


// returns a random byte sized byteblock
unsigned char *genRandomByteBlock(size_t num_bytes) {
    unsigned char *stream;
    stream = (unsigned char *) malloc(num_bytes);
    srand(time(0));

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = (rand() % 255);
    }

    return stream;
}

unsigned char *setByteBlock(size_t num_bytes) {
    unsigned char *stream;
    // stream = (unsigned char *) malloc(num_bytes);
    stream = (unsigned char *) malloc(11);

    int num [] = { 139, 176, 100, 82, 220, 198, 148, 121, 155, 202, 74 };

	

    for (int f = 0; f < 11; f++) {
       stream[f] = num[f];
    }

    return stream;
}


// calculates an exponent of 2 less than the byte block int
int calcExponent (mpz_t blockint) {
    int exponent = 0;

    mpz_t two, result;

    mpz_init_set_str(two, "2", 10);
    mpz_init_set_str(result, "2", 10);

    do {
      mpz_mul(result, result, two); 
      exponent++; 
    } while(mpz_cmp(result,blockint) < 0);

    mpz_clear(two);
    mpz_clear(result);

    return exponent;
}

// calculates an exponent of the modulus less than the byte block int
int calcExponentModulus (mpz_t modulus, mpz_t blockint) {
    int exponent = 0;

    mpz_t result;

    mpz_init_set_str(result, "", 10);
    mpz_add (result, result, modulus); 


    do {
      mpz_mul(result, result, modulus);
      exponent++;
    } while(mpz_cmp(result,blockint) < 0);

    mpz_clear(result);

    return exponent;
}


// displays the modulus scan information
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize ) {

     cout << "Block Size " << blocksize << endl;

     cout << "Random byteblock " << " ";
     for (int f = 0; f < blocksize; f++) {
         //std::cout << bal[f] << ' ';
         // printf("%d ",byteblock[f]);
          printf("%0X", byteblock[f]);
     }
     cout << endl;

     cout << "Random byteblock " << blocksize << " ";
     for (int f = 0; f < blocksize; f++) {
          printf("%d ",byteblock[f]);
     }

     cout << endl <<"Random byteblock bigint ";
     mpz_out_str(stdout, 10, blockint);
     cout<<endl;

     cout << "Modulus Size   " << modsize << endl;

     std::cout << "Modulus int 2 ^ " << modsize << " = ";
     gmp_printf("%Zd", modint);
     // cout << result;
     cout << '\n';

     std::cout << "Modulus remainder =  ";
     gmp_printf("%Zd", remainder);
     cout << endl;

     std::cout << "Modulus two exponent  =  " << exponent << endl;
     std::cout << "Modulus exponent  =  " << expmod << endl;

     cout << "Block SHA1 ";
     for (int n = 0; n < 20; n++)
     printf("%02x", sha1[n]);
     printf("\n");

}

// display the usage
void usage() {
     printf("MDencode GMP C++ Modulus Scan Test\n");
     printf("MDencode GMP is not currently multithreaded and only runs in one process\n");
     printf("MDencode GMP requires the GMP Library to build https://gmplib.org/\n\n");
     printf("Parameters [byteblock size] [mod size]\n");
     printf("Parameters 12 64\n");
}
