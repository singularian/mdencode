/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdzip Program
 * 
 * mdZip.cpp zips a file with the mdzip format 
 *
 * 
 * 
*/
#include <iostream>
#include <sys/stat.h>
#include "../testdecode_cpp/external/CLI11.hpp"
#include <gmp.h>
#include "common.h"
#include "mdHashContextList.h"

// int calcExponent (mpz_t blockint);
int mdzipfile(std::string filename, long blocksize, int modsize, std::vector<int> &fhlist, std::vector<int> &bhlist);
void printByteblock(unsigned char *byteblock, long blocksize, bool ishex);
// void printByteblock(char *byteblock, long blocksize, bool ishex);
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
     app.add_option("-t,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // add the file hash list parameter
     std::vector<int> filelist;
     std::vector<int> flcsvvals;
     std::vector<int> flvals;
     app.add_option("--fhs", flcsvvals, "File Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

     app.add_option("--fh", flvals, "File Hashlist integers list")->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

     // add the block hashlist parameter
     // std::string hashlist;
     std::vector<int> blocklist = { 1 };
     std::vector<int> vals;
     std::vector<int> csvvals;
     // std::vector<std::string> csvvals;
     // app.add_option("-r,--bh", vals, "Block Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber);
     app.add_option("-r,--bhs", csvvals, "Block Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));
     // app.add_option("-r,--bh", csvvals, "Block Hashlist csv string")->delimiter(',');

     // std::vector<int> vals;
     app.add_option("-s,--bh", vals, "Block Hashlist integers list")->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

     // add a hash keylist parameter
     bool randkey;
     std::string keylist;
     app.add_option("-k,--keylist", keylist, "Keylist csv string");

     // randomize the keylist for the hashes
     bool random = false;
     app.add_option("--rand", random, "Randomize Keylist");

     // set list blocks
     bool listzip = false;
     app.add_option("-x,--list", listzip, "Display the Block list");

     // set logging
     bool runlogging = false;
     app.add_option("-l,--log", runlogging, "Run Logging");

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

     // process the file hashlist
     filelist.insert(flcsvvals.end(), flvals.begin(), flvals.end());

     // process the block hashlist
     csvvals.insert(csvvals.end(), vals.begin(), vals.end());
     if (csvvals.size() > 0) {
         blocklist.clear();
         blocklist.insert(blocklist.end(), csvvals.begin(), csvvals.end());
     }

     // call mdzipfile
     mdzipfile(filename, blocksize, modsize, filelist, blocklist);

}

// int mdzipfile(std::string filename, mdheader mdh) {
int mdzipfile(std::string filename, long blocksize, int modsize, std::vector<int> &fhlist, std::vector<int> &bhlist) {

     long blocknumber = 1;
     double mdversion = 1.01;
     int modexponent  = 0;
     long filesize    = 0;
     filesize = GetFileSize(filename);
     // need to add the mdformat

     if (filesize < blocksize) blocksize = filesize;
     if (filesize == 0) return 0;

     cout << "Hash list ";
     for(int i=0; i < bhlist.size(); i++)
         std::cout << bhlist.at(i) << ' ';
     cout << std::endl;

     long blockcount = CalcFileBlocks(filesize, blocksize);
     long blockremainder  = filesize % blocksize;
     std::cout << "File block number "    << blockcount << std::endl;
     std::cout << "File last block size " << blockremainder << std::endl;

     std::string mdzipfile = filename + ".mdz";

     const char *fname = mdzipfile.c_str();
     std::remove(fname);

     // set the file hash list
     mdHashContextList hclfile;
     hclfile.setVectorHL(fhlist, HASHBLOCK);
     hclfile.setFileHashList(filename);

     std::ifstream nf(filename, std::ios::in | std::ios::binary);
     std::ofstream wf(mdzipfile, std::ios::out | std::ios::binary);
     // std::ofstream wf(fname, std::ios::out | std::ios::binary);
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
     wf.write(reinterpret_cast<char*>(&mdversion), sizeof(double));
     wf.write(reinterpret_cast<char*>(&filesize),  sizeof(long));
     wf.write(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
     wf.write(reinterpret_cast<char*>(&modsize),   sizeof(int));
     // wf << mdh.version;
     // wf << mdh.name;
     // nf.close();
     // wf.close();
     // return 0; // test


     // TODO need to write the file hash list 
     // TODO need to also write the block group hash list if implemented

     // initailize the block hash context list
     mdHashContextList hclblock;
     // set the block hash list with the block hash list vector 
     hclblock.setVectorHL(bhlist, HASHBLOCK);
     // display the block hash list vector
     std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);
     cout << std::endl;
     cout << "Hash Block Vector" << endl;
     cout << vectorlist << std::endl;

     // write the hash list strings
     // TODO need to write the keylist as well
     // ie aes8:sha1:md5-aes8-keylist
     int hclsize;
     std::string filehashnames  = hclfile.getHLvectorsStringNames(HASHBLOCK);
     std::string blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);
     cout << "hash string file " << filehashnames << endl;
     cout << "hash string block " << blockhashnames << endl;

     hclsize = filehashnames.size();
     wf.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
     // wf.write(reinterpret_cast<char*>(&filehashnames),   hclsize);
     wf.write(filehashnames.c_str(),   hclsize);
     cout << "hash size " << hclsize << endl;

     hclsize = blockhashnames.size();
     wf.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
     // wf.write(reinterpret_cast<char*>(&blockhashnames),   hclsize);
     wf.write(blockhashnames.c_str(),   hclsize);
     //nf.close();
     //wf.close();
     //return 0; // test

 
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
           std::cout << "Block " << std::to_string(blocknumber) << " " << std::to_string(blocksize) << "/" << std::to_string(blocksize) << endl;
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
           cout << "padding modustint " << endl;
           padBlockBytes(count, modsizeBytes, modulusint);
           
           printByteblock(modulusint, modsizeBytes, true);
           // =========================================================================================


           wf.write(reinterpret_cast<char*>(&modulusint),   sizeof(char) * modsizeBytes);

           gmp_printf("Modulus Remainder %Zd\n\n", remainder);

        // } else if ((blocknumber == blockcount) && (blockremainder > 0)) {
        } else if ((blocknumber ) == (blockcount )) {
           if (blockremainder > 0) {
               std::cout << "Block " << std::to_string(blocknumber) << " " << std::to_string(blockremainder) << "/" << std::to_string(blocksize) << endl;
               nf.read(reinterpret_cast<char*>(lastbyteblock), (size_t) lastbyteblock);
               hclblock.setByteBlockHashList((unsigned char*) lastbyteblock, blockremainder);
               hclblock.writeBlockHashList(wf);
               mpz_import (byteblockInt, blockremainder, byteorder, sizeof(lastbyteblock[0]), endian, 0, lastbyteblock);
               // display data in the byteblock
               printByteblock(lastbyteblock, blockremainder, true);
           } else {
              std::cout << "Block " << std::to_string(blocknumber) << " " << std::to_string(blocksize) << "/" << std::to_string(blocksize) << endl;
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
           wf.write(reinterpret_cast<char*>(&modulusint),   sizeof(char) * modsizeBytes);

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

// calculates an exponent of 2 less than the byte block int
/* int calcExponent (mpz_t blockint) {
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
*/

// display the byteblock
// void printByteblock(char *byteblock, long blocksize, bool ishex) {
void printByteblock(unsigned char *byteblock, long blocksize, bool ishex) {
        long i;
        for(i=0; i < blocksize; i++)
        {
            if (ishex == false) {
                printf("%d ",    byteblock[i]);
            } else {
                printf("%02X ", byteblock[i]);
            }
        }

        printf("\n");

        /* for (int f = 0; f < blocksize; f++) {
                  // std::cout << setw(2) << std::uppercase << std::hex << setfill('0') << (uint32_t)byteblock[f];
                  std::cout << " ";
        }
        std::cout << std::endl;
        */

}


// display the usage
void usage() {
std::string usageline = R"(
Examples:
   ./mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4 
)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

