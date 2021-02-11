/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdunzip Program
 * 
 * mdUnzipSimple.cpp unzips a mdzip file or displays the block list
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
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "../testdecode_cpp/external/CLI11.hpp"
#include <gmp.h>
#include "mdCore/mdCommon.h"
#include "mdCore/mdHashContextList.h"
#include "mdCore/mdMutex.h"
#include "mdCore/mdMutexLog.h"
#include "mdCore/modscanFile.h"


using namespace std;

int validateMDzip(std::string filename);
int mdlist(std::string filename, bool listfile, bool runlogging);
int mdunzipfile(std::string filename, int threadcount, bool overwrite, bool runlogging);
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, 
                 int modsizeBytes, std::string& blockhashnames, int hclblockblocksize, std::string& blockhashvector, uint64_t blockkey, 
                 bool mdlist, int threadcount );
void displayBlockInfo(std::string action, int blocksize, int blk, int lastblk, long blockremainder, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock, mdMutexLog &log);                 
void usage();

int main (int argc, char **argv) {

     std::string filename;
     long filesize;
     long blockcount;
     long blocksize   = 14;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = std::thread::hardware_concurrency();
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode Minimized MDunzip C++ Program"};
     app.add_option("-f,--file",    filename,    "MDunzip filename")->check(CLI::ExistingFile)->required();
     app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // display the block hash list 
     bool list = false;
     app.add_flag("-l,--list", list, "List the mdzip file");

     // Add an mdzip option boolean to just allow mdlist to be specified
     // if runmdzip is false you don't run it
     // you can run mdlist or mdunzip or both mdlist and mdunzip
     bool runmdzip = false;
     app.add_flag("-u,--unzip", runmdzip, "MDunzip a file");

     // overwrite the unzipped output file if it exists
     // stop and throw an error if it doesn't
     bool overwrite = false;
     app.add_flag("-o,--over", overwrite, "Overwrite an existing mdunzip output file");

     // set logging
     bool runlogging = false;
     // app.add_option("-l,--log", runlogging, "Run Logging");
     app.add_flag("--log", runlogging, "Run Logging");

     // validate the mdzip file 
     bool validatemdzip = false;
     app.add_flag("--valmdzip", validatemdzip, "Validate the mdzip file");
   
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

     // block signature vector
     std::vector<int> blocklist = { 5 };

     // validate the mdzip file
     if (validatemdzip) {
         validateMDzip(filename);
         return 0;
     }

     // execute the mdlist display mdzip file blocks if list is true
     // ./mdunzip --file=test.mdz --list=true
     try {
          if (list) mdlist(filename, list, runlogging);
     } catch (exception& ex) {
         std::cout << "MDList Exception " << std::endl;
     } 

 
     // run the mdunzipfile
     // should add a block only format with a specified signature and modsize and no header
     // like highway hash and mod 32 and block 14 and 1 or no signature key
     // ./mdunzipnh --file=phone.txt.mdsz --threads=32
     // currently creates phone.txt.mdsz.out
     try {
         if (!list) mdunzipfile(filename, threadcount, overwrite, runlogging);
         if (list && runmdzip) mdunzipfile(filename, threadcount, overwrite, runlogging);
     } catch (exception& ex) {
         std::cout << "MDunzip Exception " << std::endl;
     }     

     return 0;
}

// validate the mdzip file format
int validateMDzip(std::string filename) {
   // check the inputfilesize variable
   size_t inputfilesize = 0;
   // mdzip mdlist variables
   double mdversion     = 1.0;
   long blocksize       = 14;
   long blockcount      = 0;
   long blockremainder  = 0;
   long filesize        = 0;
   int modexponent      = 0;
   int modsize          = 32;
   int hclfilesize;
   int hclblocksize;
   uint64_t blockkey    = 0;
   std::string filehashnames;   
   std::string blockhashnames;

   // if the listfile boolean is false don't run the list mdzip file
   // if (!listfile) return 0;

   // Check the file extension
   if(fileExtension(filename) != "mdsz") {
      std::cout << "Invalid MDzip File!" << std::endl;
      return 1;
   }

   // check if the input file is below the minimum
   // the header is about 36 bytes minimum
   inputfilesize = getFilesize(filename);
   if (inputfilesize < 21) {
      std::cout << "Filename size below mdzip minimum!" << std::endl;
      return 1;
   }

   // open the mdzip file 
   std::ifstream nf(filename, std::ios::in | std::ios::binary);
   if(!nf) {
      std::cout << "Cannot open file!" << std::endl;
      return 1;
   }

   // begin reading in the mdzip file data
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blockkey),  sizeof(long));

   // initialize the modulusbytes array to store the modulo remainder
   int modsizeBytes = calcModulusBytes(modsize);

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

   // mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list vector tuple for file and hash blocks
   std::vector<int> blocklist = { 5 };
   hclblock.setVectorHL(blocklist, HASHBLOCK);

   int modexp = 1;
   int hashblocksize = hclblock.calcBlockSize(HASHBLOCK) + modsizeBytes + 1;
   int totalblocksize = hashblocksize * blockcount;

   long sumfilesize = 16 + totalblocksize;

   if (sumfilesize == inputfilesize) {
      std::cout << "MDzip File " << filename << " validates " << std::endl; 
      std::cout << "MDzip File " << sumfilesize << " = " << inputfilesize << std::endl; 
   }

   nf.close();

   return 0;   

}

// mdlist
// display the mdzip block contents of a file
int mdlist(std::string filename, bool listfile, bool runlogging) {

   // check the inputfilesize variable
   size_t inputfilesize = 0;
   // mdzip mdlist variables
   double mdversion     = 1.0;
   long blocksize       = 14;
   long blockcount      = 0;
   long blockremainder  = 0;
   long filesize        = 0;
   int modexponent      = 0;
   int modsize          = 32;
   int hclfilesize;
   int hclblocksize;
   uint64_t blockkey    = 0;
   std::string filehashnames;   
   std::string blockhashnames;

   // if the listfile boolean is false don't run the list mdzip file
   if (!listfile) return 0;

   // Check the file extension
   if(fileExtension(filename) != "mdsz") {
      std::cout << "Invalid MDzip File!" << std::endl;
      return 1;
   }

   // check if the input file is below the minimum
   // the header is about 36 bytes minimum
   inputfilesize = getFilesize(filename);
   if (inputfilesize < 21) {
      std::cout << "Filename size below mdzip minimum!" << std::endl;
      return 1;
   }

   // open the mdzip file 
   std::ifstream nf(filename, std::ios::in | std::ios::binary);
   if(!nf) {
      std::cout << "Cannot open file!" << std::endl;
      return 1;
   }

   // initialize the log object
   mdMutexLog log(runlogging);

   // begin reading in the mdzip file data
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blockkey),  sizeof(long));

   // initialize the modulusbytes array to store the modulo remainder
   int modsizeBytes = calcModulusBytes(modsize);
   unsigned char *modulusbytes = new unsigned char[modsizeBytes];
   mpz_t modulusInt;
   mpz_init_set_str(modulusInt, "1", 10);

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   // blockremainder  = filesize % blocksize;
   blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

   // mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list vector tuple for file and hash blocks
   std::vector<int> blocklist = { 5 };
   hclblock.setVectorHL(blocklist, HASHBLOCK);

   // set the block hash key
   hclblock.hregister[0].fast64seed = blockkey;

   // calculate the file and file block hash list size
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   // set the file block hash list
   std::string blockhashvector = hclblock.getHLvectorsString(HASHBLOCK);

   blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);

   // display the mdzip file info
   displayInfo(filename, mdversion, filesize, blocksize, blockcount, blockremainder, modsize, modsizeBytes,
   blockhashnames, hclblockblocksize, blockhashvector, blockkey, true, 0);

   // display the file block hash block list
   // block signatures / modulus exponent / modulus remainder
   int blk = 0;
   int lastblk = blockcount - 1;

   // initialize the gmp bigint import variables
   int byteorder = 0;
   int endian    = 0;

   // read each of the mdzip file signature blocks
   for (blk = 0; blk < blockcount; blk++) {

        // read the file block hash list 
        hclblock.readBlockHashList(nf);

        // read the modulus exponent
        if (blocksize > 32) {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(int));
        } else {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(uint8_t));
        }

        // read the modulus remainder
        nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
        mpz_import (modulusInt, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);

        // display the byte block info
        displayBlockInfo("Displaying", blocksize, blk, lastblk, blockremainder, modexponent, modulusInt, hclblock, log);
   }

   delete modulusbytes;
   mpz_clear(modulusInt);
   nf.close();

   return 0;

}

// mdunzipfile
// mdunzip a valid mdzip file 
// ie a file with the *.mdsz extension
// ie file.mdsz file
// the output unzipped file is currently file.mdz.out or extension .out
int mdunzipfile(std::string filename, int threadcount, bool overwrite, bool runlogging) {

   // std::cout << "mdunzipping file " << filename << std::endl; // to output file

   size_t inputfilesize = 0;
   std:string mdunzipfile = filename + ".out";
   // mdunzip data variables
   double mdversion     = 1.0;
   long blocksize       = 14;
   long blockcount      = 0;
   long blockremainder  = 0;
   long filesize        = 0;
   int modexponent      = 0;
   int modsize          = 32;
   int hclfilesize;
   int hclblocksize;
   long blockkey        = 0;
   std::string filehashnames;   
   std::string blockhashnames;

   // Check the file extension
   // need to modify this logic to allow more than one
   if(fileExtension(filename) != "mdsz") {
      std::cout << "Invalid MDzip File!" << std::endl;
      return 1;
   }

   // check if the input file is below the minimum
   // the header is about 36 bytes minimum
   inputfilesize = getFilesize(filename);
   if (inputfilesize < 24) {
      std::cout << "Filename size below mdzip minimum!" << std::endl;
      return 1;
   }

   // if the overwrite is specified remove the old mdunzip output file if it exists 
   const char *fname = mdunzipfile.c_str();
   if ((overwrite)) {
      if (CheckIfFileExists(mdunzipfile)) std::remove(fname);
   } else {
      if (CheckIfFileExists(mdunzipfile)) {
         std::cout << "Mdunzip " << mdunzipfile << " file exists " << std::endl;
         std::cout << "Please specify the overwrite file option " << std::endl;
         std::cout << "mdunzipnh --file=" << filename << " --over=true" << std::endl;
         return 1;
      }
   } 

   // open the mdzip file 
   std::ifstream nf(filename, std::ios::in | std::ios::binary);
   std::ofstream wf(mdunzipfile, std::ios::out | std::ios::binary);
   if(!nf) {
      std::cout << "Cannot open mdzip file!" << std::endl;
      return 1;
   }

   // open the mdunzip output file
   if(!wf) {
      std::cout << "Cannot open mdzip output file!" << std::endl;
      return 1;
   }

   // begin reading in the mdzip file data
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blockkey),  sizeof(long));

   // initialize the modulusbytes array to store the modulo remainder
   int modsizeBytes = calcModulusBytes(modsize);
   unsigned char *modulusbytes = new unsigned char[modsizeBytes];
   mpz_t modulusInt, modulusIntRemainder;
   mpz_init_set_str(modulusInt, "1", 10);
   mpz_init_set_str(modulusIntRemainder, "1", 10);

   // calculate the modulus 2 ^ modsize 
   mpz_ui_pow_ui (modulusInt, 2, modsize);

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   // blockremainder  = filesize % blocksize;
   blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

   // the curent block size
   // long currentblocksize = blocksize;

   //mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list for the hash blocks
   std::vector<int> blocklist = { 5 };
   hclblock.setVectorHL(blocklist, HASHBLOCK);

   // set the key
   hclblock.hregister[0].fast64seed = blockkey;

   // calculate the file and file block hash list size
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);

   // set the file block hash list
   std::string blockhashvector = hclblock.getHLvectorsString(HASHBLOCK);

   // display the mdzip file info
   displayInfo(filename, mdversion, filesize, blocksize, blockcount, blockremainder, modsize, modsizeBytes, 
               blockhashnames, hclblockblocksize, blockhashvector, blockkey, false, threadcount);

   // display the file block hash block list
   // block signatures / modulus exponent / modulus remainder
   int blk = 0;
   int lastblk = blockcount - 1;

   // initialize the gmp bigint import variables
   int byteorder = 0;
   int endian    = 0;

   // initialize the mutex object
   // I should set a result variable and pass it into the mutex
   // it result = 0 then the mutex can set it and stop the execution for the mod scan
   mdMutex    mutex(threadcount);
   mdMutexLog log(runlogging);

   // initialize the modulus scan array
   // this allows it to run mulithreaded 
   modscan* mst = new modscan[threadcount];

   // initialize the modulus scan thread objects with the block hash list string
   for(int tnum = 0; tnum < threadcount; tnum++) {
         mst[tnum].hcl.setVectorHLstring(blockhashnames, HASHBLOCK);
   }
 
   // read each of the mdzip file signature blocks
   for (blk = 0; blk < blockcount; blk++) {

         // read the file block hash list 
         hclblock.readBlockHashList(nf);

         // read the modulus exponent
         if (blocksize > 32) {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(int));
         } else {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(uint8_t));
         }

         // read the modulus remainder
         nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
         mpz_import (modulusIntRemainder, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);

         // display the byte block info
         displayBlockInfo("Unzipping", blocksize, blk, lastblk, blockremainder, modexponent, modulusIntRemainder, hclblock, log);

         // check if this is the last byte block
         // if it is set the blocksize to the last block size
         if ((blk == lastblk) && (blockremainder != blocksize)) {
           if (blocksize == 0) break;
           blocksize = blockremainder;           
         } 

         // set the thread modulus scan objects
         for(int tnum = 0; tnum < threadcount; tnum++) {
            // if this is the first block run the full initialization
            if (blk < 1) { 
               mst[tnum].setModscan(&log, byteorder, endian, modulusIntRemainder, modulusInt, modexponent, 1, blocksize, tnum, threadcount, &mutex);
            } else {
               // modulusInt remainder is the mod remainder
               // modulusInt is the 2 ^ mod exponent
               mst[tnum].resetThread(modulusIntRemainder, modulusInt, modexponent, blocksize, tnum);
            }  
            // set the hash context list and the signatures based on the current byte block
            // copy the register struct to the thread register
            mst[tnum].hcl.hregister[0] = hclblock.hregister[0];
         }

         // initialize the modulus scan threads vector
         std::vector<std::thread> threads;
         for(int tnum = 0; tnum < threadcount; tnum++){
               threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
         }

         // start the count
         // auto start = std::chrono::high_resolution_clock::now();

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

            // find the last thread match
            int threadMatchNumber = mutex.getMatchThread(); 

            // check the modulus scan results
            std::ostringstream result; 
            unsigned char *modbyteblock;
            modbyteblock = mst[threadMatchNumber].getModscanByteBlock();
            // result << endl << "Found Match" << endl << endl;
            // result << "Elapsed Time (s) " << std::to_string(elapsed_time/1e9) << endl;
            result << "Modulus Scan thread " << threadMatchNumber << " Match " << std::endl;
            log.writeLog(result.str());

            log.logMatchByteblock(modbyteblock, blocksize, false);
            log.logMatchByteblock(modbyteblock, blocksize, true);

            // add a newline before the next block
            std::cout << std::endl << std::endl;

            // Write the modulus scan byteblock to a file
            wf.write(reinterpret_cast<char*>(modbyteblock),   blocksize);
         }

         // stop the threads
         for(int tnum = 0; tnum < threadcount; tnum++) {
            mst[tnum].stopThread();
            while (!mst[tnum].isStoppedThread()) { }
         }
         
         mutex.resetMatched();
         threads.clear();

   }

   mpz_clear(modulusInt);
   mpz_clear(modulusIntRemainder);
   
   nf.close();
   wf.close();

   return 0;

}

// display the mdlist mdzip file info
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, int modsizeBytes, std::string& blockhashnames, int hclblockblocksize, 
                 std::string& blockhashvector, uint64_t blockkey, bool mdlist, int threadcount ) {


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
   if (mdlist == false) std::cout << std::left << std::setw(20) << "Threadcount:" << threadcount << std::endl;
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
void displayBlockInfo(std::string action, int blocksize, int blk, int lastblk, long blockremainder, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock, mdMutexLog &log) {
   
   int blknum   = blk + 1;
   int cblksize = blocksize;
   std::cout << action << " Block " << blknum << " Bytes Size ";

   // if this is the last block
   // set the blocksize to the last block size
   if ((blk == lastblk) && (blockremainder != blocksize)) {
           cblksize = blockremainder;      
   } 

   // display the current block size fraction
   // if this is a 12 byte block
   // 8/12 if the block size is 8 out of a 12 byte block size
   // 12/12 if the blocksize is equal
   std::cout << cblksize << "/" << blocksize << std::endl;

   // display out the hash block signatures
   std::cout << hclblock.displayHLhashes() << std::endl;

   // display the modulus exponent
   std::cout << "Modulus Exponent " << modexponent << std::endl;

   // if log == true log it otherwise just display it
   gmp_printf("Modulus Remainder %Zd\n\n", modulusIntRemainder);
   // result << "Modulus Bigint           ";
   // gmp_printf("%Zd", modint);
   // data = mpz_get_str(NULL, 10, modint);
   // result << data << endl;
   // free(data);
}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list=true
   mdunzipnh --file=filename.mdsz --list=true --unzip=false

   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand=true
   mdzipnh --file=test.txt  

)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    //cout << "Hashlist:" << endl << endl;
    // mdHashContextList hcl;
    // hcl.displayHLRegistry(0);

}

