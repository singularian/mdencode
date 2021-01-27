#include <cstring>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <sstream>
#include "../mdCore/mdHashContextList.h"
#include "../testdecode_cpp/external/CLI11.hpp" 

int main (int argc, char **argv) {

    // current signature number handled in the hash context list
    int signum       = LAST - 1;

    // process the command line argument with the CLI11 command line parser
    CLI::App app{"MDEncode GMP C++ Test Program"};

    std::string filename;
    app.add_option("-f,--file",    filename,    "MDunzip filename")->check(CLI::ExistingFile)->required();

    // std::vector<int> vals;
    std::vector<int> fhlist = { 1, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 26 };
    app.add_option("-s,--hl", fhlist, "Block Hashlist integers list")->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (argc < 2)
    {
         cout << app.help() << endl;
         // usage();
         return 0;
     }
    

    // initialize the hash list object
    mdHashContextList hclfile;

    // display the current hash signature list
    // hclfile.displayHLRegistry(0);

    // initialize a file hash list vector  
    // hclfile.setVectorHL(file, HASHFILE);
    hclfile.setVectorHL(fhlist, HASHBLOCK);
    hclfile.setFileHashList(filename); // set the file hash list variables

    // display the block hash list
    std::string vectorlist = hclfile.getHLvectorsString(HASHBLOCK);
    // std::string hashlist   = hclfile.displayHLhashes();

    std::cout << "hashlist "  << vectorlist << std::endl;

    std::cout << "file hash list " << hclfile.displayHLhashes() << std::endl;

/*
    // TODO initialize a block group hash list vector
    // maybe make the parameters a double or float
    // ie 1.1 is files
    // 1.2 is blocks groups hash 1
    // 1.3 is blocks hash list
    // 1.4 is everything with hash signature 1
    std::vector<int> blockgroup = { 16, 17, 20, 30 };
    hcl.setVectorHL(blockgroup, HASHBLOCKGROUP);

    // initialize a block hash list vector
    std::vector<int> block = { 1, 2, 3, 15, 16, 17 };
    hcl.setVectorHL(block, HASHBLOCK);

    // display the file and blockgroup and blockhash list input vectors
    hcl.displayHLvectors();
    std::cout << std::endl;

    // set the byteblock hashlist
    unsigned char byteblock [] = { 139, 176, 100, 82, 220, 98, 148, 121, 155, 202, 74 };
    hcl.setByteBlockHashList(byteblock, 11);

    // return the vector lengths
    std::cout << "Hash file block size "  << hcl.getHLVectorLength(HASHFILE) << std::endl;
    std::cout << "Hash block group size " << hcl.getHLVectorLength(HASHBLOCKGROUP) << std::endl;
    std::cout << "Hash block size "       << hcl.getHLVectorLength(HASHBLOCK) << std::endl;
    std::cout << std::endl;

    // calculate the hash block size
    std::cout << "Hash file block size "  << hcl.calcBlockSize(HASHFILE) << std::endl;
    std::cout << "Hash block group size " << hcl.calcBlockSize(HASHBLOCKGROUP) << std::endl;
    std::cout << "Hash block size "       << hcl.calcBlockSize(HASHBLOCK) << std::endl;
    std::cout << std::endl;

    // display the block hash list
    std::cout << hcl.displayHLhashes() << std::endl;
*/
}

