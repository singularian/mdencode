#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/sha.h>
#include <sstream>
#include "../mdHashContextList.h"

int main()
{
    // initialize the hash list object
    mdHashContextList hcl;

    // display the current hash signature list
    hcl.displayHashList(0);
    std::cout << std::endl;

    // initialize a file hash list vector  
    std::vector<int> file = { 1, 2, 3, 15, 16 };
    hcl.setBlockHashList(file, HASHFILE);

    // TODO initialize a block group hash list vector
    // maybe make the parameters a double or float
    // ie 1.1 is files
    // 1.2 is blocks groups hash 1
    // 1.3 is blocks hash list
    // 1.4 is everything with hash signature 1
    std::vector<int> blockgroup = { 16, 17, 20, 30 };
    hcl.setBlockHashList(blockgroup, HASHBLOCKGROUP);

    // initialize a block hash list bector
    std::vector<int> block = { 1, 2, 3, 15, 16, 17 };
    hcl.setBlockHashList(block, HASHBLOCK);

    hcl.displayHLvectors();
    std::cout << std::endl;

    // set the byteblock hashlist
    unsigned char byteblock [] = { 139, 176, 100, 82, 220, 98, 148, 121, 155, 202, 74 };
    hcl.setBlockHashList(byteblock, 11);

    // calculate the hash block size
    std::cout << "Hash file block size " << hcl.calcBlockSize(HASHFILE) << std::endl;
    std::cout << "Hash block group size " << hcl.calcBlockSize(HASHBLOCKGROUP) << std::endl;
    std::cout << "Hash block size " << hcl.calcBlockSize(HASHBLOCK) << std::endl;

    // display the block hash list
    std::cout << hcl.displayHLhashes() << std::endl;
}
