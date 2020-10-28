#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <gmp.h>
#include <time.h>
#include <string>
#include <openssl/sha.h>
#include "CLI11.hpp" 
#include "mdMutex.h"
#include "modscan.h"
#include "string.h"
#include "stdio.h"

using namespace std;

uint8_t sha1[40];

int genSHA1(unsigned char *byteblock, int blocksize);
unsigned char *genRandomByteBlock(size_t num_bytes);
unsigned char *convertHexToByteBlock(const std::string & source);
unsigned char *setByteBlock(size_t num_bytes);
int calcExponent (mpz_t blockint);
int calcExponentModulus (mpz_t modulus, mpz_t blockint);
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount );
void usage();

/* 
   This is a C++ GMP modulus scan multithreaded test program 
   MDencode GMP requires the GMP Library to build https://gmplib.org/
   This program uses one signature SHA1 to bootstrap the testing.
   In the future it will use a C++ Hash Context list with more than one signature
*/
int main (int argc, char **argv) {

     size_t blocksize = 12;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = 1;

     if (argc < 2)  
     { 
         usage();
         return 0; 
     } 

     CLI::App app{"MDEncode GMP C++ Test Program"};
     app.add_option("-b,--block", blocksize, "Blocksize number")->check(CLI::Number);
     app.add_option("-m,--mod", modsize, "Modulus size number")->check(CLI::Number);
     app.add_option("-t,--threads", threadcount, "Thread count number")->check(CLI::Number);
     //app.add_option("-v,--version", version, "Version number");

     // I think the modulus scan is not handling the 0022FF or padding the zero correctly for zero byte blocks
     // need to check the export
     // std::string hexstring = "2200FF";
     std::string hexstring; 
     // I think CLI11 uses -h for help so I can't use -h
     app.add_option("-x,--hex", hexstring, "Hex Byteblock string");

     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     unsigned char *byteblock;
     // generate a random n byte byteblock if the hexstring is empty
     if (hexstring.empty()) {
     // generate a random n byte byteblock
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
     // order very important
     // within each word endian can be 1 for most significant byte first, -1 for least significant first, or 0 for the native endianness of the host CPU
     // order has to match the modulus scan mpz_export
     mpz_import (byteblockInt, blocksize, 0, sizeof(byteblock[0]), 0, 0, byteblock);

     // calculate the modulus 2 ^ modsize 
     mpz_ui_pow_ui (modulusInt, 2, modsize);

     // calculate the modulus remainder 
     mpz_mod (remainder, byteblockInt, modulusInt); 

     // calculate the modulus exponent with two
     int exp = calcExponent(byteblockInt);

     // calculate the modulus exponent with the modulus
     int expmod = calcExponentModulus(modulusInt, byteblockInt);

     // compute the SHA1 hash of the byteblock
     // this currently uses one signature and not a hash context list
     // others will be added in the future
     genSHA1(byteblock, blocksize);

     // display the current block stats
     displayFloor(byteblock, remainder, modulusInt, byteblockInt, modsize, exp, expmod, blocksize, threadcount );

     // initialize the mutex object
     // I should set a result variable and pass it into the mutex
     // it result = 0 then the mutex can set it and stop the execution for the mod scan
     mdMutex mutex;

     // initialize the modulus scan array
     // this currently only runs with one thread
     modscan* mst = new modscan[threadcount];
     for(int tnum = 0; tnum < threadcount; tnum++) {
         // ms.setModscan(remainder, modulusInt, exp, expmod, blocksize, threadnumber, threadcount, sha1);
         mst[tnum].setModscan(remainder, modulusInt, exp, expmod, blocksize, tnum, threadcount, &mutex, sha1);
     } 

     // initialize the modulus scan threads vector
     std::vector<std::thread> threads;
     for(int tnum = 0; tnum < threadcount; tnum++){
        threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
     }

     // execute the threads
     for(int tnum = 0; tnum < threads.size(); tnum++)
     {
        // threads.at(i).join();
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
          for (int i = 0; i < blocksize; i++) {
               printf("%d ", byteblock[i]);
          }
          printf("\n");

          for (int j = 0; j < blocksize; j++) {
               printf("%d ", modbyteblock[j]);
          }
          printf("\n");
     } else {
          cout << "Modulus Scan and Random byteblock don't match" << endl;
     }

     /* free used memory */
     free (byteblock);
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);
     // mpz_clears(remainder, modulusInt, byteblockInt, NULL);



    return 0;
}

// calculate test signature
// use the openssl SHA1 signature generator on the byteblock
int genSHA1(unsigned char *byteblock, int blocksize) {

       SHA1(byteblock, blocksize, sha1);

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
// I don't think the modscan is currently handling 00FF33 or 0000FFFF7873 hex strings
// need to check the GMP Export
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


// displays the modulus scan information
void displayFloor(unsigned char *byteblock, mpz_t remainder, mpz_t modint, mpz_t blockint, int modsize, int exponent, int expmod, int blocksize, int threadcount ) {

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

     cout << "Block SHA1               ";
     for (int n = 0; n < 20; n++)
     printf("%02X", sha1[n]);
     cout << endl;

     cout << "Thread Count             " << threadcount << endl;

}

// display the usage
void usage() {
std::string usageline = R"(MDencode GMP C++ Threaded Modulus Scan Test
MDencode GMP requires the GMP Library to build https://gmplib.org/
MDencode GMP also requires the OpenSSL Library

Usage: ./decoderRandomTestHCthreads_gmp [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -b,--block UINT             Blocksize number
  -m,--mod INT                Modulus size number
  -t,--threads INT            Thread count number
  -x,--hex TEXT               Hex Byteblock string

Examples:
   ./decoderRandomTestHCthreads_gmp -b 12 -m 64 -t 16
   ./decoderRandomTestHCthreads_gmp --block=12 --mod=128 --threads=16
)";

     cout << usageline << endl; 
}
