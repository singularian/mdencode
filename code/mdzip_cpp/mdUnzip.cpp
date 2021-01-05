/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdunzip Program
 * 
 * mdUnzip.cpp unzips a mdzip file or displays the block list
 *
 * 
 * 
*/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "../testdecode_cpp/external/CLI11.hpp"
#include <gmp.h>
#include "common.h"
#include "mdHashContextList.h"
#include "mdMutex.h"
#include "mdMutexLog.h"
#include "modscanFile.h"


using namespace std;

int mdlist(std::string filename, bool listfile, bool runlogging);
int mdunzipfile(std::string filename, int threadcount, bool runlogging);
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, 
                 int modsizeBytes, std::string& filehashnames, std::string& blockhashnames, int hclfileblocksize, int hclblockblocksize, 
                 std::string& filehashvector, std::string& blockhashvector, bool mdlist, int threadcount );
void usage();

int main (int argc, char **argv) {

     std::string filename;
     long filesize;
     long blockcount;
     long blocksize   = 12;
     int modsize      = 64;
     int threadnumber = 0;
     int threadcount  = 8;
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode MDunzip C++ Program"};
     app.add_option("-f,--file",    filename,    "MDunzip filename")->check(CLI::ExistingFile)->required();
     // app.add_option("-b,--block",   blocksize,   "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
     // app.add_option("-m,--mod",     modsize,     "Modulus size number")->check(CLI::PositiveNumber);
     app.add_option("-t,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // display the block hash list 
     bool list = false;
     app.add_option("-l,--list", list, "List the mdzip file");

     // Add an mdzip option boolean to just allow mdlist to be specified
     // if runmdzip is false you don't run it
     // you can run mdlist or mdunzip or both mdlist and mdunzip
     bool runmdzip = true;
     app.add_option("-u,--unzip", runmdzip, "mdunzip a file");

     // overwrite the unzipped output file if it exists
     // stop and throw an error if it doesn't
     bool overwrite = true;
     app.add_option("-o,--over", runmdzip, "Overwriting an existing mdunzip output file");

     // set logging
     bool runlogging = false;
     // app.add_option("-l,--log", runlogging, "Run Logging");
     app.add_option("--log", runlogging, "Run Logging");

     
     if (argc < 2)
     {
        std::cout << app.help() << std::endl;
        usage();
        return 0;
     }


     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     // execute the mdlist display mdzip file blocks if list is true
     // ./mdunzip --file=test.mdz --list=true
     mdlist(filename, list, runlogging);
 
     // run the mdunzipfile
     // should add a block only format with a specified signature and modsize and no header
     // like highway hash and mod 32 and block 14 and 1 or no signature key
     // ./mdunzip --file=phone.txt.mdz --threads=32
     // currently creates phone.txt.mdz.out
     if (runmdzip) mdunzipfile(filename, threadcount, runlogging);

     return 0;
}

// mdlist
// display the mdzip block contents of a file
int mdlist(std::string filename, bool listfile, bool runlogging) {

   // check the inputfilesize variable
   size_t inputfilesize = 0;
   // mdzip mdlist variables
   double mdversion     = 1.0;
   long blocksize       = 0;
   long blockcount      = 0;
   long blockremainder  = 0;
   long filesize        = 0;
   int modexponent      = 0;
   int modsize          = 0;
   int hclfilesize;
   int hclblocksize;
   std::string filehashnames;   
   std::string blockhashnames;

   // if the listfile boolean is false don't run the list mdzip file
   if (!listfile) return 0;

   // check if the input file is below the minimum
   // the header is about 36 bytes minimum
   inputfilesize = getFilesize(filename);
   if (inputfilesize < 48) {
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
   nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
   nf.read(reinterpret_cast<char*>(&modsize),   sizeof(int));

   // initialize the modulusbytes array to store the modulo remainder
   int modsizeBytes = calcModulusBytes(modsize);
   unsigned char *modulusbytes = new unsigned char[modsizeBytes];
   mpz_t modulusInt;
   mpz_init_set_str(modulusInt, "1", 10);

   // read the file hash list string from the mdzip file
   nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int));
   char* buf = new char[hclfilesize];
   // nf.read(reinterpret_cast<char*>(&filehashnames),  hclfilesize);
   if (hclfilesize > 0) {
     nf.read(buf,  hclfilesize);
     filehashnames.append(buf, hclfilesize);
     delete buf;
   }

   // read the file block hash list string from the mdzip file
   nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int));
   char* buf2 = new char[hclblocksize];
   nf.read(buf2,  hclblocksize);
   blockhashnames.append(buf2, hclblocksize);
   delete buf2;
   // nf.read(reinterpret_cast<char*>(&blockhashnames), hclblocksize);

   // TODO load the keylist 
   // create another int size for the keylist and load the keylist into a char array
   // then set each hashlist object with the keylist
   // probably should add a key byte size to the registry 
   // change the hash block list to a struct then have the first hashlist object load it and use it's registry to update the others
   // the thread_local doesn't work

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   blockremainder  = filesize % blocksize;

   mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list vector tuple for file and hash blocks
   hclfile.setVectorHLstring(filehashnames, HASHBLOCK);
   hclblock.setVectorHLstring(blockhashnames, HASHBLOCK);

   // calculate the file and file block hash list size
   int hclfileblocksize  = hclfile.calcBlockSize(HASHBLOCK);
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   // set the file hash list parameters and hash block size
   std::string filehashvector = hclfile.getHLvectorsString(HASHBLOCK);

   // TODO set the hashblockgroup 
   //  std::string bghashvector = getHLvectorsString(HASHBLOCKGROUP);

   // set the file block hash list
   std::string blockhashvector = hclblock.getHLvectorsString(HASHBLOCK);

   // display the mdzip file info
   displayInfo(filename, mdversion, filesize, blocksize, blockcount, blockremainder, modsize, modsizeBytes, filehashnames, 
   blockhashnames, hclfileblocksize, hclblockblocksize, filehashvector,  blockhashvector, true, 0);

   // display the file block hash block list
   // block signatures / modulus exponent / modulus remainder
   int blk = 0;
   int lastblk = blockcount - 1;

   // initialize the gmp bigint import variables
   int byteorder = 0;
   int endian    = 0;

   // read each of the mdzip file signature blocks
   for (blk = 0; blk < blockcount; blk++) {
        std::cout << "Reading Block " << (blk + 1);
        if ((blk == lastblk) && (blockremainder != blocksize)) {
           std::cout << " bytes size " << blockremainder << "/" << blocksize << std::endl;
        } else {
           std::cout << " bytes size " << blocksize << "/" << blocksize << std::endl;
        }

        // read the file block hash list 
        hclblock.readBlockHashList(nf);
        std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
        std::cout << hclblock.displayHLhashes() << std::endl;

        // read the modulus exponent
        if (blocksize > 32) {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(int));
        } else {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(uint8_t));
        }
        std::cout << "Modulus Exponent " << modexponent << std::endl;

        // read the modulus remainder
        nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
        mpz_import (modulusInt, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);
        // printByteblock(modulusInt, modsizeBytes, true);
        gmp_printf("Modulus Remainder %Zd\n\n", modulusInt);
   }

   delete modulusbytes;
   mpz_clear(modulusInt);
   nf.close();

   return 0;

}

// mdunzipfile
// mdunzip a valid mdzip file 
// ie a file with the *.mdz extension
// ie file.mdz file
// the output unzipped file is currently file.mdz.out or extension .out
int mdunzipfile(std::string filename, int threadcount, bool runlogging) {

   std::cout << "mdunzipping file " << filename << std::endl;

   size_t inputfilesize = 0;
   std:string mdunzipfile = filename + ".out";
   // mdunzip data variables
   double mdversion     = 1.0;
   long blocksize       = 0;
   long blockcount      = 0;
   long blockremainder  = 0;
   long filesize        = 0;
   int modexponent      = 0;
   int modsize          = 0;
   int hclfilesize;
   int hclblocksize;
   std::string filehashnames;   
   std::string blockhashnames;

   // check if the input file is below the minimum
   // the header is about 36 bytes minimum
   inputfilesize = getFilesize(filename);
   if (inputfilesize < 48) {
      std::cout << "Filename size below mdzip minimum!" << std::endl;
      return 1;
   }

   // remove the old mdunzip output file if it exists
   // TODO need to add the overwrite
   const char *fname = mdunzipfile.c_str();
   std::remove(fname);

   // open the mdzip file 
   std::ifstream nf(filename, std::ios::in | std::ios::binary);
   std::ofstream wf(mdunzipfile, std::ios::out | std::ios::binary);
   if(!nf) {
      std::cout << "Cannot open mdzip file!" << std::endl;
      return 1;
   }

   if(!wf) {
      std::cout << "Cannot open mdzip output file!" << std::endl;
      return 1;
   }


   // begin reading in the mdzip file data
   nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
   nf.read(reinterpret_cast<char*>(&modsize),   sizeof(int));

   // initialize the modulusbytes array to store the modulo remainder
   int modsizeBytes = calcModulusBytes(modsize);
   unsigned char *modulusbytes = new unsigned char[modsizeBytes];
   mpz_t modulusInt, modulusIntRemainder;
   mpz_init_set_str(modulusInt, "1", 10);
   mpz_init_set_str(modulusIntRemainder, "1", 10);

   // calculate the modulus 2 ^ modsize 
   mpz_ui_pow_ui (modulusInt, 2, modsize);


   // read the file hash list string from the mdzip file
   nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int));
   char* buf = new char[hclfilesize];
   // nf.read(reinterpret_cast<char*>(&filehashnames),  hclfilesize);
   if (hclfilesize > 0) {
     nf.read(buf,  hclfilesize);
     filehashnames.append(buf, hclfilesize);
     delete buf;
   }

   // read the file block hash list string from the mdzip file
   nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int));
   char* buf2 = new char[hclblocksize];
   nf.read(buf2,  hclblocksize);
   blockhashnames.append(buf2, hclblocksize);
   delete buf2;
   // nf.read(reinterpret_cast<char*>(&blockhashnames), hclblocksize);

   // TODO load the keylist 
   // create another int size for the keylist and load the keylist into a char array
   // then set each hashlist object with the keylist
   // probably should add a key byte size to the registry 
   // change the hash block list to a struct then have the first hashlist object load it and use it's registry to update the others
   // the thread_local doesn't work

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   blockremainder  = filesize % blocksize;

   mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list vector tuple for file and hash blocks
   hclfile.setVectorHLstring(filehashnames, HASHBLOCK);
   hclblock.setVectorHLstring(blockhashnames, HASHBLOCK);

   // calculate the file and file block hash list size
   int hclfileblocksize  = hclfile.calcBlockSize(HASHBLOCK);
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   // set the file hash list parameters and hash block size
   std::string filehashvector = hclfile.getHLvectorsString(HASHBLOCK);

   // TODO set the hashblockgroup 
   //  std::string bghashvector = getHLvectorsString(HASHBLOCKGROUP);

   // set the file block hash list
   std::string blockhashvector = hclblock.getHLvectorsString(HASHBLOCK);

   // display the mdzip file info
   displayInfo(filename, mdversion, filesize, blocksize, blockcount, blockremainder, modsize, modsizeBytes, filehashnames, 
               blockhashnames, hclfileblocksize, hclblockblocksize, filehashvector,  blockhashvector, false, threadcount);

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
   mdMutex mutex;
   mdMutexLog log(runlogging);

   // initialize the modulus scan array
   // this allows it to run mulithreaded 
   modscan* mst = new modscan[threadcount];

   for(int tnum = 0; tnum < threadcount; tnum++) {
         // mst[tnum].setModscan(&log, byteorder, endian, modulusIntRemainder, modulusInt, modexponent, 1, blocksize, tnum, threadcount, &mutex);
         // set the hash context list and the signatures based on the current byte block
         // mst[tnum].hcl.setVectorHL(def, HASHBLOCK);
         mst[tnum].hcl.setVectorHLstring(blockhashnames, HASHBLOCK);
         // mst[tnum].hcl.setByteBlockHashList(byteblock, blocksize);
   }
 
   // read each of the mdzip file signature blocks
   for (blk = 0; blk < blockcount; blk++) {

         std::cout << "Unzipping Block " << (blk + 1);
         if ((blk == lastblk) && (blockremainder != blocksize)) {
           std::cout << " bytes size " << blockremainder << "/" << blocksize << std::endl;
           blocksize = blockremainder;
         } else {
           std::cout << " bytes size " << blocksize << "/" << blocksize << std::endl;
         }

         // read the file block hash list 
         hclblock.readBlockHashList(nf);
         std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
         std::cout << hclblock.displayHLhashes() << std::endl;

         // read the modulus exponent
         if (blocksize > 32) {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(int));
         } else {
           nf.read(reinterpret_cast<char*>(&modexponent),   sizeof(uint8_t));
         }
         std::cout << "Modulus Exponent " << modexponent << std::endl;

         // read the modulus remainder
         nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
         mpz_import (modulusIntRemainder, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);
         // printByteblock(modulusInt, modsizeBytes, true);
         gmp_printf("Modulus Remainder %Zd\n\n", modulusIntRemainder);
         
         for(int tnum = 0; tnum < threadcount; tnum++) {
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

         // need to change this to three states
         // finding   = 0 // searching for the value with the modscan
         // not found = 1 // modscan mutext match result
         // found     = 2 // modscan mutext match result
         // Maybe while (result == 0) {
         // std::cout << "mutex is matched " <<  mutex.getIsMatched() << std::endl;  
         while (mutex.getIsMatched() == false) {

         }

         int threadMatchNumber = mutex.getMatchThread(); 


         // check the modulus scan results
         std::ostringstream result; 
         unsigned char *modbyteblock;
         modbyteblock = mst[threadMatchNumber].getModscanByteBlock();
         // result << endl << "Found Match" << endl << endl;
         // result << "Elapsed Time (s) " << std::to_string(elapsed_time/1e9) << endl;
         result << "Modulus Scan thread " << threadMatchNumber << " Match "; // << endl;
         log.writeLog(result.str());
         std::cout <<  std::endl;

         log.logMatchByteblock(modbyteblock, blocksize, false);
         log.logMatchByteblock(modbyteblock, blocksize, true);

         std::cout <<  std::endl; // TODO need to add this to the log

         // Need to write the modbyteblock to a file
         wf.write(reinterpret_cast<char*>(modbyteblock),   blocksize);

         // stop the threads
         for(int tnum = 0; tnum < threadcount; tnum++) {
            mst[tnum].stopThread();
            while (!mst[tnum].isStoppedThread()) { }
         }
         
         mutex.resetMatched();
         threads.clear();

   }

   delete modulusbytes;
   mpz_clear(modulusInt);
   mpz_clear(modulusIntRemainder);
   
   nf.close();
   wf.close();

   return 0;

}

// display the mdlist mdzip file info
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, int modsizeBytes, std::string& filehashnames, std::string& blockhashnames, int hclfileblocksize, int hclblockblocksize, 
                 std::string& filehashvector,  std::string& blockhashvector, bool mdlist, int threadcount ) {


   std::cout << std::left << std::setw(20) << "Filename Details: " << filename << std::endl;
   std::cout << std::endl;

   std::cout << std::left << std::setw(20) << "Version: "    << mdversion << std::endl;
   std::cout << std::left << std::setw(20) << "Filesize: "   << filesize  << std::endl;
   std::cout << std::left << std::setw(20) << "Blocksize: "  << blocksize << std::endl;


   std::cout << std::left << std::setw(20) << "Blockcount: "     << blockcount << std::endl;
   std::cout << std::left << std::setw(20) << "Blockremainder: " << blockremainder << std::endl;
   
   std::cout << std::left << std::setw(20) << "Modsize: "        << modsize << std::endl;
   std::cout << std::left << std::setw(20) << "Modsize Bytes: "  << modsizeBytes << std::endl;
   std::cout << std::left << std::setw(20) << "Filehashlist: "   << filehashnames << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashlist: "  << blockhashnames << std::endl;

   std::cout << std::left << std::setw(20) << "File Hash Bytes: "  << hclfileblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Block Hash Bytes: " << hclblockblocksize << std::endl;
   if (mdlist == false) std::cout << std::left << std::setw(20) << "Threadcount:" << threadcount << std::endl;
   std::cout << std::left << std::setw(20) << "Platform:" << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;

   // display the file hash list parameters and hash block size
   std::cout << std::endl;
   std::cout << "File hashlist " << std::endl;
   std::cout << filehashvector << std::endl;

   // TODO display the hashblockgroup 
   // std::cout << "File block group hash list " << std::endl;
   // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

   // display the file block hash list
   std::cout << "File block hashlist " << std::endl;
   std::cout << blockhashvector << std::endl;
}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   ./mdunzip --file=filename.mdz --thread=16 
   ./mdunzip --file=test.mdz --thread=16 
   ./mdunzip --file=test.mdz --list=true
   ./mdunzip --file=filename.mdz --list=true --unzip=false
)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

