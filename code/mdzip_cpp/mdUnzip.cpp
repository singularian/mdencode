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
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "../testdecode_cpp/external/CLI11.hpp"
#include <gmp.h>
#include "common.h"
#include "mdHashContextList.h"

using namespace std;

int mdlist(std::string filename, bool listfile, bool runlogging);
int mdunzipfile(std::string filename, bool runlogging);
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
     if (runmdzip) mdunzipfile(filename, runlogging);

     return 0;
}

// mdlist
// display the mdzip block contents of a file
int mdlist(std::string filename, bool listfile, bool runlogging) {

   size_t inputfilesize = 0;
   // mdzip variables
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

   std::cout << std::left << std::setw(20) << "Filename Details: " << filename << std::endl << std::endl;
   std::cout << std::left << std::setw(20) << "Version: "    << mdversion << std::endl;
   std::cout << std::left << std::setw(20) << "Filesize: "   << filesize  << std::endl;
   std::cout << std::left << std::setw(20) << "Blocksize: "  << blocksize << std::endl;

   // calculate the file block count and last block size
   blockcount = CalcFileBlocks(filesize, blocksize);
   blockremainder  = filesize % blocksize;

   std::cout << std::left << std::setw(20) << "Blockcount: "     << blockcount << std::endl;
   std::cout << std::left << std::setw(20) << "Blockremainder: " << blockremainder << std::endl;
   
   std::cout << std::left << std::setw(20) << "Modsize: " << modsize << endl;
   std::cout << std::left << std::setw(20) << "Modsize Bytes: "  << modsizeBytes << std::endl;
   std::cout << std::left << std::setw(20) << "Filehashlist: "   << filehashnames << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashlist: "  << blockhashnames << std::endl;

   mdHashContextList hclfile;
   mdHashContextList hclblock;

   // set the hash list vector tuple for file and hash blocks
   hclfile.setVectorHLstring(filehashnames, HASHBLOCK);
   hclblock.setVectorHLstring(blockhashnames, HASHBLOCK);

   // calculate the file and file block hash list size
   int hclfileblocksize  = hclfile.calcBlockSize(HASHBLOCK);
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   std::cout << std::left << std::setw(20) << "File Blocksize: " << hclfileblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Block Blocksize: " << hclblockblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Platform:" << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;

   // display the file hash list parameters and hash block size
   std::cout << std::endl;
   std::cout << "File hashlist " << std::endl;
   std::cout << hclfile.getHLvectorsString(HASHBLOCK) << std::endl;

   // TODO display the hashblockgroup 
   // std::cout << "File block group hash list " << std::endl;
   // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

   // display the file block hash list
   std::cout << "File block hashlist " << std::endl;
   std::cout << hclblock.getHLvectorsString(HASHBLOCK) << std::endl;

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
// TODO mdunzip a valid mdzip file 
// ie a file with the *.mdz extension
// ie file.mdz file
int mdunzipfile(std::string filename, bool runlogging) {

   std::cout << "mdunzipping file " << filename << std::endl;
   return 0;

}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   ./mdunzip --file=filename.mdz --thread=16 
   ./mdunzip --file=test.mdz --thread=16 
   ./mdunzip --file=test.mdz --list=true
)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

