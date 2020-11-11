/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * 
 * 
 */

#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <gmp.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm> 
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
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount, std::string& displayHLhashes, bool log );
void usage();

/* 
   This is a C++ GMP modulus scan multithreaded test program 
   MDencode GMP requires the GMP Library to build https://gmplib.org/
   This program uses a Hash Context List with SHA1-64, SHA1-128 and SHA1 and Highway Hash 64
   In the future the Hash Context list will include more signatures
*/
int main (int argc, char **argv) {

     size_t blocksize = 13;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = 8;
     // current signature number handled in the hash context list
     int signum       = 12;

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
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize, threadcount, hashlist, runlogging );

     // std::cout << endl << "Running decode modscan" << endl << endl;
     log.writeLog((char *) "Running decode modscan");

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


     // check the modulus scan results
     unsigned char *modbyteblock;
     // modbyteblock = ms.getModscanByteBlock();
     modbyteblock = mst[threadMatchNumber].getModscanByteBlock();
     if (memcmp(modbyteblock, byteblock, blocksize) == 0) {
          cout << "Modulus Scan thread " << threadMatchNumber << " and Random byteblock match" << endl;
          printByteblock(byteblock, blocksize, false);
          printByteblock(modbyteblock, blocksize, false);
     } else {
          cout << "Modulus Scan and Random byteblock don't match" << endl;
          printByteblock(byteblock, blocksize, false);
          printByteblock(modbyteblock, blocksize, false);
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
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount, std::string& hashlist, bool log ) {

     // current date/time based on current system
     time_t now = time(0);
   
     // convert now to string form
     char* dt = ctime(&now);

     cout << "Start Time               " << dt;
     cout << "Block Size               " << blocksize << endl;

     cout << "Random Byteblock         ";
     for (int f = 0; f < blocksize; f++) {
          // cout << byteblock[f] << ' ';
          printf("%02X", byteblock[f]);
     }
     // printByteblock(byteblock, blocksize, true);
     cout << endl;

     cout << "Random Byteblock Hex     ";
     for (int f = 0; f < blocksize; f++) {
          //std::cout << byteblock[f] << ' ';
          printf("%02X  ", byteblock[f]);
     }
     cout << endl;


     cout << "Random Byteblock Int     ";
     for (int f = 0; f < blocksize; f++) {
          printf("%-3d ",byteblock[f]);
     }

     cout << endl;
     cout << "Random Byteblock Bigint  ";
     mpz_out_str(stdout, 10, blockint);
     cout << endl;

     cout << "Modulus Size             " << modsize << endl;

     cout << "Modulus Bigint           ";
     gmp_printf("%Zd", modint);
     cout << endl;

     cout << "Modulus Remainder        ";
     gmp_printf("%Zd", remainder);
     cout << endl;

     cout << "Modulus 2   ^ Exponent   " << exponent << endl;
     cout << "Modulus Mod ^ Exponent   " << expmod << endl;

     cout << "Block Signatures         ";
     cout << hashlist;
     cout << endl;

     cout << "Thread Count             " << threadcount << endl;

     cout << "Logging                  " << boolalpha << log << endl;
     cout << endl;
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
