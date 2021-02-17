/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan MDzip Program
 * 
 * mdZip.cpp zips a file with the mdzip format 
 * 
 * The current mdzip file extension is mdz
 *
 * 
 * 
*/
#include <iostream>
#include <sys/stat.h>
#include "external/CLI11.hpp"
#include <gmp.h>
#include <gmpxx.h>
#include "mdCore/mdCommon.h"
#include "mdCore/mdHashContextList.h"

int mdzipfile(std::string filename, long blocksize, int modsize, std::vector<int> &fhlist, std::vector<int> &bhlist, bool randombh);
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, 
                 int modsizeBytes, std::string& filehashnames, std::string& blockhashnames, int hclfileblocksize, int hclblockblocksize, 
                 std::string& filehashvector, std::string& blockhashvector, std::string& blockkeys, std::string& filesig, bool mdlist, int threadcount );
void displayBlockInfo(std::string action, unsigned char *byteblock, long currentblocksize, long blocksize, int blk, int lastblk, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock);
void usage();

using namespace std;

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
     CLI::App app{"MDEncode MDzip C++ Program"};
     app.add_option("-f,--file",    filename,    "MDzip filename")->check(CLI::ExistingFile)->required();
     app.add_option("-b,--block",   blocksize,   "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
     app.add_option("-m,--mod",     modsize,     "Modulus size number")->check(CLI::PositiveNumber);
     app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // add the file hash list parameter
     std::vector<int> flcsvvals;    
     app.add_option("--fhs", [&flcsvvals, &signum](std::vector<std::string> val){
        return splitRange(val, flcsvvals, signum);
     }, "File Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

     // integer file hash list parameter
     std::vector<int> flvals;
     app.add_option("--fh", [&flvals, &signum](std::vector<std::string> val){
        return splitRange(val, flvals, signum);
     }, "File Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

     // add the csv block hashlist parameter   
     std::vector<int> csvvals;
     app.add_option("-r,--bhs", [&csvvals, &signum](std::vector<std::string> val){
        return splitRange(val, csvvals, signum);
     }, "File Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

     // integer block hash list parameter
     std::vector<int> intvals;
     app.add_option("-s,--bh", [&intvals, &signum](std::vector<std::string> val){
        return splitRange(val, intvals, signum);
     }, "Block Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

     // add a hash keylist parameter
     // TODO Not currently used
     bool randkey;
     std::string keylist;
     app.add_option("-k,--keylist", keylist, "Keylist csv string");

     // TODO randomize the keylist for the file hashes

     // randomize the keylist for the block hashes
     bool randombh = false;
     app.add_flag("--randbh", randombh, "Randomize the Block Hash Keylist");

     // set list blocks not currently used
     // bool listzip = false;
     // app.add_option("-x,--list", listzip, "Display the Block list");

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

     // process the file hashlists
     // combine the flcsvvals and flvals vectors
     flcsvvals.insert(flcsvvals.end(), flvals.begin(), flvals.end());

     // process the block hashlists 
     // combine csvvals and the integer vals block hash list
     // the hash context list will unique them in non sorted order
     csvvals.insert(csvvals.end(), intvals.begin(), intvals.end());

     if (csvvals.size() == 0) {
        csvvals = { FAST64 };
     }    

     // run mdzipfile
     mdzipfile(filename, blocksize, modsize, flcsvvals, csvvals, randombh);

}

// mdzip an input file
// current mdzip extension is .mdz
// this is currently litte endian and 64 bit for the longs
int mdzipfile(std::string filename, long blocksize, int modsize, std::vector<int> &fhlist, std::vector<int> &bhlist, bool randombh) {

     long blocknumber = 1;
     double mdversion = 1.01;
     int modexponent  = 0;
     long filesize    = 0;
     filesize = GetFileSize(filename);
     // need to add the mdformat

     if (filesize < blocksize) blocksize = filesize;
     if (filesize == 0) return 0;

     // calculate the file block count and last block size
     long blockcount = CalcFileBlocks(filesize, blocksize);

     // calculate the last block size
     long blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

     // the curent block size
     long currentblocksize = blocksize;

     // set the mdzip filename with the extension
     std::string mdzipfile = filename + ".mdz";

     const char *fname = mdzipfile.c_str();
     std::remove(fname);

     // set the file hash list
     mdHashContextList hclfile;
     hclfile.setVectorHL(fhlist, HASHBLOCK);
     hclfile.setFileHashList(filename);

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
    
     // need to make sure these are byte order independent
     // the block size header is currently 28 bytes
     mdzip.write(reinterpret_cast<char*>(&mdversion), sizeof(double));
     mdzip.write(reinterpret_cast<char*>(&filesize),  sizeof(long));
     mdzip.write(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
     mdzip.write(reinterpret_cast<char*>(&modsize),   sizeof(int));

     // initialize the block hash context list
     mdHashContextList hclblock;
     // set the block hash list with the block hash list vector 
     hclblock.setVectorHL(bhlist, HASHBLOCK);
     // display the block hash list vector
     std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);

     // write the hash list strings
     // TODO need to write the keylist as well
     // ie aes8:sha1:md5-aes8-keylist
     int hclsize;
     std::string filehashnames  = hclfile.getHLvectorsStringNames(HASHBLOCK);
     std::string blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);

     // write the file hash list names
     hclsize = filehashnames.size();
     mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
     // wf.write(reinterpret_cast<char*>(&filehashnames),   hclsize);
     mdzip.write(filehashnames.c_str(),   hclsize);

     // write the block hash list names string 
     hclsize = blockhashnames.size();
     mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
     // wf.write(reinterpret_cast<char*>(&blockhashnames),   hclsize);
     mdzip.write(blockhashnames.c_str(),   hclsize);

     // write the file hash list to the mdzip file
     // TODO write the file hash key random list
     hclfile.writeBlockHashList(mdzip);
     //std::cout << "file hash list " << std::endl;
     //std::cout << hclfile.displayHLhashes() << std::endl;
     std::string filesigs = hclfile.displayHLhashes();

     // TODO need to also write the block group hash list if implemented

     // write the block hash keys list
     // if randomblock or randbh is set it uses the blockhashnames
     // TODO allow the user to specify a block hash key list 
     // For now I can make it use the same list as the blockhash list
     std::string blockkeys = "default";
     if (randombh == true) {
         hclsize = blockhashnames.size();
         mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
         // std::cout << "bhash " <<  blockhashnames << std::endl;
         // wf.write(blockhashnames.c_str(),   hclsize); 
         hclblock.randomizeKeyList();
         hclblock.writeKeyList(mdzip);
         blockkeys = hclblock.displayHLhashKeys();
     } else {
         // write the hash size as a zero byte
         hclsize = 0;
         mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int)); 
     }   
 
     // create the byteblock buffer with the blocksize 
     unsigned char *byteblock  = new unsigned char[blocksize];

     // calculate the correct modulus byte size in case of a odd modulus size 33
     // the modulus parameter is in bits and this converts it to bytes 
     int modsizeBytes = calcModulusBytes(modsize);
     unsigned char *modulusint = new unsigned char[modsizeBytes];

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
     blockhashnames, hclfileblocksize, hclblockblocksize, filehashvector,  blockhashvector, blockkeys, filesigs, true, 0);

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
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, 
                 int modsize, int modsizeBytes, std::string& filehashnames, std::string& blockhashnames, int hclfileblocksize,
                 int hclblockblocksize, std::string& filehashvector,  std::string& blockhashvector, std::string& blockkeys, 
                 std::string& filesig, bool mdlist, int threadcount ) {


   std::cout << std::left << std::setw(20) << "Filename Details: " << filename << std::endl;

   std::cout << std::left << std::setw(20) << "Version: "    << mdversion << std::endl;
   std::cout << std::left << std::setw(20) << "Filesize: "   << filesize  << std::endl;
   std::cout << std::left << std::setw(20) << "Blocksize: "  << blocksize << std::endl;


   std::cout << std::left << std::setw(20) << "Blockcount: "     << blockcount << std::endl;
   std::cout << std::left << std::setw(20) << "Blockremainder: " << blockremainder << std::endl;
   
   std::cout << std::left << std::setw(20) << "Modsize: "        << modsize << std::endl;
   std::cout << std::left << std::setw(20) << "Modsize Bytes: "  << modsizeBytes << std::endl;
   std::cout << std::left << std::setw(20) << "Filehashlist: "   << filehashnames << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashlist: "  << blockhashnames << std::endl;
   std::cout << std::left << std::setw(20) << "Filehashlist size: "  << filehashnames.size() << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhashlist size: " << blockhashnames.size() << std::endl;
   std::cout << std::left << std::setw(20) << "Blockhash key list: "   << blockkeys << std::endl;

   std::cout << std::left << std::setw(20) << "File Hash Bytes: "  << hclfileblocksize << std::endl;
   std::cout << std::left << std::setw(20) << "Block Hash Bytes: " << hclblockblocksize << std::endl;
   if (mdlist == false) std::cout << std::left << std::setw(20) << "Threadcount:" << threadcount << std::endl;
   std::cout << std::left << std::setw(20) << "Platform:" << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;

   // display the file hash list parameters and hash block size
   std::cout << std::endl;
   std::cout << "File hashlist " << std::endl;
   std::cout << filehashvector   << std::endl;

   // TODO display the hashblockgroup 
   // std::cout << "File block group hash list " << std::endl;
   // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

    // display the file block hash list
   std::cout << "File block hashlist " << std::endl;
   std::cout << blockhashvector << std::endl;

   std::cout << "File Signatures " << std::endl;
   std::cout << filesig << std::endl;
   std::cout << std::endl;
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
MDzip Examples:
   mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
   mdzip --file=randfile --block=14 --mod=32 --fh 13     --bh 5        --randbh
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh

MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=16 
   mdunzip --file=test.mdz --list
   mdunzip --file=filename.mdz --list --unzip 
   mdunzip --file=filename.mdz --valmdzip 
)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

