/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * 
 * modscanFile.h is the modulus scan mdunzip file class
 * 
 * 
 */
#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <gmp.h>

using namespace std;

class modscan 
{ 
    private:
       int byteorder; 
       int endian; 
       unsigned char *byteblock; 
       mdMutex* mutexref;
       mdMutexLog* log;
       bool stop;
       bool stopped;
    public: 
       string filename; 
       int exponent;
       int modexponent;
       int modsize;
       int blocksize;
       int threadnumber;
       int threadcount;
       bool isMatched;
       mpz_t two;
       mpz_t modremainder;
       mpz_t remainder;
       mpz_t modulusInt; 
       mpz_t modulusThreadInt; 
       mpz_t modulusExpInt; 
       mpz_t modulusExpIntCeil; 
       mpz_t blockInt;
       // hash context list
       mdHashContextList hcl;
 
    // Default constructor
    modscan() 
    {
       this->isMatched = false;
       // mpz_init_set_str(remainder,  "0", 10);
       // mpz_init_set_str(modulusInt, "0", 10);
       mpz_init_set_str(two, "2", 10); // exponent floor
       mpz_init_set_str(modremainder, "0", 10); // exponent floor
       mpz_init_set_str(modulusThreadInt, "0", 10); // exponent floor
       /////mpz_init_set_ui(modulusThreadInt, (unsigned long) threadnumber); // exponent floor
       mpz_init_set_ui(modulusThreadInt, 0);
       // mpz_init_set_ui(modulusThreadInt, threadnumber); // exponent floor
       // mpz_inits(remainder, modulusInt, modulusExpInt, blockInt, NULL);
       mpz_inits(remainder, modulusInt, modulusThreadInt, modulusExpInt, modulusExpIntCeil, NULL);
       // initialize the byte block bigint
       // mpz_init_set_str(blockInt, "0", 10);
       // mpz_init2 (blockInt, blocksize * 8); // For preallocating the size and padding
       mpz_init_set_str(blockInt, "0", 10);
    }

    // Destructor
    ~modscan()
    {
       std::cout << "Destroying modscan" << endl;
       mpz_clears(two, modremainder, remainder, modulusInt, modulusThreadInt, modulusExpInt, modulusExpIntCeil, blockInt, NULL);

       if (sizeof(byteblock) / sizeof(byteblock[0]) != 0) {
           cout << "Free byteblock" << endl;
           delete []byteblock;
       }
    }

    void setModscan(mdMutexLog *mdmutexlog, int bytesorder, int end, mpz_t rem, mpz_t modint, int exp, int modexp, int blocks, int threadnum, int threadcnt, mdMutex *mdmutex) {

        log      = mdmutexlog;
        mpz_add (remainder, remainder, rem);
        mpz_add (modulusInt, modulusInt, modint);
        mpz_add (blockInt, blockInt, remainder);

        mutexref     = mdmutex;
        byteorder    = bytesorder;
        endian       = end;
        exponent     = exp;
        modexponent  = modexp;
        blocksize    = blocks;
        threadnumber = threadnum;
        threadcount  = threadcnt;
        stop         = false;
        stopped      = false;
        
    // ==================================================================
    // test code
    /*if (threadnum == 0) {
        printf("modulus blocksize %d \n",  blocksize);
        printf("modulus exponent1 %d \n",  exponent);
        printf("modulus exponent2 %d \nmodulus int ",  modexponent);
        gmp_printf("modulus int %Zd\n", modulusInt);
        gmp_printf("modulus int remainder %Zd\n", remainder);
        gmp_printf("starting block int %Zd\n", blockInt);
    }*/
    // ==================================================================

        byteblock = new unsigned char[blocksize];


    }
 
    void printname() 
    { 
       std::cout << "Filename is: " << filename << endl; 
    } 

    int decode()
    {
       // std::cout << endl << "Running decode modscan" << endl << endl;

       size_t count;
       int lineNum = 0;
       int lineCount = 100000000;
       lineCount = lineCount + (1000000 * (threadnumber + 1));

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

       // set the exponent ceiling
       mpz_pow_ui (modulusExpIntCeil, two, exponent + 1);

       // add the modulus ^ modulus exponent to the block int 
       mpz_add (blockInt, blockInt, modremainder);

       // set the modulusThreadInt which is zero
       // add the modulusThreadInt + threadnumber
       // it expects the threadnumber argument to be an unsigned long int or it doesn't multiply correctly
       mpz_add_ui (modulusThreadInt, modulusThreadInt, (unsigned long int) threadnumber);

       // multiply the modulusInt * the modulusThreadInt which was previously set to the threadnumber
       mpz_mul (modulusThreadInt, modulusThreadInt, modulusInt);
       
       // add the modulusThreadInt to the starting blockInt
       mpz_add (blockInt, blockInt, modulusThreadInt); 
   
       // multiply the modulusInt * threadcount
       // the threadcount must be 1 or greater
       // mpz_mul_si (mpz_t rop, const mpz_t op1, long int op2)
       mpz_mul_si (modulusInt, modulusInt, (long int) threadcount);

       // need to check the initial floor for 0011 byte block
       // gmp_printf("\nmodscan blockint modulus incrementer %Zd", blockInt);
       // cout << endl;

       // log the initial thread floor 
       // should add some initial logging here for the initial blockInt
       if (log->checkIfLogging()) log->mdMutexLog::writeLogThreadFloor(threadnumber, threadcount, modulusInt, modulusThreadInt);

       int n;
       int diff;

       while (stop == false)
       {
           // byte order and endian parameters must match the import byte block 
           // currently byte order msb and native endian
           mpz_export(byteblock, &count, byteorder, sizeof(byteblock[0]), endian, 0, blockInt);
           // mpz_export(byteblock, &count, 0, sizeof(byteblock[0]), 0, 0, blockInt); // faster and compatable with the go version - I think go uses msb I padded the byte array
           // mpz_export(byteblock, &count, -1, sizeof(byteblock[0]), 0, 0, blockInt); // works with padding 00FFDD but slower 

           // handle blocks that start with a zero byte
           // pad 00 00 11 11 byte blocks
           // convert 11110000 to 00001111
           if (count < blocksize) {
               diff = blocksize - count;
               for (n = (blocksize - diff); n >= 0; n--) byteblock[n+diff] = byteblock[n];
               for (n = 0; n < diff; n++) byteblock[n] = 0;
           }

           // check the hash context list
           if (hcl.compareBlockHashList(byteblock, blocksize)) {
               this->isMatched = true;
               mutexref->mdMutex::setMatched(threadnumber);
               break;
           } else {
              if (lineNum > lineCount) {
                 lineNum = 0;
                 log->mdMutexLog::writeLogThread(threadnumber, blockInt);

                 // Check the ceiling
                 // If it is greater than the mod ^ exp + 1 (the ceil) break
                 // and increment or call the mutex not found 
                 // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2
                 if (mpz_cmp(modulusExpIntCeil, blockInt) < 0) {
                    mutexref->mdMutex::incNotFound();
                    break;
                 }
              }
           }
          
           // increment by the modulusInt 
           mpz_add (blockInt, blockInt, modulusInt);
           lineNum++;
        }

       stopped = true; 
       return 0;

    }

    // converts 2 ^ exp to the modulus floor ^ exp
    void convertFloorBase2(mpz_t modfloor, mpz_t modint) {
        mpz_t mfloor;
        mpz_t zero;

        mpz_init_set_str(mfloor, "0", 10);
        mpz_init_set_str(zero, "0", 10);
        mpz_init_set_str(modremainder, "0", 10);

        mpz_add (mfloor, mfloor, modfloor);
        mpz_mod (modremainder, modfloor, modint);
        int cmp = mpz_cmp(modremainder,zero);

        if (cmp == 0) {
          mpz_set(modremainder, mfloor);
        } else {
          mpz_sub(modremainder, modremainder, modfloor);

        }

        mpz_clear(mfloor);
        mpz_clear(zero);
 
    }

    // get the mod scan byte block
    unsigned char* getModscanByteBlock()
    {
        return byteblock; 
    }

    // check if the thread is stopped
    bool isStoppedThread() {
        
        return stopped;
    }

    // Stop the thread
    void stopThread() {
        // std::cout << "Stopping The thread " << threadnumber << " has stopped..." << std::endl;
        stop = true;
    }

    // void resetThread( mpz_t rem, mpz_t modint, int modexp, int blocks) {
    void resetThread( mpz_t rem, mpz_t modint, int modexp, int blocks, int threadnum) {
        stop        = false;
        stopped     = false;
        exponent    = modexp;
        modexponent = 1;

        // check if this is the last byteblock less than the blocksize
        if (blocks < blocksize) {
            delete []byteblock;
            byteblock = new unsigned char[blocks];
        }

        blocksize    = blocks;

        mpz_init_set_str(remainder, "0", 10); 
        mpz_init_set_str(modremainder, "0", 10); // exponent floor
        mpz_init_set_str(modulusInt, "0", 10);
        mpz_init_set_str(blockInt, "0", 10);
        mpz_init_set_ui(modulusThreadInt, 0);

        mpz_add (remainder, remainder, rem);
        mpz_add (modulusInt, modulusInt, modint);
        mpz_add (blockInt, blockInt, remainder);

/*        if (threadnum == 0) {
        printf("modulus blocksize %d \n",  blocksize);
        printf("modulus exponent1 %d \n",  exponent);
        printf("modulus exponent2 %d \nmodulus int ",  modexponent);
        gmp_printf("modulus int %Zd\n", modulusInt);
        gmp_printf("modulus int remainder %Zd\n", remainder);
        gmp_printf("starting block int %Zd\n", blockInt);
        }
*/
    }

}; 
