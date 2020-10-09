#include <stdlib.h>
#include <iostream>
#include <gmp.h>
#include <time.h> 

using namespace std;

unsigned char *genRandomByteBlock(size_t num_bytes);
int calcExponent (mpz_t blockint);

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

     unsigned char *byteblock;
     byteblock = genRandomByteBlock(blocksize);


     cout << "Random array " << endl;
     for (int f = 0; f < blocksize; f++) {
         //std::cout << bal[f] << ' ';
         printf("%d ",byteblock[f]);
     }
     cout << endl;


     mpz_t x, remainder, modulusInt, byteblockInt;

     mpz_init_set_str(x, "2", 10);
     mpz_init_set_str(remainder, "16", 10);
     mpz_init_set_str(modulusInt, "1", 10);
     mpz_init(byteblockInt);

     // void mpz_import (mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op) 
     mpz_import (byteblockInt, blocksize, 1, sizeof(byteblock[0]), 0, 0, byteblock);

     // void * mpz_export (void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)


     // mpz_mul(result, x, y);
     //cout<<"\nThe exact result is:";
     //mpz_out_str(stdout, 10, result);
     //cout<<endl;

     cout<<"\nThe byteblock bigint result is: ";
     mpz_out_str(stdout, 10, byteblockInt);
     cout<<endl;

     // calculate the modulus 2 ^ modsize 
     mpz_ui_pow_ui (modulusInt, 2, modsize);
     // mpz_pow (result, x, y);
     std::cout << "2 ^ " << modsize << " = ";
     // cout << mpz_out_str(stdout, 10, result);
     gmp_printf("%Zd", modulusInt);
     // cout << result;
     cout << '\n';

     // std::cout << "2 ^ 16 = " << mpz_out_str(stdout, 2, result) << '\n';
    
     mpz_mod (remainder, byteblockInt, modulusInt); 
     std::cout << "modulus = " << mpz_out_str(stdout, 10, remainder) << '\n';

     int exp = calcExponent(byteblockInt);
     std::cout << "modulus exponent = " << exp << '\n';

     /* free used memory */
     free (byteblock);
     mpz_clear(x);
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);



    return 0;
}

unsigned char *genRandomByteBlock(size_t num_bytes) {
    unsigned char *stream;
    stream = (unsigned char *) malloc(num_bytes);
    srand(time(0));

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = (rand() % 255);
    }

    return stream;
}

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
