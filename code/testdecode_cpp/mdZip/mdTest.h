/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode C++ Modulus Scan Test Decode Object
 * mdTest.h
 *
 * 
 * MDencode mdtest uses a hash context list configured with the command arguments to run against a modulus scan 
 *
 * This includes the MD2 and MD4 and MD5 and MD6 signatures in the hash context list.
 *
 * Paramters: modulus size, threadnumber, blocksize and hashlists and hashlist keys (TODO), hexbytes, enable logging
 * 
 */
#include <stdlib.h>
#include <algorithm>
#include <chrono> 
#include <ctime>
#include <iostream>
#include <sstream>
#include "stdio.h"
#include <gmp.h>
#include <gmpxx.h>
#include <string>
#include <time.h>
#include <thread>
#include <vector>
#include "mdCore/mdCommon.h"
#include "mdCore/mdMutex.h"
#include "mdCore/mdMutexLog.h"
#include "mdCore/mdHashContextList.h"
#include "mdCore/modscan.h"

class mdTest 
{
private:
    // test variables
    int threadcount       = 8;
    // byteclock variables
    std::string    hexstring; 
    unsigned char *byteblock;
    long blocksize        = 10;
    // modulus variables
    int modsizebits       = 32; // should make this modsizeBits
    int modsizeBytes      = 4;
    int modexponent       = 0;  // 2 ^ n less than the byteblock int
    int modexponent2      = 0;  // the modulus to the exponent less than the byteblock int
    // hash context list variables
    // hcl placeholder
    // hash list variables
    std::string blockhashnames;
    std::vector<int> bhlist;
    std::string blockkeys  = "default";
    std::string vectorlist;
    std::string hashlist;
    // boolean variables
    bool randombh          = false;
    bool skipDecode        = false;
    // logging variables
    bool runlogging        = false;
    // initialize the gmp bigint variables
    int byteorder = 0;
    int endian    = 0;
    size_t count;
    mpz_t byteblockInt, modulusInt, remainder;
    // initialize the mutex and log objects
    mdMutex    mutex{8};
    mdMutexLog log{false};
 public:    

    // initialize the mdzip object
    mdTest() {
    }

    // initialize the mdTest object data
    mdTest(std::string hexString, size_t blockSize, int modsizeBits, bool randomBH, std::vector<int> &bhList, int threadCount, bool skip, bool runLogging) {
        hexstring   = hexString;         
        blocksize   = blockSize;
        modsizebits = modsizeBits;
        threadcount = threadCount;
        randombh    = randomBH;
        bhlist      = bhList;
        skipDecode  = skip;
        runlogging  = runLogging;

        setModulus();
        setByteBlock();
                
        // initialize the mutex and log object
        mutex.setThreadCount(threadcount);
        log.setLogging(runlogging);
    }

    // Destructor
    ~mdTest() {
    } 
    
     // get the file block size
    long getBlockSize() {
        return blocksize;
    }

    // initialize the test uint8_t byteblock 
    void setByteBlock() {
          
        // generate a random n byte byteblock if the hexstring is empty
        if (hexstring.empty()) {
            genRandomByteBlock(blocksize);
        // otherwise process the hex string into a byte block
        } else {
            convertHexToByteBlock(hexstring);
            blocksize = hexstring.length() / 2;
        }
    }  

    // get the random byteblock
    // this returns a pointer to the byteblock
    unsigned char *getByteBlock() {
        return byteblock;
    }

    // set the modulus byte size
    void setModulus() {

        if (modsizebits == 0) {
            modsizeBytes = 0;
            return;    
        }    
 
        if ((modsizebits % 8) == 0)
        {
            modsizeBytes = modsizebits / 8;
        }
        else if (modsizebits < 8)
        {
            modsizeBytes = 1;
        }
        else
        {
            modsizeBytes = (modsizebits / 8) + 1;
        }
    }

    // get the modulus bit size
    long getModulusBitsSize() {
        return modsizebits;
    }

    // get the modulus byte size
    long getModulusBytesSize() {
        return modsizeBytes;
    }

    // set the modulus exponent size
    void setModulusExponent(long exponent) {
        modexponent = exponent;
    }

    // get the modulus exponent number
    long getModulusExponent() {
        return modexponent;
    }
 
    // decodes a random byte sized byteblock with a group of signatures and a random signature key
    int decodeRandomBlock () {

        mpz_init_set_str(byteblockInt, "0", 10);
        mpz_init_set_str(modulusInt, "1", 10);
        mpz_init_set_str(remainder, "0", 10);
        
        // create the byteblock bigint
        // void mpz_import (mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op) 
        // https://machinecognitis.github.io/Math.Gmp.Native/html/8c8c1e55-275f-cff8-2152-883a4eaa163c.htm
        /*
            public:
            static void mpz_import(
            mpz_t^ rop, 
            size_t count, 
            int order, 
            size_t size, 
            int endian, 
            size_t nails, 
            void_ptr op )
        */
        // the import is currently byteorder most significant and native endian 
        // the modscan has to match the byte order and endian paramters
        // set byte order and endian once and then set them in modscan object so it matches and can be set once
        mpz_import (byteblockInt, blocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);  

        // calculate the modulusInt
        // 2 ^ modsize - 1
        calcModulusInt(modulusInt, modsizebits);

        // calculate the modulus remainder 
        mpz_mod (remainder, byteblockInt, modulusInt); 

        // calculate the modulus exponent with two
        modexponent = calcExponent(byteblockInt);

        // calculate the modulus exponent with the modulus
        modexponent2 = calcExponentModulus(modulusInt, byteblockInt);

        // initialize the modulus scan array
        // this allows it to run mulithreaded 
        modscan* mst = new modscan[threadcount];
        for(int tnum = 0; tnum < threadcount; tnum++) {
    
            mst[tnum].setModscan(&log, byteorder, endian, remainder, modulusInt, modexponent, modexponent2, blocksize, tnum, threadcount, &mutex);
    
            // set the hash context list and the signatures based on the current byte block
            mst[tnum].hcl.setVectorHL(bhlist);
            
            // randomize the keylist if the randombh is true
            // copy the keylist to the other modscan objects after the first modscan object keylist randomization
            if (randombh == true) { 
                // randomize the keylist on the first modscan object
                if (tnum == 0) {
                    mst[tnum].hcl.randomizeKeyList();

                    // initialize the block hash signature list for the first modscan object
                    mst[tnum].hcl.setByteBlockHashList(byteblock, blocksize);
                // copy the mdRegister struct to the other modscan objects
                } else {     
                    mst[tnum].hcl.hregister[0] = mst[0].hcl.hregister[0];
                }
            // set the modscan hashblock list without randomizing the keys    
            } else {

                // initialize the block hash signature list if the modscan object is the first
                if (tnum == 0) mst[tnum].hcl.setByteBlockHashList(byteblock, blocksize);
                // copy the mdRegister struct hash to the other mdRegister modscan structs from the first object
                if (tnum > 0)  mst[tnum].hcl.hregister[0] = mst[0].hcl.hregister[0];
            }
        } 

        // display the current block stats
        vectorlist = mst[0].hcl.getHLvectorsString();
        hashlist   = mst[0].hcl.displayHLhashes();

        // set the blockkeys to default if the random block hash keys is false
        // if the randombh is true set the blockkeys to the signature list 
        if (randombh == true) {
            blockkeys = mst[0].hcl.displayHLhashKeys();
        }  

        // display the current block stats after initialization
        // display the block keys
        displayFloor();

        if (skipDecode) {
            delete[] byteblock;
            delete[] mst;
            mpz_clear(remainder);
            mpz_clear(modulusInt);
            mpz_clear(byteblockInt);
            log.writeLog("Skipping decode modscan");
            return 0;      
        } 

        // initialize the modulus scan threads vector
        std::vector<std::thread> threads;
        for(int tnum = 0; tnum < threadcount; tnum++){
            threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
        }

        // std::cout << endl << "Running decode modscan" << endl << endl;
        log.writeLog("Running decode modscan");

        // start the count
        auto start = std::chrono::high_resolution_clock::now();

        // execute the threads
        for(int tnum = 0; tnum < threads.size(); tnum++)
        {
            threads.at(tnum).detach();
        } 

        // check the mutex ismatched for three states
        // searching = 0 // searching for the value with the modscan
        // not found = 1 // modscan mutext match result
        // found     = 2 // modscan mutext match result 
        while (mutex.getIsMatched() == SEARCHING) {

        } 

        // stop the threads on the first match
        for(int tnum = 0; tnum < threadcount; tnum++) {
            mst[tnum].stopThread();
            while (!mst[tnum].isStoppedThread()) { }
        } 

        // if the result is not found display not found   
        if (mutex.getIsMatched() == NOTFOUND) {
                log.writeLog("Modulus Scan Match Not Found");
                // break; // need to check the other blocks
        // match is found   
        } else {

            int threadMatchNumber = mutex.getMatchThread(); 

            // calculate the duration time
            auto stop = std::chrono::high_resolution_clock::now(); 
            // auto hours = std::chrono::duration_cast<std::chrono::hours>(stop - start); 
            // auto min = std::chrono::duration_cast<std::chrono::minutes>(stop - start); 
            double elapsed_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());

            // check the modulus scan results
            std::ostringstream result; 
            unsigned char *modbyteblock;
            modbyteblock = mst[threadMatchNumber].getModscanByteBlock();

            if (memcmp(modbyteblock, byteblock, blocksize) == 0) {
                result << endl << "Found Match" << endl << endl;
                result << "Elapsed Time (s) " << std::to_string(elapsed_time/1e9) << endl;
                result << "Modulus Scan thread " << threadMatchNumber << " and Random byteblock match"; // << endl;
                log.writeLog(result.str());

                // log.logMatchByteblock(byteblock, blocksize, true);
                // log.logMatchByteblock(modbyteblock, blocksize, true);

                log.logMatchByteblock(byteblock, blocksize, false);
                log.logMatchByteblock(modbyteblock, blocksize, false);

            } else {
                result << "Modulus Scan and Random byteblock don't match" << endl;
                log.writeLog(result.str());

                log.logMatchByteblock(byteblock, blocksize, true);
                log.logMatchByteblock(modbyteblock, blocksize, true);

                log.logMatchByteblock(byteblock, blocksize, false);
                log.logMatchByteblock(modbyteblock, blocksize, false);
            }
        }    

        /* free used memory */
        delete[] byteblock;

        // delete the modscan objects 
        delete[] mst;

        mpz_clear(remainder);
        mpz_clear(modulusInt);
        mpz_clear(byteblockInt);
        // mpz_clears(remainder, modulusInt, byteblockInt, NULL);

        return 0;
    }

    // creates a random number byte sized byteblock
    void genRandomByteBlock(size_t num_bytes) {

        byteblock = new unsigned char [num_bytes];

        srand(time(0));

        for (int f = 0; f < num_bytes; f++) {
            byteblock[f] = (rand() % 255);
        }
    }

    // convert a hex byte string argument to unsigned byte byteblock array
    // it should also check 00FF33 or 0000FFFF7873 hex strings
   void convertHexToByteBlock(std::string &source) {

        if ((source.length() % 2) == 1) source = source + "0";
        size_t num_bytes = (source.length() / 2);
        byteblock = new unsigned char [num_bytes];

        int f = 0;
        for (unsigned int i = 0; i < source.length(); i += 2) {
            std::string byteString = source.substr(i, 2);
            unsigned char byte = (char) strtol(byteString.c_str(), NULL, 16);
            byteblock[f] = byte;
            f++;
        }
    }


    // displays the modulus scan information
    void displayFloor() {   

        std::ostringstream result;
        int f = 0;
        // current date/time based on current system
        time_t now = time(0);
    
        // convert now to string form
        char* dt = ctime(&now);

        result << endl;
        result << "Start Time               " << dt;
        result << "Block Size               " << blocksize << endl;

        result << "Random Byteblock         ";
        for (f = 0; f < blocksize; f++) {
            result << setw(2) << std::uppercase << std::hex << setfill('0') << (int)byteblock[f];
        }
        result << endl;

        // write the byteblock number header
        result << "                         ";
        for (f = 0; f < blocksize; f++) {
            result << setw(4) <<  std::left << setfill(' ') << std::to_string((f + 1));
        }

        result << std::endl;

        // write the byteblock hex
        result << "Random Byteblock Hex     ";
        int pad = 4;
        for (f = 0; f < blocksize; f++) {
            if ((int)byteblock[f] < 16) {
                result << "0";
                pad = 3;
            }
            result << setw(pad) << std::uppercase << std::hex << setfill(' ') << (int)byteblock[f];
            pad = 4;
        }
        result << std::endl;

        // write the byteblock integer number
        result << "Random Byteblock Int     ";
        for (f = 0; f < blocksize; f++) {
            result << setw(4) <<  std::left << setfill(' ') << std::to_string(byteblock[f]);
        }

        result << std::endl;

        // display the byteblock bigint
        mpz_class blockBigInt(byteblockInt);
        result << "Random Byteblock Bigint  " << blockBigInt.get_str() << std::endl;
        result << "Modulus Size             " << std::to_string(modsizebits) << std::endl;

        // display the modulus bigint
        mpz_class modBigInt(modulusInt);
        result << "Modulus Bigint           " << modBigInt.get_str() << std::endl;

        // display the modulus remainder
        mpz_class modRemainder(remainder);
        result << "Modulus Remainder        " << modRemainder.get_str() << std::endl;

        result << "Modulus 2   ^ Exponent   " << std::to_string(modexponent) << std::endl;
        result << "Modulus Mod ^ Exponent   " << std::to_string(modexponent2) << std::endl;
        
        result << "Block Signatures         ";
        result << hashlist << std::endl;
        
        // result << std::left << std::setw(20) << "Blockkeylist: "   << blockkeys << std::endl;
        result << "Blockkeylist             " << blockkeys << std::endl;    
        result << "Thread Count             " << std::to_string(threadcount) << std::endl;
        result << "Logging                  " << boolalpha << log.checkIfLogging() << std::endl << std::endl;
        result << "Hash Block Vector" << std::endl;
        result << vectorlist;

        // write the modulus scan data to the log
        log.writeLog(result.str());
    }

};