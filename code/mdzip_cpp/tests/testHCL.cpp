#include <string>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <openssl/sha.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "mdCore/mdCommon.h"
#include "../mdCore/mdHashContextListNew.h"
#include "mdCore/mdBitstream.h"

void testBitstream();
void testDisplayHCL();
void testFileHCL();
void testBlockHCL();
void testHashStringHCL();

int main()
{
    testBitstream();
    testDisplayHCL();
    testFileHCL();
    testBlockHCL();
    testHashStringHCL();   
}

void testBitstream() {
   // blocksize and blockcount and blockremainder
    std::string filename = "Makefile";
    std::ifstream nf(filename, std::ios::in | std::ios::binary);
    // mdBitstream mb(11,421, 3);
    mdBitstream mb(10,462, 3);
    mb.calcMinMaxExponent(nf);
    // mb.mdzipModExponentBitstream(nf); // not working except on last block
    // seek to beginning of file
    // nf.seekg (0, nf.beg);
    // mb.testFile(nf); // I think there is an issue with ifstream and file in sub directory

    std::cout << "Min Exponent " << mb.getMinSize() << std::endl;
    std::cout << "Max Exponent " << mb.getMaxSize() << std::endl;
    std::cout << "Diff Min/Max Exponent " << mb.getDiff() << std::endl;
    std::cout << "Last Block Exponent " << mb.getMinSizeMinusLast() << std::endl;

    nf.close();

}

void testDisplayHCL() {

    // initialize the hash list object
    mdHashContextList hcl;

    // display the current hash signature list
    hcl.displayHLRegistry(0);
    std::cout << std::endl;

    std::cout << "Hash length " << hcl.getRegistrySize() << std::endl;


}    

void testFileHCL() {
    mdHashContextList hclfile(HASHFILE);

    // initialize a file hash list vector  
    std::vector<int> file = { 1, 2, 3, 15, 16 };
    hclfile.setVectorHL(file);

    // display the file hash list input vectors
    hclfile.displayHLvectors();
    std::cout << std::endl;

    // return the vector lengths
    std::cout << "Hash file block size "  << hclfile.getHLVectorLength() << std::endl;

    // calculate the hash block size
    std::cout << "Hash file block size "  << hclfile.calcBlockSize() << std::endl;
    std::cout << "Hash file key size "    << hclfile.calcBlockKeySize() << std::endl;
    std::cout << std::endl;

    assert(hclfile.getHLVectorLength()  == 5);
    assert(hclfile.calcBlockSize()      == 52);
    assert(hclfile.calcBlockKeySize()   == 20);

}


void testBlockHCL() {
    mdHashContextList hclblock;

    // initialize a block hash list vector
    std::vector<int> block = { 1, 2, 3, 15, 16, 17, 34 };
    hclblock.setVectorHL(block);

    // display the block hash list input vectors
    hclblock.displayHLvectors();
    std::cout << std::endl;

    // set the byteblock hashlist
    unsigned char byteblock [] = { 139, 176, 100, 82, 220, 98, 148, 121, 155, 202, 74 };
    hclblock.setByteBlockHashList(byteblock, 11);

    // return the vector lengths
    std::cout << "Hash block size "       << hclblock.getHLVectorLength() << std::endl;

    // calculate the hash block size
    std::cout << "Hash block size "       << hclblock.calcBlockSize() << std::endl;
    std::cout << "Hash block key size "   << hclblock.calcBlockKeySize() << std::endl;
    std::cout << std::endl;

    // display the block hash list
    std::cout << hclblock.displayHLhashes() << std::endl;

    assert(hclblock.getHLVectorLength()  == 7);
    assert(hclblock.calcBlockSize()      == 80);
    assert(hclblock.calcBlockKeySize()   == 20);

}

void testHashStringHCL() {

    // initialize the hash list string object
    // this uses a hash string input and converts it to the hash list vector tuple
    mdHashContextList hclstr;
    std::string hclstring = "cit64:crc32:fast32:fast64:fnv32a:fnv64:md2:md4:met641:met642:sha512:whp:zz12:";
    std::cout << std::endl << "Initializing hclstring " << hclstring << std::endl;

    hclstr.setVectorHLstring(hclstring);
    hclstr.displayHLvectors();
    std::cout << std::endl;

    assert(hclstr.getHLVectorLength()  == 12);
    assert(hclstr.calcBlockSize()      == 212);
    assert(hclstr.calcBlockKeySize()   == 32);
    
}