/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdzip Program
 * 
 * mdZipSimple.cpp zips a file with the mdzip format 
 * filename.mdsz
 * 
 * MDzip simple format
 * 
 *   uint64_t input file filesize
 *   uint64_t 64 bit randomizable signature fasthash64 key
 *   block signature format (file block 14 bytes)
 *   8 bytes for the 64-bit fasthash 64 hash - (It could use any number of 64 bit hashes or be configurable)
 *   4 bytes for the 32-bit modulus
 *   1 byte for the modulus exponent
 * 
*/
#include <iostream>
#include <sys/stat.h>
#include "external/CLI11.hpp"
#include <gmp.h>
#include <gmpxx.h>
#include "mdCore/mdCommon.h"
#include "mdCore/mdHashContextList.h"

int mdzipfileNoHeader(std::string filename, long blocksize, int modsize, uint64_t key, std::vector<int> &bhlist);
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, 
                 int modsizeBytes, std::string& blockhashnames, int hclblockblocksize, std::string& blockhashvector, uint64_t blockkey, 
                 int threadcount );
void displayBlockInfo(std::string action, unsigned char *byteblock, long currentblocksize, long blocksize, int blk, int lastblk, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock);                 
void usage();

using namespace std;

int main (int argc, char **argv) {

     std::string filename;
     long filesize;
     long blockcount;
     long blocksize   = 14;
     int modsize      = 32;
     uint64_t key     = 100000;
     int threadnumber = 0;
     int threadcount  = 8;
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode Minimized MDzip C++ Program"};
     app.add_option("-f,--file", filename, "MDzip filename")->check(CLI::ExistingFile)->required();

     // add the block hashlist parameter
     // could also add something to set this for a different 64-bit signature
     // this is currently the fasthash 64 signature number it should probably be a string name
     std::vector<int> blocklist = { FAST64 };

     // set the fasthash64 uint64_t 64 bit long
     app.add_option("-k,--key", key, "Set fasthash key number")->check(CLI::PositiveNumber);

     // randomize the keylist for the hashes
     bool random = false;
     app.add_flag("-r,--rand", random, "Randomize the Key");

     // set logging
     bool runlogging = false;
     app.add_flag("-l,--log", runlogging, "Run Logging");

     // check the argument count and display the usage if it's not specified
     if (argc < 2)
     {
        std::cout << app.help() << std::endl;
        usage();
        return 0;
     }

     // process the command arguments
     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     // if the random key is set randomize the key
     if (random) key = randLong(); 

     // run mdzipfile
     mdzipfileNoHeader(filename, blocksize, modsize, key, blocklist);

}

// mdzip an input file
// current mdzip extension is .mdsz
// this is currently litte endian and 64 bit for the longs
int mdzipfileNoHeader(std::string filename, long blocksize, int modsize, uint64_t key, std::vector<int> &bhlist) {

     long blocknumber = 1;
     double mdversion = 1.01;
     int modexponent  = 0;
     long filesize    = 0;
     filesize = GetFileSize(filename);
     // need to add the mdformat

     // if (filesize < blocksize) blocksize = filesize;
     if (filesize == 0) return 0;

     // calculate the file block count and last block size 
     long blockcount = CalcFileBlocks(filesize, blocksize);
     
     // calculate the last block size
     long blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

     // the curent block size
     long currentblocksize = blocksize;

     std::string mdzipfile = filename + ".mdsz";

     const char *fname = mdzipfile.c_str();
     std::remove(fname);

     // create the ifstream and ofstream objects
     std::ifstream nf(filename, std::ios::in | std::ios::binary);
     std::ofstream mdzip(mdzipfile, std::ios::out | std::ios::binary);
     if(!mdzip) {
        std::cout << "Cannot open mdzip file!" << std::endl;
        return 1;
     }

     // initialize the gmp bigint variables
     int byteorder = 0;
     int endian    = 0;
     size_t count;
     mpz_t byteblockInt, modulusInt, remainder;
     mpz_init_set_str(byteblockInt, "0", 10);
     mpz_init_set_str(modulusInt, "1", 10);
     mpz_init_set_str(remainder, "0", 10);

     // calculate the modulus bigint as 2 ^ modsize 
     // example 2 ^ 32 for a 32 bit modulus
     // example 2 ^ 64 for a 64 bit modulus
     mpz_ui_pow_ui (modulusInt, 2, modsize);
     // subtract 1 from the modulusInt
     // 2^modsize - 1
     // 32-bits example 4,294,967,295 (23^2 − 1)
     mpz_sub_ui(modulusInt, modulusInt, 1);
    
     // need to make sure these are byte order independent
     // the block size header is currently 16 bytes
     mdzip.write(reinterpret_cast<char*>(&filesize),  sizeof(long));
     // read the signature key 
     mdzip.write(reinterpret_cast<char*>(&key),  sizeof(long));

     // initialize the block hash context list
     mdHashContextList hclblock;

     // set the block hash list with the block hash list vector 
     hclblock.setVectorHL(bhlist, HASHBLOCK);

     // set the key
     hclblock.hregister[0].fast64seed = key;

     // set the block hash list vector
     std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
 
     // create the byteblock buffer with the blocksize  
     unsigned char *byteblock  = new unsigned char[blocksize];

     // calculate the correct modulus byte size in case of a odd modulus size 33
     // the modulus parameter is in bits and this converts it to bytes 
     int modsizeBytes = calcModulusBytes(modsize);
     unsigned char *modulusint = new unsigned char[modsizeBytes];

     // set the file block hash list
     std::string blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);

     // display the mdzip file info
     displayInfo(filename, mdversion, filesize, blocksize, blockcount, blockremainder, modsize, modsizeBytes,
     blockhashnames, blocksize, vectorlist, key, 0);

     // last block  
     long lastblk = blockcount;

     while (!nf.eof())
     {
       if (blocknumber <= blockcount) {
           // check if this is the last block and the lastblocksize is not equal to the file block size
           // if it isn't resize the byteblock array and set the currentblocksize to blockremainder
           if ((blocknumber == blockcount) && (blockremainder != blocksize)) {
               currentblocksize = blockremainder;   
               delete byteblock;
               byteblock  = new unsigned char[currentblocksize];   
           } 

           // read the current byteblock from the input file and generate a signature
           nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);

           // set the byte block hash list
           hclblock.setByteBlockHashList((unsigned char*) byteblock, currentblocksize);
           // write the signature list to the mdzip file
           hclblock.writeBlockHashList(mdzip);

           // create a bigint number for the byte block
           mpz_import (byteblockInt, currentblocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

           // calculate the modulus remainder 
           // modulus remainder = byteblockInt mod modulusInt
           mpz_mod (remainder, byteblockInt, modulusInt);
         
           // calculate the modulus exponent with base two 
           int modexponent = calcExponent(byteblockInt);

           // write modexponent
           // if the file block size is less than 32 it's a byte 2 ^ 255 max
           // if block size is greater than 32 bytes it's a int
           if (blocksize > 32) { 
               mdzip.write(reinterpret_cast<char*>(&modexponent),   sizeof(int));
           } else {
               uint8_t modexponent2 = modexponent;
               mdzip.write(reinterpret_cast<char*>(&modexponent2),   sizeof(uint8_t));
           }

           // export the gmp modulus int remainder to a modulus int byte block
           mpz_export(modulusint, &count, byteorder, sizeof(modulusint[0]), endian, 0, remainder);

           // if the export count is less than the byte block size
           // pad the GMP modulusint byte block 
           padBlockBytes(count, modsizeBytes, modulusint);
           
           // write the modulus int remainder byte block
           mdzip.write(reinterpret_cast<char*>(modulusint),   sizeof(char) * modsizeBytes);
           
           // display the byte block info
           // TODO Need to add the logging
           // displayBlockInfo("Zipping", byteblock, currentblocksize, blocksize, blocknumber, lastblk, modexponent, remainder, hclblock, log);
           displayBlockInfo("Zipping", byteblock, currentblocksize, blocksize, blocknumber, lastblk, modexponent, remainder, hclblock); 

           // if this is the last block stop processing 
           if (blocknumber == blockcount) break;

        }   

        blocknumber++;

     }

     delete byteblock;
     delete modulusint;

     nf.close();
     mdzip.close();

     if(!mdzip.good()) {
       cout << "Error occurred at writing time!" << endl;
       // return 1;
     }

     /* free used memory */
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);


     return 0;

}

// display the mdlist mdzip file info
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, int modsizeBytes, std::string& blockhashnames, int hclblockblocksize, 
                 std::string& blockhashvector, uint64_t blockkey, int threadcount ) {


   std::cout << std::left << std::setw(20) << "Zip Filename: " << filename << std::endl;
   std::cout << std::left << std::setw(20) << "Unzip Filename: " << filename << ".out" << std::endl;

   std::cout << std::left << std::setw(20) << "Version: "    << mdversion << std::endl;
   std::cout << std::left << std::setw(20) << "Filesize: "   << filesize  << std::endl;
   std::cout << std::left << std::setw(20) << "Blocksize: "  << blocksize << std::endl;


   std::cout << std::left << std::setw(20) << "Blockcount: "     << blockcount << std::endl;
   std::cout << std::left << std::setw(20) << "Blockremainder: " << blockremainder << std::endl;
   
   std::cout << std::left << std::setw(20) << "Modsize: "        << modsize << std::endl;
   std::cout << std::left << std::setw(20) << "Modsize Bytes: "  << modsizeBytes << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashlist: "  << blockhashnames << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashkeylist: "  << blockkey  << std::endl;
   std::cout << std::left << std::setw(20) << "Block Hash Bytes: " << hclblockblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Platform:" << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;
   std::cout << std::endl;

   // display the file block hash list
   std::cout << "File block hashlist " << std::endl;
   std::cout << blockhashvector << std::endl;
}

// displayBlockInfo
//
// display the signature block info
//
// action, blocknumber, currentblocksize
// signature list
// modulus exponent
// modulus remainder
//
// Example
// Unzipping Block 127 Bytes Size 14/14
// fast64 11350068956456432289 
// Modulus Exponent 65
// Modulus Remainder 4243591440
void displayBlockInfo(std::string action, unsigned char *byteblock, long currentblocksize, long blocksize, int blk, int lastblk, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock) {
   
   int blknum   = blk;
   std::cout << action << " Block " << blknum << " Bytes Size ";

   // display the current block size fraction
   // if this is a 12 byte block
   // 8/12 if the block size is 8 out of a 12 byte block size
   // 12/12 if the blocksize is equal
   std::cout << currentblocksize << "/" << blocksize << std::endl;

   // print the byte block
   printByteblock(byteblock, currentblocksize, true);

   // display out the hash block signatures
   std::cout << hclblock.displayHLhashes() << std::endl;

   // display the modulus exponent
   std::cout << "Modulus Exponent " << modexponent << std::endl;

   // display the modulus remainder
   mpz_class modint(modulusIntRemainder);
   std::cout << "Modulus Remainder " << modint << std::endl << std::endl;
}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand
   mdzipnh --file=test.txt  

   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list
   mdunzipnh --file=filename.mdsz --list --unzip
   mdunzipnh --file=filename.mdsz --valmdzip
)";

    std::cout << usageline << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    // cout << "Hashlist:" << endl << endl;
    // mdHashContextList hcl;
    // hcl.displayHLRegistry(0);

}

