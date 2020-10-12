#include <bits/stdc++.h>
#include "sha1.h"

using namespace std;

class modscan 
{ 
    private:
       uint8_t sha1[41];
       unsigned char *byteblock; 
    public: 
       string filename; 
       int exponent;
       int modexponent;
       int modsize;
       int blocksize;
       int threadnumber;
       int threadcount;
       mpz_t two;
       mpz_t modremainder;
       mpz_t remainder;
       mpz_t modulusInt; 
       mpz_t modulusExpInt; 
       mpz_t blockInt; 
 
    // Default constructor
    modscan() 
    {
       // mpz_init_set_str(remainder,  "0", 10);
       // mpz_init_set_str(modulusInt, "0", 10);
       mpz_init_set_str(two, "2", 10); // exponent floor
       mpz_init_set_str(modremainder, "0", 10); // exponent floor
       mpz_init_set_str(blockInt, "0", 10);
       // mpz_inits(remainder, modulusInt, modulusExpInt, blockInt, NULL);
       mpz_inits(remainder, modulusInt, modulusExpInt, NULL);
       // mpz_init2 (blockInt, blocksize * 8); // For padding
    }

    // Destructor
    ~modscan()
    {
       std::cout << "Destroying modscan" << endl;
       mpz_clears(two, modremainder, remainder, modulusInt, modulusExpInt, blockInt, NULL);

       if (sizeof(byteblock) / sizeof(byteblock[0]) != 0) {
           cout << "free byteblock" << endl;
           delete []byteblock;
       // free (byteblock);
       }
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

        byteblock = new unsigned char[blocksize+1];
        // byteblock = (unsigned char *) malloc(blocksize);
        memcpy(sha1,sha1block, sizeof(sha1) / sizeof(sha1[0]));

        //cout << "sha1 ";
        //for (int n = 0; n < 20; n++)
        //     printf("%02x", sha1[n]);
        //printf("\n");

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
       ///if (modexponent > 1) {
           //// mpz_pow_ui (modulusExpInt, modulusInt, modexponent);
           // add the modulus ^ modulus exponent to the block int 
           //// mpz_add (blockInt, blockInt, modulusExpInt);
       //}
       // mpz_pow_ui (modulusExpInt, modulusInt, modexponent);
       mpz_pow_ui (modulusExpInt, two, exponent);
       convertFloorBase2(modulusExpInt, modulusInt);

       // add the modulus ^ modulus exponent to the block int 
       mpz_add (blockInt, blockInt, modremainder);

       printf("modulus exponent %d\n",  modexponent);
       gmp_printf("modulus exponent %Zd", modulusExpInt);
       cout << endl;
       gmp_printf("%Zd", modulusInt);
       cout << endl;
       gmp_printf("%Zd", blockInt);
       cout << endl;

       SHA1_CTX sha;
       uint8_t results[40]; 
       char *buf; 
       int n;
       SHA1Init(&sha);

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
           

           mpz_add (blockInt, blockInt, modulusInt);
           lineNum++;
       }

    }

    // converts 2 ^ exp to the modulus floor ^ exp
    void convertFloorBase2(mpz_t modfloor, mpz_t modint) {
        mpz_t mfloor;
        mpz_t zero;
        // mpz_t modremainder;

        mpz_init_set_str(mfloor, "0", 10);
        mpz_init_set_str(zero, "0", 10);
        mpz_init_set_str(modremainder, "0", 10);

        // mpz_add (modremainder, modremainder, modfloor);
        mpz_add (mfloor, mfloor, modfloor);
        mpz_mod (modremainder, modfloor, modint);
        int cmp = mpz_cmp(modremainder,zero);

        if (cmp == 0) {
          // modremainder = modremainder.Set(mfloor)
          mpz_set(modremainder, mfloor);
        } else {
          mpz_sub(modremainder, modremainder, modfloor);

        }

        mpz_clear(mfloor);
        mpz_clear(zero);
        // clear(modremainder)
 
    }

    unsigned char* getModscanByteBlock()
    {

        return byteblock; 
    }

}; 
