/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
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
#include <gmp.h>
#include <string>
#include <time.h>
#include <vector>
#include "external/CLI11.hpp" 
#include "mdMutex.h"
#include "mdMutexLog.h"
#include "modscan.h"
#include "string.h"
#include "stdio.h"

using namespace std;

unsigned char *genRandomByteBlock(size_t num_bytes);
unsigned char *convertHexToByteBlock(const std::string & source);
unsigned char *setByteBlock(size_t num_bytes);
int calcExponent (mpz_t blockint);
int calcExponentModulus (mpz_t modulus, mpz_t blockint);
void printByteblock(unsigned char *byteblock, int blocksize, bool ishex);
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount, std::string& displayHLhashes, mdMutexLog *log );
void usage();

// main
int main (int argc, char **argv) {

     size_t blocksize = 12;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = 8;
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode GMP C++ Test Program"};
     app.add_option("-b,--block", blocksize, "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
     app.add_option("-m,--mod", modsize, "Modulus size number")->check(CLI::PositiveNumber);
     app.add_option("-t,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);
     //app.add_option("-v,--version", version, "Version number");

     // add a hashlist parameter
     // std::string hashlist;
     std::vector<int> def = { 1 };
     std::vector<int> vals;
     std::vector<int> csvvals;
     // std::vector<std::string> csvvals;
     // app.add_option("-r,--bh", vals, "Block Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber);
     app.add_option("-r,--bh", csvvals, "Block Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));
     // app.add_option("-r,--bh", csvvals, "Block Hashlist csv string")->delimiter(',');

     // std::vector<int> vals;
     app.add_option("-s,--hl", vals, "Block Hashlist integers list")->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

     // add a hash keylist parameter
     std::string keylist;
     app.add_option("-k,--keylist", keylist, "Keylist csv string");

     // process the hex byte arguments
     std::string hexstring; 
     // I think CLI11 uses -h for help so I can't use -h
     app.add_option("-x,--hex", hexstring, "Hex Byteblock string");

     bool runlogging = false;
     app.add_option("-l,--log", runlogging, "Run Logging");

     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     csvvals.insert(csvvals.end(), vals.begin(), vals.end());
     auto it = unique(begin(csvvals), end(csvvals));
     csvvals.erase(it, end(csvvals));
     // need to unique this list for duplicates
     if (csvvals.size() > 0) {
         def.clear();
         def.insert(def.end(), csvvals.begin(), csvvals.end());
     }

     // for(string v  : csvvals)
     std::cout << "hash values ";
     for(int v2  : def)
     std::cout << " " << v2 << " ";
     std::cout << std::endl;
/*
     cout << endl << "csv vals ";
     for(int v  : vals)
        std::cout << ": " << v << " ";
     std::cout << std::endl;
*/

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

     // display the current block stats
     // displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize, threadcount, runlogging );

     // initialize the mutex object
     // I should set a result variable and pass it into the mutex
     // it result = 0 then the mutex can set it and stop the execution for the mod scan
     mdMutex mutex;
     mdMutexLog log(runlogging);

     // initialize the modulus scan array
     // this allows it to run mulithreaded 
     modscan* mst = new modscan[threadcount];
     for(int tnum = 0; tnum < threadcount; tnum++) {
         mst[tnum].setModscan(&log, byteorder, endian, remainder, modulusInt, exp, expmod, blocksize, tnum, threadcount, &mutex);
         // set the hash context list and the signatures based on the current byte block
         mst[tnum].hcl.setBlockHashList(def);
         mst[tnum].hcl.setBlockHashList(byteblock, blocksize);
     } 

     // initialize the modulus scan threads vector
     std::vector<std::thread> threads;
     for(int tnum = 0; tnum < threadcount; tnum++){
        threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
     }

     // display the current block stats
     std::string hashlist = mst[0].hcl.displayHLhashes();
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize, threadcount, hashlist, &log );

     // std::cout << endl << "Running decode modscan" << endl << endl;
     log.writeLog("Running decode modscan");

     // start the count
     auto start = std::chrono::high_resolution_clock::now();

     // execute the threads
     for(int tnum = 0; tnum < threads.size(); tnum++)
     {
        threads.at(tnum).detach();
     } 

     // need to change this to three states
     // found     = 0
     // not found = 1
     // found     = 2
     // Maybe while (result == 0) {
     while (mutex.getIsMatched() == false) {

     }

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
          log.logMatchByteblock(byteblock, blocksize, true);
          log.logMatchByteblock(modbyteblock, blocksize, true);

          log.logMatchByteblock(byteblock, blocksize, false);
          log.logMatchByteblock(modbyteblock, blocksize, false);
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
unsigned char *convertHexToByteBlock(const std::string & source) {

    unsigned char *stream;
    size_t num_bytes = (source.length() / 2);
    stream = (unsigned char *) malloc(num_bytes * sizeof(unsigned char));

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

// calculates an exponent of the modulus less than the byte block int
int calcExponentModulus (mpz_t modulus, mpz_t blockint) {
    int exponent = 0;

    mpz_t result;

    mpz_init_set_str(result, "", 10);
    mpz_add (result, result, modulus); 


    do {
      mpz_mul(result, result, modulus);
      exponent++;
    } while(mpz_cmp(result,blockint) < 0);

    mpz_clear(result);

    return exponent;
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
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount, std::string& hashlist, mdMutexLog *log) {

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

     result << endl;

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
     result << endl;

     // write the byteblock integer number
     result << "Random Byteblock Int     ";
     for (f = 0; f < blocksize; f++) {
          result << setw(4) <<  std::left << setfill(' ') << std::to_string(byteblock[f]);
     }

     result << endl;
     result << "Random Byteblock Bigint  ";
     char *data;
     // mpz_out_str(stdout, 10, blockint);
     data = mpz_get_str(NULL, 10, blockint);
     result << data << endl; 
     // gmp_printf("%Zd\n", blockint);
     // cout << endl;
     free(data);

     result << "Modulus Size             " << std::to_string(modsize) << endl;

     result << "Modulus Bigint           ";
     // gmp_printf("%Zd", modint);
     data = mpz_get_str(NULL, 10, modint);
     result << data << endl;
     free(data);

     result << "Modulus Remainder        ";
     // gmp_printf("%Zd", remainder);
     data = mpz_get_str(NULL, 10, remainder);
     result << data << endl;
     free(data);

     result << "Modulus 2   ^ Exponent   " << std::to_string(exponent) << endl;
     result << "Modulus Mod ^ Exponent   " << std::to_string(expmod) << endl;

     result << "Block Signatures         ";
     result << hashlist;
     result << endl;

     result << "Thread Count             " << std::to_string(threadcount) << endl;

     result << "Logging                  " << boolalpha << log->checkIfLogging() << endl;
     result << endl;

     log->writeLog(result.str());
}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   ./decoderRandomTestHCthreads_gmp -b 12 -m 64 -t 16
   ./decoderRandomTestHCthreads_gmp --block=12 --mod=64    --threads=16
   ./decoderRandomTestHCthreads_gmp --block=12 --mod=128   --threads=16
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=0011
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --hl 1 2 3 4 5
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --bh 1,5,7
)";

    cout << usageline << endl;
    cout << endl; 

    cout << "Hashlist" << endl << endl;

    mdHashContextList hcl;
    hcl.displayHashList(0);

}
