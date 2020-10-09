#include <stdlib.h>
#include <iostream>
#include <gmp.h>
#include <time.h> 
#include "modscan.h"

using namespace std;

unsigned char *genRandomByteBlock(size_t num_bytes);
int calcExponent (mpz_t blockint);
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int blocksize );

int main (int argc, char **argv) {

     size_t blocksize = 12;
     int modsize      = 64;

     if (argc < 3)  
     { 
         printf("Parameters [byteblock size] [mod size]\n"); 
         printf("Parameters 12 14\n"); 
         return 0; 
     } 

     blocksize = atoi(argv[1]);
     modsize   = atoi(argv[2]); 

     // generate a random n byte byteblock
     unsigned char *byteblock;
     byteblock = genRandomByteBlock(blocksize);

     mpz_t x, remainder, modulusInt, byteblockInt;

     mpz_init_set_str(x, "2", 10);
     mpz_init_set_str(remainder, "0", 10);
     mpz_init_set_str(modulusInt, "1", 10);
     mpz_init_set_str(byteblockInt, "1", 10);

     // create the byteblock bigint
     // void mpz_import (mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op) 
     mpz_import (byteblockInt, blocksize, 1, sizeof(byteblock[0]), 0, 0, byteblock);

     // export the gmp bigint
     // void * mpz_export (void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)

     // calculate the modulus 2 ^ modsize 
     mpz_ui_pow_ui (modulusInt, 2, modsize);

     // calculate the modulus remainder 
     mpz_mod (remainder, byteblockInt, modulusInt); 

     // calculate the modulus exponent
     int exp = calcExponent(byteblockInt);

     // display the current block stats
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, blocksize ); 

     modscan ms;
     ms.filename = "filename";
     ms.printname();

     /* free used memory */
     free (byteblock);
     mpz_clear(x);
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);



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

// displays the modulus scan information
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int blocksize ) {

     cout << "Random array " << blocksize << " ";
     for (int f = 0; f < blocksize; f++) {
         //std::cout << bal[f] << ' ';
         // printf("%d ",byteblock[f]);
          printf("%X", byteblock[f]);
     }
     cout << endl;

     cout<<"The byteblock bigint result is: ";
     mpz_out_str(stdout, 10, blockint);
     cout<<endl;

     std::cout << "modulus int 2 ^ " << modsize << " = ";
     gmp_printf("%Zd", modint);
     // cout << result;
     cout << '\n';

     std::cout << "modulus remainder =  ";
     gmp_printf("%Zd", remainder);
     cout << endl;

     std::cout << "modulus exponent  =  " << exponent << '\n';

}
