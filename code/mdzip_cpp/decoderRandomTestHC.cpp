/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * decoderRandomTestHC.cpp
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
#include <vector>
#include "external/CLI11.hpp" 
#include "mdCore/mdCommon.h"
#include "mdCore/mdMutex.h"
#include "mdCore/mdMutexLog.h"
#include "mdCore/mdHashContextList.h"
#include "mdCore/modscanFile.h"
#include "string.h"


using namespace std;

unsigned char *genRandomByteBlock(size_t num_bytes);
unsigned char *convertHexToByteBlock(std::string & source);
unsigned char *setByteBlock(size_t num_bytes);
int decodeRandomBlock (size_t blocksize, int modsize, bool randombh, std::vector<int> &bhlist, unsigned char *byteblock, 
                       int threadnumber, int threadcount, bool runlogging);
void printByteblock(unsigned char *byteblock, int blocksize, bool ishex);
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, 
                  int expmod, int blocksize, int threadcount, std::string& vectorlist, std::string& displayHLhashes,  std::string& blockkeys, 
                  mdMutexLog *log );
void usage();

// main
int main (int argc, char **argv) {

     size_t blocksize = 12;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = std::thread::hardware_concurrency();
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode GMP C++ Test Program"};
     app.add_option("-b,--block", blocksize, "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
     app.add_option("-m,--mod", modsize, "Modulus size number")->check(CLI::PositiveNumber);
     app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);
     //app.add_option("-v,--version", version, "Version number");

     // add the block hashlist parameter
     // csv hash list    
     std::vector<int> csvvals;
     app.add_option("-r,--bh", [&csvvals, &signum](std::vector<std::string> val){
        return splitRange(val, csvvals, signum);
     }, "Block Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

     // integer block hash list
     std::vector<int> intvals;
     app.add_option("-s,--hl", [&intvals, &signum](std::vector<std::string> val){
         return splitRange(val, intvals, signum);
     }, "Block Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

     // randomize the keylist for the block hashes
     bool randombh = false;
     app.add_flag("--randbh", randombh, "Randomize the Block Hash Keylist");

     // process the hex byte arguments
     std::string hexstring; 
     // I think CLI11 uses -h for help so I can't use -h
     app.add_option("-x,--hex", hexstring, "Hex Byteblock string");

     bool runlogging = false;
     app.add_flag("-l,--log", runlogging, "Run Logging");

     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     // combine csvvals and the integer vals block hash list
     // the hash context list will unique them in non sorted order
     csvvals.insert(csvvals.end(), intvals.begin(), intvals.end());

     // if the hash context list is empty assign a default value
     if (csvvals.size() == 0) {
        csvvals = { FAST64 };
     }    

     // check if the argument count is less than 2
     // then display the usage
     if (argc < 2)
     {
         cout << app.help() << endl;
         usage();
         return 0;
     }
    
     unsigned char *byteblock;
     // generate a random n byte byteblock if the hexstring is empty
     if (hexstring.empty()) {
        byteblock = genRandomByteBlock(blocksize);
     // process the hex string into a byte block
     } else {
        byteblock = convertHexToByteBlock(hexstring);
        blocksize = hexstring.length() / 2;
     }

     // set a predefined byte block for testing
     // byteblock = setByteBlock(blocksize);

     // run the modulus scan decode on the byteblock
     decodeRandomBlock(blocksize, modsize, randombh, csvvals, byteblock, threadnumber, threadcount, runlogging);

     return 0;
}

// decodes a random byte sized byteblock with a group of signatures and a random signature key
int decodeRandomBlock (size_t blocksize, int modsize, bool randombh, std::vector<int> &bhlist, unsigned char *byteblock, 
                       int threadnumber, int threadcount, bool runlogging) {

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
     int byteorder = 1;
     int endian    = 0;
     mpz_import (byteblockInt, blocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);  

     // calculate the modulus 2 ^ modsize 
     mpz_ui_pow_ui (modulusInt, 2, modsize);

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

     // initialize the modulus scan threads vector
     std::vector<std::thread> threads;
     for(int tnum = 0; tnum < threadcount; tnum++){
        threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
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
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize, threadcount, vectorlist, hashlist, blockkeys, &log );

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
     free (byteblock);
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);
     // mpz_clears(remainder, modulusInt, byteblockInt, NULL);



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

// convert hex bytes to byte array
// it should also check 00FF33 or 0000FFFF7873 hex strings
unsigned char *convertHexToByteBlock(std::string & source) {

    unsigned char *stream;
    if ((source.length() % 2) == 1) source = source + "0";
    size_t num_bytes = (source.length() / 2);
    stream = (unsigned char *) malloc(num_bytes * sizeof(unsigned char));

    // TODO eliminate this vector and just set these
    std::vector<unsigned char> bytes;


    for (unsigned int i = 0; i < source.length(); i += 2) {
      std::string byteString = source.substr(i, 2);
      char byte = (char) strtol(byteString.c_str(), NULL, 16);
      bytes.push_back(byte);
    }

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = bytes[f];
    }


    return stream;

}

// returns a predefined test byteblock
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


// display the byteblock
void printByteblock(unsigned char *byteblock, int blocksize, bool ishex) {

        int i;
        for(i=0;i<blocksize;i++)
        {
            if (ishex == false) {
                printf("%d ",    byteblock[i]);
            } else {
                printf("%02X  ", byteblock[i]);
            }
        }

        printf("\n");
}


// displays the modulus scan information
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, 
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
     result << "Modulus Size             " << std::to_string(modsize) << std::endl;

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

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   decoderRandomTestHC2 -b 12 -m 64 -t 16
   decoderRandomTestHC2 --block=12 --mod=64    --threads=16
   decoderRandomTestHC2 --block=12 --mod=128   --threads=16
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=0011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --hl 1 2 3 4 5
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --bh 1,5,7
   decoderRandomTestHC2 --mod=64 --threads=32  --hl 1 2 3 4 5 --randbh --block=12
   decoderRandomTestHC2 --mod=64 --threads=32 --bh=6-8,23,33,34 --hl 1 2 3 4-8 --randbh --block=12
)";

    cout << usageline << endl;
    cout << endl; 

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}
