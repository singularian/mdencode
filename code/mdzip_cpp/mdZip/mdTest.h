/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode C++ Modulus Scan Test Decode Object
 * mdTest.h
 *
 * This is a C++ GMP modulus scan multithreaded test program 
 * MDencode GMP requires the GMP Library to build https://gmplib.org/ and the openssl library
 * It also requires the libfnv library to be built and installed for the FNV1 signature
 * MDencode uses a hash context list configured with the command arguments to run against a modulus scan 
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
#include "external/CLI11.hpp" 
#include "mdCore/mdCommon.h"
#include "mdCore/mdMutex.h"
#include "mdCore/mdMutexLog.h"
#include "mdCore/mdHashContextList.h"
#include "mdCore/modscanFile.h"

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
    int modexponent       = 0;
    long modByteBlockSize = 0; // mdzip bitstream modulus exponent byte size
    // hash context list variables
    int hclblocksize      = 0; // redundant
    int hclblockblocksize = 0; // hash file size
    int hclblockkeysize   = 0; // variable for mdunzip 
    // hash list variables
    std::string blockhashnames;
    std::vector<int> bhlist;
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

    // initialize the mdText object data
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
    
    // set the block size
    void setBlockSize(long bsize) {
        blocksize = bsize;
    }

    // get the file block size
    long getBlockSize() {
        return blocksize;
    }

    // initialize the test uint8_t byteblock 
    void setByteBlock() {
          
        // generate a random n byte byteblock if the hexstring is empty
        if (hexstring.empty()) {
            byteblock = genRandomByteBlock(blocksize);
        // otherwise process the hex string into a byte block
        } else {
            byteblock = convertHexToByteBlock(hexstring);
            blocksize = hexstring.length() / 2;
        }
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

    // get the modulus bit size
    long getModulusBytesSize() {
        return modsizebits;
    }

    // set the modulus exponent size
    void setModulusExponent(long exponent) {
        modexponent = exponent;
    }

    // get the modulus exponent size
    long getModulusExponent() {
        return modexponent;
    }
 
    // decodes a random byte sized byteblock with a group of signatures and a random signature key
    int decodeRandomBlock () {

        mpz_t remainder, modulusInt, byteblockInt;

        mpz_init_set_str(remainder, "0", 10);
        mpz_init_set_str(modulusInt, "1", 10);
        mpz_init_set_str(byteblockInt, "0", 10);

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
        int exp = calcExponent(byteblockInt);

        // calculate the modulus exponent with the modulus
        int expmod = calcExponentModulus(modulusInt, byteblockInt);

        // initialize the mutex object
        // if result not equal to searching then the modscan can set the mutex result and stop the execution for the mod scan
        mdMutex mutex(threadcount);
        mdMutexLog log(runlogging);

        // initialize the modulus scan array
        // this allows it to run mulithreaded 
        modscan* mst = new modscan[threadcount];
        for(int tnum = 0; tnum < threadcount; tnum++) {
    
            mst[tnum].setModscan(&log, byteorder, endian, remainder, modulusInt, exp, expmod, blocksize, tnum, threadcount, &mutex);
    
            // set the hash context list and the signatures based on the current byte block
            mst[tnum].hcl.setVectorHL(bhlist, HASHBLOCK);
            
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
        std::string vectorlist = mst[0].hcl.getHLvectorsString(HASHBLOCK);
        std::string hashlist   = mst[0].hcl.displayHLhashes();

        std::string blockkeys = "default"; 
        if (randombh == true) {
            blockkeys = mst[0].hcl.displayHLhashKeys();
        }  

        // display the current block stats after initialization
        // display the block keys
        displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsizebits, exp, expmod, blocksize, threadcount, vectorlist, hashlist, blockkeys, &log );

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

    // returns a random byte sized byteblock
    unsigned char *genRandomByteBlock(size_t num_bytes) {

        byteblock = new unsigned char [num_bytes];

        srand(time(0));

        for (int f = 0; f < num_bytes; f++) {
        byteblock[f] = (rand() % 255);
        }

        return byteblock;
    }

    // convert a hex byte string argument to unsigned byte byteblock array
    // it should also check 00FF33 or 0000FFFF7873 hex strings
    unsigned char *convertHexToByteBlock(std::string &source) {

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

        return byteblock;

    }


    // displays the modulus scan information
    void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsizebits, int exponent, int expmod, 
                    int blocksize, int threadcount, std::string& vectorlist, std::string& hashlist, std::string& blockkeys, mdMutexLog *log) {

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
        mpz_class blockBigInt(blockint);
        result << "Random Byteblock Bigint  " << blockBigInt.get_str() << std::endl;
        result << "Modulus Size             " << std::to_string(modsizebits) << std::endl;

        // display the modulus bigint
        mpz_class modBigInt(modint);
        result << "Modulus Bigint           " << modBigInt.get_str() << std::endl;

        // display the modulus remainder
        mpz_class modRemainder(remainder);
        result << "Modulus Remainder        " << modRemainder.get_str() << std::endl;

        result << "Modulus 2   ^ Exponent   " << std::to_string(exponent) << std::endl;
        result << "Modulus Mod ^ Exponent   " << std::to_string(expmod) << std::endl;
        
        result << "Block Signatures         ";
        result << hashlist << std::endl;
        
        // result << std::left << std::setw(20) << "Blockkeylist: "   << blockkeys << std::endl;
        result << "Blockkeylist             " << blockkeys << std::endl;    
        result << "Thread Count             " << std::to_string(threadcount) << std::endl;
        result << "Logging                  " << boolalpha << log->checkIfLogging() << std::endl << std::endl;
        result << "Hash Block Vector" << std::endl;
        result << vectorlist;


        log->writeLog(result.str());
    }

};