#include <bits/stdc++.h>
#include "sha1.h"

using namespace std;

class modscan 
{ 
    private:
       uint8_t sha1[20];
       unsigned char *byteblock; 
    public: 
       string filename; 
       int exponent;
       int modexponent;
       int modsize;
       int blocksize;
       int threadnumber;
       int threadcount;
       mpz_t remainder;
       mpz_t modulusInt; 
       mpz_t modulusExpInt; 
       mpz_t blockInt; 
       // ~modscan();      // destructor
 
    // Default constructor
    modscan() 
    {
       // mpz_init_set_str(remainder,  "0", 10);
       // mpz_init_set_str(modulusInt, "0", 10);
       mpz_init_set_str(blockInt, "0", 10);
       // mpz_inits(remainder, modulusInt, modulusExpInt, blockInt, NULL);
       mpz_inits(remainder, modulusInt, modulusExpInt, NULL);
       // mpz_init2 (blockInt, blocksize * 8); // For padding
    }

    // Destructor
    ~modscan()
    {
       std::cout << "Destroying modscan" << endl;
       mpz_clears(remainder, modulusInt, modulusExpInt, blockInt, NULL);
       // delete []byteblock;
       free (byteblock);
    }

    void setModscan(mpz_t rem, mpz_t modint, int exp, int modexp, int blocks, uint8_t *sha1block) {
        mpz_add (remainder, remainder, rem);
        mpz_add (modulusInt, modulusInt, modint);
        mpz_add (blockInt, blockInt, remainder);
        exponent    = exp;
        modexponent = modexp;
        blocksize   = blocks;

        printf("modulus blocksize %d \n",  blocksize);
        printf("modulus exponent1 %d \n",  exponent);
        printf("modulus exponent2 %d \nmodulus int ",  modexponent);
        gmp_printf("modulus int %Zd\n", modulusInt);
        gmp_printf("modulus int remainder %Zd\n", remainder);
        gmp_printf("starting block int %Zd\n", blockInt);

        // byteblock = new unsigned char[blocksize+100];
        byteblock = (unsigned char *) malloc(blocksize);
        memcpy(sha1,sha1block, sizeof(sha1) / sizeof(sha1[0]));

        cout << "sha1 ";
        for (int n = 0; n < 20; n++)
             printf("%02x", sha1[n]);
        printf("\n");

    }
 
    void printname() 
    { 
       std::cout << "Filename is: " << filename << endl; 
    } 

    void decode()
    {
       std::cout << "Running decode modscan" << endl;

       size_t count;
       int continueFlag = 0;
       int lineNum = 0;

       // need to change this to use the 2^exp 
       // calculate the modulus raised to the exponent
       // mpz_pow_ui (modulusExpInt, 2, modsize);
       if (modexponent > 1) {
           mpz_pow_ui (modulusExpInt, modulusInt, modexponent);
           // add the modulus ^ modulus exponent to the block int 
           mpz_add (blockInt, blockInt, modulusExpInt);
       }

       // add the modulus ^ modulus exponent to the block int 
       // mpz_add (blockInt, blockInt, modulusExpInt);

       printf("modulus exponent %d\n",  modexponent);
       gmp_printf("modulus exponent %Zd", modulusExpInt);
       cout << endl;
       gmp_printf("%Zd", modulusInt);
       cout << endl;
       gmp_printf("%Zd", blockInt);
       cout << endl;

       SHA1_CTX sha;
       uint8_t results[20]; 
       char *buf; 
       int n;
       SHA1Init(&sha);

       mpz_t testInt;
       mpz_init_set_str(testInt, "168873676072691430781078090", 10);
 
       while (continueFlag == 0)
       {
           // for (int i = 0; i < blocksize; i++) byteblock[i] = 0;
           // nails has to be zero for it to update
           // mpz_export(byteblock, &count, -1, sizeof(byteblock[0]), 0, 0, blockInt); 
           // mpz_export(byteblock, &count, 1, sizeof(byteblock[0]), 0, 0, blockInt); 
           // order has to match the import
           mpz_export(byteblock, &count, 0, sizeof(byteblock[0]), 0, 0, blockInt); 

           // it has to initialize the sha1Init each time so it resets the context
           // instead of updating it
           SHA1Init(&sha);
           SHA1Update(&sha, (uint8_t *)byteblock, blocksize); 
           SHA1Final(results, &sha);
           
           // if (strcmp((char *) results, (char *) sha1) == 0) {
           if (memcmp(results, sha1, 20) == 0) {
               printf("Found block ");
               for (n = 0; n < blocksize; n++) {
                    printf("%02x", byteblock[n]);
               }
               printf("\n");
               for (n = 0; n < blocksize; n++) {
                    printf("%02d ", byteblock[n]); 
               }
               printf("\n");
               for (n = 0; n < 20; n++)
                    printf("%02x", results[n]);
               printf("\n");
               break;
           } else {
              if (lineNum > 10000000) {
              printf("Searching block ");
              gmp_printf("%Zd", blockInt);
              printf(" byteblock ");
              for (n = 0; n < blocksize; n++) {
                    printf("%02x", byteblock[n]);
              }
              printf(" sha1 ");
              for (n = 0; n < 20; n++)
                    printf("%02x", sha1[n]);
              printf(" results  ");
              for (n = 0; n < 20; n++)
                    printf("%02x", results[n]);
              printf("\n");
                 lineNum = 0;
              }
           }
           
           // test code
           // to check if a certain bigit is found
           /* if (mpz_cmp(testInt,blockInt) == 0) {
               printf("Test block ");
              gmp_printf("%Zd ", blockInt);
              gmp_printf("%Zd", testInt);
              printf(" byteblock ");
              for (n = 0; n < blocksize; n++) {
                    printf("%02X", byteblock[n]);
              }
              printf(" sha1 ");
              for (n = 0; n < 20; n++)
                    printf("%02x", sha1[n]);
              printf(" results  ");
              for (n = 0; n < 20; n++)
                    printf("%02x", results[n]);
              printf("\n");

           } */


           mpz_add (blockInt, blockInt, modulusInt);
           lineNum++;
       }

       // testInt
       mpz_clear(testInt);         

    }
}; 
