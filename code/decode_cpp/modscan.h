#include <bits/stdc++.h>

using namespace std;

class modscan 
{ 
    private:
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
       mpz_inits(remainder, modulusInt, modulusExpInt, blockInt, NULL);
    }

    // Destructor
    ~modscan()
    {
       std::cout << "Destroying modscan" << endl;
       mpz_clears(remainder, modulusInt, modulusExpInt, blockInt, NULL);
       delete []byteblock;
    }

    void setModscan(mpz_t rem, mpz_t modint, int exponent, int modexponent, int blocksize) {
        mpz_add (remainder, remainder, rem);
        mpz_add (modulusInt, modulusInt, modint);
        mpz_add (blockInt, blockInt, remainder);
        exponent    = exponent;
        modexponent = modexponent;
        blocksize   = blocksize;

        byteblock = new unsigned char[blocksize+1];
    }
 
    void printname() 
    { 
       std::cout << "Filename is: " << filename << endl; 
    } 

    void decode()
    {
       std::cout << "Running decode modscan" << endl;

       size_t count;
       int continueFlag = 1;

       // calculate the modulus raised to the exponent
       // mpz_pow_ui (modulusExpInt, 2, modsize);
       mpz_pow_ui (modulusExpInt, modulusInt, modexponent);

       // add the modulus ^ modulus exponent to the block int 
       mpz_add (blockInt, blockInt, modulusExpInt);

       mpz_export   (byteblock, &count, -1, sizeof(byteblock[0]), 0, 0, blockInt);

       while (continueFlag == 0)
       {
           
           // mpz_add (blockInt, blockInt, modulusInt);
       }
    }
}; 
