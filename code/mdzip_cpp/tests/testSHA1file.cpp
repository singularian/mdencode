#include <cstring>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <sstream>
#include "../mdHashContextList.h"
// #include "../filehash.h"

void printByteblock(unsigned char *byteblock, long blocksize, bool ishex);

int main()
{

    std::string filename = "file.txt";
    uint64_t city64i = 0;
    city64i = calculateCityhashFile((char *) filename.c_str(), 11111222);
    std::cout << "cityhash64 file " << city64i << std::endl;

    uint8_t ripe160i[21];
    calculateRipe160((char *) filename.c_str(), ripe160i);
    std::cout << "openssl ripe160" << std::endl;
    printByteblock(ripe160i, 20, true);

    uint8_t sha1i[21];
    char filenm[100] = "file.txt";
    // calculateSHA1(filenm, sha1i);
    // std::cout << "openssl sha1" << std::endl; 
    // printByteblock(sha1i, 20, true);

    calculateSHA1((char *) filename.c_str(), sha1i);
    std::cout << "openssl sha1" << std::endl;
    printByteblock(sha1i, 20, true);
    

    // initialize the hash list object
    mdHashContextList hclfile;

    // display the current hash signature list
    // hclfile.displayHLRegistry(0);
    //std::cout << std::endl;

    // initialize a file hash list vector  
    std::vector<int> file = { 1, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 26 };
    // hclfile.setVectorHL(file, HASHFILE);
    hclfile.setVectorHL(file, HASHBLOCK);
    hclfile.setFileHashList(filename); // set the file hash list variables
    
    // display the block hash list
    std::cout << hclfile.displayHLhashes() << std::endl;
     
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

