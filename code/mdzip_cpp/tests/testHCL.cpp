#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/sha.h>
#include <sstream>
#include "../mdCore/mdHashContextList.h"

int main()
{
    // initialize the hash list object
    mdHashContextList hcl;

    // display the current hash signature list
    hcl.displayHLRegistry(0);
    std::cout << std::endl;

    // initialize a file hash list vector  
    std::vector<int> file = { 1, 2, 3, 15, 16 };
    hcl.setVectorHL(file, HASHFILE);

    // TODO initialize a block group hash list vector
    // maybe make the parameters a double or float
    // ie 1.1 is files
    // 1.2 is blocks groups hash 1
    // 1.3 is blocks hash list
    // 1.4 is everything with hash signature 1
    std::vector<int> blockgroup = { 16, 17, 20, 30 };
    hcl.setVectorHL(blockgroup, HASHBLOCKGROUP);

    // initialize a block hash list vector
    std::vector<int> block = { 1, 2, 3, 15, 16, 17, 34 };
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
    std::cout << "Hash block key size "   << hcl.calcBlockKeySize(HASHBLOCK) << std::endl;
    std::cout << std::endl;

    // display the block hash list
    std::cout << hcl.displayHLhashes() << std::endl;

    // initialize the hash list string object
    // this uses a hash string input and converts it to the hash list vector tuple
    mdHashContextList hclstr;
    std::string hclstring = "cit64:crc32:fast32:fast64:fnv32a:fnv64:md2:md4:met641:met642:sha512:whp:zz12:";
    std::cout << std::endl << "Initializing hclstring " << hclstring << std::endl;

    hclstr.setVectorHLstring(hclstring, HASHBLOCK);
    hclstr.displayHLvectors();
    std::cout << std::endl;

}
