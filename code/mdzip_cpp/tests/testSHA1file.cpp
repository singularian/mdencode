#include <cstring>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/sha.h>
#include <sstream>
// #include "../mdHashContextList.h"
#include "../filehash.h"

void printByteblock(unsigned char *byteblock, long blocksize, bool ishex);

int main()
{

    std::string filename = "file.txt";
    uint64_t city64i = 0;
    city64i = calculateCityhash((char *) filename.c_str(), 11111222);
    std::cout << "cityhash64 file " << city64i << std::endl;

    uint8_t sha1i[21];
    char file[100] = "file.txt";
    calculateSHA1(file, sha1i);
    printByteblock(sha1i, 20, true);

    calculateSHA1((char *) filename.c_str(), sha1i);
    printByteblock(sha1i, 20, true);
    

    // initialize the hash list object
//    mdHashContextList hcl;

/*    // display the current hash signature list
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

}

