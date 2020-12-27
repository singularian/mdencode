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


     // size_t sz = 0;
     // string filename = "file.txt";
     // sz = getFilesize(filename);

   mdlist(filename, list, runlogging);
   mdunzipfile(filename, runlogging);

   return 0;
}

// mdlist
int mdlist(std::string filename, bool listfile, bool runlogging) {

   double mdversion    = 1.01;
   long blocksize      = 0;
   long blockcount     = 0;
   long blockremainder = 0;
   long filesize       = 0;
   int modexponent     = 0;
   int modsize         = 0;
   int hclfilesize;
   int hclblocksize;
   std::string filehashnames;   
   std::string blockhashnames;

   // if the listfile boolean is false don't run the list mdzip file
   if (!listfile) return 0;

   std::ifstream nf(filename, std::ios::in | std::ios::binary);
   if(!nf) {
      cout << "Cannot open file!" << endl;
      return 1;
   }

   nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
   nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
   nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
   nf.read(reinterpret_cast<char*>(&modsize),   sizeof(int));

   nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int));
   char* buf = new char[hclfilesize];
   // nf.read(reinterpret_cast<char*>(&filehashnames),  hclfilesize);
   if (hclfilesize > 0) {
     nf.read(buf,  hclfilesize);
     filehashnames.append(buf, hclfilesize);
     delete buf;
   }

   nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int));
   char* buf2 = new char[hclblocksize];
   nf.read(buf2,  hclblocksize);
   blockhashnames.append(buf2, hclblocksize);
   delete buf2;
   // nf.read(reinterpret_cast<char*>(&blockhashnames), hclblocksize);

   cout << std::left << std::setw(20) << "Filename Details: " << filename << endl << endl;
   cout << std::left << std::setw(20) << "Version: "    << mdversion << endl;
   cout << std::left << std::setw(20) << "Filesize: "   << filesize << endl;
   cout << std::left << std::setw(20) << "Blocksize: "  << blocksize << endl;

   blockcount = CalcFileBlocks(filesize, blocksize);
   blockremainder  = filesize % blocksize;

   cout << std::left << std::setw(20) << "Blockcount: "     << blockcount << endl;
   cout << std::left << std::setw(20) << "Blockremainder: " << blockremainder << endl;
   
   // std::cout << "File block number "    << blockcount << std::endl;
   // std::cout << "File last block size " << blockremainder << std::endl;

   cout << std::left << std::setw(20) << "Modsize: " << modsize << endl;
   // cout << "Filehashlist: "   << hclfilesize << endl;
   // cout << "Filehashlist: "   << filehashnames.c_str() << endl;
   cout << std::left << std::setw(20) << "Filehashlist: "   << filehashnames << endl;
   // cout << "Blockhashlist: "  << blockhashnames.c_str() << endl;
   cout << std::left << std::setw(20) << "Blockhashlist: "  << blockhashnames << endl;

   mdHashContextList hclfile;
   mdHashContextList hclblock;

   hclfile.setVectorHLstring(filehashnames, HASHBLOCK);
   hclblock.setVectorHLstring(blockhashnames, HASHBLOCK);

   int hclfileblocksize  = hclfile.calcBlockSize(HASHBLOCK);
   int hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

   std::cout << std::left << std::setw(20) << "File Blocksize: " << hclfileblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Block Blocksize: " << hclblockblocksize << std::endl;

   std::cout << std::endl;
   std::cout << "File hashlist " << std::endl;
   std::cout << hclfile.getHLvectorsString(HASHBLOCK) << std::endl;

   // std::cout << "File block group hash list " << std::endl;
   // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

   std::cout << "File block hashlist " << std::endl;
   std::cout << hclblock.getHLvectorsString(HASHBLOCK) << std::endl;

   // std::cout << std::left << std::setw(20) << "File Blocksize: " << hclfileblocksize << std::endl;
   // std::cout << std::left << std::setw(20) << "Block Blocksize: " << hclblockblocksize << std::endl;

   // file hash block
   // block signatures / modulus exponent / modulus remainder


   nf.close();

   return 0;

}


// mdunzipfile
int mdunzipfile(std::string filename, bool runlogging) {


   return 0;

}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
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
