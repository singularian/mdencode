/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdzip Program
 * 
 * mdZipSimple.cpp zips a file with the mdzip format 
 * filename.mdsz
 * 
 * It uses one 64 bit hash fasthash64
 * and a filesize header uint64_t 
 * TODO add a 64 bit key
 * and a 14 byte block size
 * the modulus size is 32 bits
 *
 * 
 * 
*/
#include <iostream>
#include <sys/stat.h>
#include "../testdecode_cpp/external/CLI11.hpp"
#include <gmp.h>
#include "mdCore/mdCommon.h"
#include "mdCore/mdHashContextList.h"

int mdzipfileNoHeader(std::string filename, long blocksize, int modsize, uint64_t key, std::vector<int> &bhlist);
void usage();

using namespace std;

int main (int argc, char **argv) {

     std::string filename;
     long filesize;
     long blockcount;
     long blocksize   = 14;
     int modsize      = 32;
     uint64_t key     = 1;
     int threadnumber = 0;
     int threadcount  = 8;
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode Minimized MDzip C++ Program"};
     app.add_option("-f,--file",    filename,    "MDzip filename")->check(CLI::ExistingFile)->required();

     // add the block hashlist parameter
     std::vector<int> blocklist = { 5 };

     // add a hash keylist parameter
     // bool randkey;
     // std::string keylist;
     // app.add_option("-k,--keylist", keylist, "Keylist csv string");
     app.add_option("-k,--key", key, "Set fasthash key number")->check(CLI::PositiveNumber);

     // randomize the keylist for the hashes
     bool random = false;
     app.add_option("-r,--rand", random, "Randomize the Key");

     // set logging
     bool runlogging = false;
     app.add_option("-l,--log", runlogging, "Run Logging");

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

     cout << "Hash list ";
     for(int i=0; i < bhlist.size(); i++)
         std::cout << bhlist.at(i) << ' ';
     cout << std::endl;

     long blockcount = CalcFileBlocks(filesize, blocksize);
     long blockremainder  = filesize % blocksize;

     std::cout << "File size " << filesize << std::endl;
     std::cout << "File block number "    << blockcount << std::endl;
     std::cout << "File last block size " << blockremainder << std::endl;

     std::string mdzipfile = filename + ".mdsz";

     const char *fname = mdzipfile.c_str();
     std::remove(fname);


     std::ifstream nf(filename, std::ios::in | std::ios::binary);
     std::ofstream wf(mdzipfile, std::ios::out | std::ios::binary);

     if(!wf) {
        std::cout << "Cannot open file!" << std::endl;
        return 1;
     }

     // initialize the gmp bigint variables
     int byteorder = 0;
     int endian    = 0;
     size_t count;
     mpz_t remainder, modulusInt, byteblockInt;
     mpz_init_set_str(remainder, "0", 10);
     mpz_init_set_str(modulusInt, "1", 10);
     mpz_init_set_str(byteblockInt, "0", 10);
    
     // need to make sure these are byte order independent
     // the block size header is currently 28 bytes
     wf.write(reinterpret_cast<char*>(&filesize),  sizeof(long));
     // need to write the uint64_t key
     // 64-bit key for the fasthash64 or the 64 bit key
     // TODO write the fasthash64 uint64_t key
     // =================================================
     // *************************************************
     // I think I broke this the key is different TO DO 
     // *************************************************
     wf.write(reinterpret_cast<char*>(&key),  sizeof(long));

     // initailize the block hash context list
     mdHashContextList hclblock;
     // set the block hash list with the block hash list vector 
     std::vector<int> blocklist = { 5 };
     hclblock.setVectorHL(blocklist, HASHBLOCK);

     // set the key
     hclblock.hregister[0].fast64seed = key;

     // display the block hash list vector
     std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
     cout << std::endl;
     cout << "Hash Block Vector" << endl;
     cout << vectorlist << std::endl;
 
     // constexpr size_t bufferSize = blocksize;
     // unique_ptr<unsigned char[]> byteblock(new unsigned char[blocksize]);
     // unique_ptr<char[]> byteblock(new char[blocksize]);
     unsigned char *byteblock  = new unsigned char[blocksize];
     // need to add the last byte block size
     unsigned char *lastbyteblock = new unsigned char[blockremainder];

     // calculate the correct modulus byte size in case of a odd modulus size 33
     // the modulus parameter is in bits and this converts it to bytes 
     int modsizeBytes = calcModulusBytes(modsize);
     cout << "Modsize bytes " << modsizeBytes << endl << endl;
     unsigned char *modulusint = new unsigned char[modsizeBytes];

     while (nf)
     {

        if (blocknumber < blockcount) {
           std::cout << "Writing Block " << std::to_string(blocknumber) << " " << std::to_string(blocksize) << "/" << std::to_string(blocksize) << endl;
           // nf.read(byteblock.get(), blocksize);
           // hclblock.setByteBlockHashList(byteblock.get(), blocksize);
           nf.read(reinterpret_cast<char*>(byteblock), (size_t) blocksize);
           hclblock.setByteBlockHashList((unsigned char*) byteblock, blocksize);
           hclblock.writeBlockHashList(wf);
           mpz_import (byteblockInt, blocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);
           // display data in the byteblock
           printByteblock(byteblock, blocksize, true);

           std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
           std::cout << hclblock.displayHLhashes() << std::endl;


           // calculate the modulus 2 ^ modsize 
           mpz_ui_pow_ui (modulusInt, 2, modsize);

           // calculate the modulus remainder 
           mpz_mod (remainder, byteblockInt, modulusInt);

           int modexponent = calcExponent(byteblockInt);
           // calculate the modulus exponent with two
           // this should be a byte int if the file block size is less than 32 and an 32-bit int if it is greater than 32 bytes
           if (blocksize > 32) { 
               wf.write(reinterpret_cast<char*>(&modexponent),   sizeof(int));
           } else {
               uint8_t modexponent2 = modexponent;
               wf.write(reinterpret_cast<char*>(&modexponent2),   sizeof(uint8_t));
           }
 
           std::cout << "Modulus Exponent " << modexponent << std::endl;

           // write the modulus remainder
           mpz_export(modulusint, &count, byteorder, sizeof(modulusint[0]), endian, 0, remainder);
           // =========================================================================================
           printByteblock(modulusint, modsizeBytes, true);
           cout << "padding modus int " << endl;
           padBlockBytes(count, modsizeBytes, modulusint);
           
           printByteblock(modulusint, modsizeBytes, true);
           // =========================================================================================


           // wf.write(reinterpret_cast<char*>(&modulusint),   sizeof(char) * modsizeBytes);
           wf.write(reinterpret_cast<char*>(modulusint),   sizeof(char) * modsizeBytes);
           // wf.write(modulusint,   sizeof(char) * modsizeBytes);

           gmp_printf("Modulus Remainder %Zd\n\n", remainder);

        // } else if ((blocknumber == blockcount) && (blockremainder > 0)) {
        } else if ((blocknumber ) == (blockcount )) {
           if (blockremainder > 0) {
               std::cout << "Writing Block " << std::to_string(blocknumber) << " " << std::to_string(blockremainder) << "/" << std::to_string(blocksize) << endl;
               nf.read(reinterpret_cast<char*>(lastbyteblock), (size_t) lastbyteblock);
               hclblock.setByteBlockHashList((unsigned char*) lastbyteblock, blockremainder);
               hclblock.writeBlockHashList(wf);
               mpz_import (byteblockInt, blockremainder, byteorder, sizeof(lastbyteblock[0]), endian, 0, lastbyteblock);
               // display data in the byteblock
               printByteblock(lastbyteblock, blockremainder, true);
           } else {
              std::cout << "Writing Block " << std::to_string(blocknumber) << " " << std::to_string(blocksize) << "/" << std::to_string(blocksize) << endl;
              nf.read(reinterpret_cast<char*>(byteblock), (size_t) blocksize);
              hclblock.setByteBlockHashList((unsigned char*) byteblock, blocksize);
              hclblock.writeBlockHashList(wf);
              mpz_import (byteblockInt, blocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

              // display data in the byteblock
              printByteblock(byteblock, blocksize, true);

           }

           std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
           std::cout << hclblock.displayHLhashes() << std::endl;


           // calculate the modulus 2 ^ modsize 
           mpz_ui_pow_ui (modulusInt, 2, modsize);

           // calculate the modulus remainder 
           mpz_mod (remainder, byteblockInt, modulusInt);

           // calculate the modulus exponent with two
           // this should be a byte int if the file block size is less than 32 and an 32-bit int if it is greater than 32 bytes
           int modexponent = calcExponent(byteblockInt);
           if (blocksize > 32) { 
               wf.write(reinterpret_cast<char*>(&modexponent),   sizeof(int));
           } else {
               uint8_t modexponent2 = modexponent;
               wf.write(reinterpret_cast<char*>(&modexponent2),   sizeof(uint8_t));
           }

           std::cout << "Modulus Exponent " << modexponent << std::endl;

           // write the modulus remainder
           mpz_export(modulusint, &count, byteorder, sizeof(modulusint[0]), endian, 0, remainder);
           // cout << "padding modus int " << endl;
           padBlockBytes(count, modsizeBytes, modulusint);
           wf.write(reinterpret_cast<char*>(modulusint),   sizeof(char) * modsizeBytes);

           gmp_printf("Modulus Remainder %Zd\n\n", remainder);
           break;
        }
        blocknumber++;
     }

     delete byteblock;
     delete lastbyteblock;
     delete modulusint;

     nf.close();
     wf.close();

     if(!wf.good()) {
       cout << "Error occurred at writing time!" << endl;
       // return 1;
     }

     /* free used memory */
     mpz_clear(remainder);
     mpz_clear(modulusInt);
     mpz_clear(byteblockInt);


     return 0;

}

// display the usage
void usage() {
std::string usageline = R"(
Examples:
   mdzipnh --file=test.txt  
   mdzipnh --file=test.txt 
   mdzipnh --file=test.txt  

   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list=true
   mdunzipnh --file=filename.mdsz --list=true --unzip=false
)";

    std::cout << usageline << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    // cout << "Hashlist:" << endl << endl;
    // mdHashContextList hcl;
    // hcl.displayHLRegistry(0);

}

