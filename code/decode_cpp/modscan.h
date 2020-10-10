#include <bits/stdc++.h>

using namespace std;

class modscan 
{ 
    // Access specifier 
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
 
    // Default constructor
    modscan() 
    {
       // mpz_init_set_str(remainder,  "0", 10);
       // mpz_init_set_str(modulusInt, "0", 10);
       mpz_inits(remainder, modulusInt, NULL);
    }

    void setX(mpz_t rem, mpz_t modint, int exponent, int modexponent, int blocksize) {
        mpz_add (remainder, remainder, rem);
        mpz_add (modulusInt, modulusInt, modint);
        exponent    = exponent;
        modexponent = modexponent;
        blocksize   = blocksize;
    }
 
    // Member Functions() 
    void printname() 
    { 
       std::cout << "Filename is: " << filename << endl; 
    } 

    void decode()
    {
       std::cout << "Running decode modscan" << endl;

       int continueFlag = 1;

       while (continueFlag == 0)
       {
           
           // mpz_add (block_num, block_num, modulus);
       }
    }
}; 
