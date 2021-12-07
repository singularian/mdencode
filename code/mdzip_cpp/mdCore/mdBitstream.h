#include "external/bitstream/Bitstream.h"
#include <gmp.h>
#include <gmpxx.h>

class mdBitstream 
{
private:
    long blocksize                  = 0;
    long blockcount                 = 0;
    long blockremainder             = 0; // the last file block size
    int fileListSize                = 0;
    int blockListSize               = 0;
    int bitformat                   = 7;
    int bitsize                     = 7; // the modexponent bitstream bit size
    int minExponent                 = 255;
    int minExponentMinusLastBlock   = 255;
    int maxExponent                 = 0;
    int diffExponent                = 0;
    uint8_t *modExpBlock;
    long modBitBlockSize            = 0;
    long modByteBlockSize           = 0;
    BitstreamReader *bsr;
    BitstreamWriter *bsw;
    // initialize the gmp bigint variables
    int byteorder                   = 0;
    int endian                      = 0;
    size_t count;
    mpz_t byteblockInt, modulusInt, remainder;
public:
    int psize;

    // initialize the mdBitstream object
    mdBitstream() {
    }

    mdBitstream(long blockSize, long blockCount, long blockRemainder) {
        blocksize      = blockSize;
        blockcount     = blockCount;
        blockremainder = blockRemainder;

    }

    // Destructor
    ~mdBitstream() {
        std::cout << "Destroying mdBitstream" << std::endl;
        delete modExpBlock;	
    } 

    // get the min modexponent size
    long getMinSize() {
        return minExponent;
    }

    // get the min modexponent size without the last byte block if it is not the same size???
    long getMinSizeMinusLast() {
        return minExponentMinusLastBlock;
    }

    // get the max modexponent size
    long getMaxSize() {
        return maxExponent;
    }

    // get the max modexponent size
    long getDiff() {
        return diffExponent;
    }

    // get the format
    int getFormat() {
        return bitformat;
    }

    // set the bitsteam format
    // currently from 2 to 7
    void setFormat() {
        bitformat = 7;
        if (diffExponent <= 3) bitformat = 2;
        if ((diffExponent > 3) &&  (diffExponent <= 7))  bitformat = 3;
        if ((diffExponent > 8) &&  (diffExponent <= 15)) bitformat = 4;
        if ((diffExponent > 16) && (diffExponent <= 31)) bitformat = 5;
        if ((diffExponent > 32) && (diffExponent <= 63)) bitformat = 6;

    }

    // calculate modulus exponent bitstream bit size
    long setBitstreamBitSize() {

        bitsize = 7;
        switch (bitformat) {
            case 2:
                bitsize = 2;
            case 3:
                bitsize = 3;
            case 4:
                bitsize = 4;
            case 5:
                bitsize = 5;
            case 6:
                bitsize = 6;    
        }    

        return bitsize;
    }    

    // calculate modulus exponent bitstream block size
    void calcBitstreamSize() {

        switch (bitformat) {
            case 2:
                modBitBlockSize = blockcount * 2;
                // could also have a 3 bits for the bit size 2 - 7
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the minimum modulus exponent
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the last byte block
                modByteBlockSize = (modBitBlockSize / 8);
            case 3:
                modBitBlockSize = blockcount * 3;
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the minimum modulus exponent
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the last byte block
                modByteBlockSize = (modBitBlockSize / 8);  
            case 4:
                modBitBlockSize = blockcount * 4;
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the minimum modulus exponent
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the last byte block
                modByteBlockSize = (modBitBlockSize / 8);   
            case 5:
                modBitBlockSize = blockcount * 5;
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the minimum modulus exponent
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the last byte block
                modByteBlockSize = (modBitBlockSize / 8);     
            case 6:
                modBitBlockSize = blockcount * 6;
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the minimum modulus exponent
                modBitBlockSize = modBitBlockSize + 7; // add 7 bits for the last byte block
                modByteBlockSize = (modBitBlockSize / 8);
            default:
                long modBitBlockSize = blockcount * 7;
                long modByteBlockSize = (modBitBlockSize / 8); // need to round up one for blocks with a decimal result
        }

        if ((modBitBlockSize % 8) > 0) modByteBlockSize += 1;
    }    

    // return the modExpBlock
    uint8_t *getmodExpBlock() {
        return modExpBlock;
    }    

    void testFileBlock(std::string filename) {
        std::ifstream nf(filename, std::ios::in | std::ios::binary);

        testFile(nf);

        nf.close();
    }    

    // test file
    void testFile(std::ifstream &nf) {

        long blocknumber = 1;

        // the curent block size
        long currentblocksize = blocksize;

        // initialize the gmp bigint variables
        size_t count;

        // create the byteblock buffer with the blocksize  
        unsigned char *byteblock  = new unsigned char[blocksize];

        // set last block  
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

                // read the current byteblock from the input file and generate a modulus exponent
                nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);


                std::cout << "Block " << blocknumber << " ";
                std::cout << currentblocksize << "/" << blocksize << " ";
                printByteblock(byteblock, currentblocksize, true);
       
                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

    }

    // calculate the minimum and maximum exponent
    // diff 3   = 2 bits
    // diff 7   = 3 bits 
    // diff 15  = 4 bits
    // diff 31  = 5 bits
    // diff 63  = 6 bits
    // diff 63+ = 7 bits
    void calcMinMaxExponent(std::ifstream &nf) {

        long blocknumber = 1;
        // int modexponent  = 0;

        // the curent block size
        long currentblocksize = blocksize;

        // initialize the gmp bigint variables
        size_t count;

        // create the byteblock buffer with the blocksize  
        unsigned char *byteblock  = new unsigned char[blocksize];

        // set last block  
        long lastblk = blockcount;

        // initialize the byteblockint
        mpz_init_set_str(byteblockInt, "0", 10);
        // mpz_class modint(byteblockInt);


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

                // read the current byteblock from the input file and generate a modulus exponent
                nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);

                // create a bigint number for the byte block
                mpz_import (byteblockInt, currentblocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

                // calculate the modulus exponent with base two 
                int modexponent = calcExponent(byteblockInt);
                if (modexponent < minExponent) minExponent = modexponent;
                if ((modexponent < minExponentMinusLastBlock) && (blocknumber < blockcount)) minExponentMinusLastBlock = modexponent;
                if (modexponent > maxExponent) maxExponent = modexponent;

                // display the modulus remainder
                mpz_class modint(byteblockInt);
        

                std::cout << "Block " << blocknumber << " ";
                std::cout << currentblocksize << "/" << blocksize;
                std::cout << " modexponent " << modexponent;
                std::cout << " modint " << modint << std::endl;
                printByteblock(byteblock, currentblocksize, true);
                 std::cout << "\n";
       
                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        // diffExponent = maxExponent - minExponent;
        diffExponent = maxExponent - minExponentMinusLastBlock;

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

    }

    // converts the modulus exponent to a 2 to 6 bitstream
    void mdzipModExponentBitstream26(std::ifstream &nf) {

        long blocknumber = 1;

        // the curent block size
        long currentblocksize = blocksize;

        // initialize the gmp bigint variables
        size_t count;

        // create the byteblock buffer with the blocksize  
        unsigned char *byteblock  = new unsigned char[blocksize];

        // set last block  
        long lastblk = blockcount;

        // initialize the byteblockint
        mpz_init_set_str(byteblockInt, "0", 10);

        // calculate modulus exponent bit block size
        setFormat();
        setBitstreamBitSize();
        calcBitstreamSize();

        // Create bitStream object
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        // std::cout << "block size " << sizeof(modExpBlock) << " " << modByteBlockSize << std::endl;
        BitstreamWriter bsw(modExpBlock, modByteBlockSize);
        bsw.put<bitsize>(minExponent);

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

                // read the current byteblock from the input file and generate a modulus exponent
                nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);

                // create a bigint number for the byte block
                mpz_import (byteblockInt, currentblocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

                // calculate the modulus exponent with base two 
                int modexponent = calcExponent(byteblockInt);
                if (blocknumber < blockcount) {
                    modexponent = modexponent - minExponent;
                    bsw.put<bitsize>(modexponent);
                } else {
                    bsw.put<bitsize>(modexponent);
                }   
                std::cout << "modexponent " << modexponent << std::endl;

                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

    }

    // converts the modulus exponent to a 7 bitstream
    void mdzipModExponentBitstream(std::ifstream &nf) {

        long blocknumber = 1;

        // the curent block size
        long currentblocksize = blocksize;

        // initialize the gmp bigint variables
        size_t count;

        // create the byteblock buffer with the blocksize  
        unsigned char *byteblock  = new unsigned char[blocksize];

        // set last block  
        long lastblk = blockcount;

        // initialize the byteblockint
        mpz_init_set_str(byteblockInt, "0", 10);

        // calculate modulus exponent bit block size
        modBitBlockSize = blockcount * 7;
        modByteBlockSize = (modBitBlockSize / 8);
        // need to round up one for blocks with a decimal result
        if ((modBitBlockSize % 8) > 0) modByteBlockSize += 1;

        // Create bitStream object
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        // std::cout << "block size " << sizeof(modExpBlock) << " " << modByteBlockSize << std::endl;
        BitstreamWriter bsw(modExpBlock, modByteBlockSize);

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

                // read the current byteblock from the input file and generate a modulus exponent
                nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);

                // create a bigint number for the byte block
                mpz_import (byteblockInt, currentblocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

                // calculate the modulus exponent with base two 
                int modexponent = calcExponent(byteblockInt);
                // add the modulus exponent to the bitstreamwriter as a 7 bit number
                // this could be optimized to use RLE or 4 or 5 or 6 bits
                // 7 bits will encode all the possible combinations of 2^80 which is the exponent max in a 10 byte block 
                bsw.put<7>(modexponent);
                std::cout << "modexponent " << modexponent << std::endl;

                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

    }

    // load the 7 bit bitstream
    void loadMdzipBitstream(std::ifstream &nf) {
        modBitBlockSize = blockcount * 7;
        modByteBlockSize = (modBitBlockSize / 8); // need to round up one for blocks a decimal result
        if ((modBitBlockSize % 8) > 0) modByteBlockSize += 1;

        // Create bitStream object
        // Create the modulus exponent bitstream buffer
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        // load the modulus exponent bitstream block from the file
        nf.read(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
        // initialize the bitstreamreader

/////        bsr = BitstreamReader bsr(modExpBlock, modByteBlockSize);

    }    

    // write the 7 bit bitstream
    void writeBitstream(std::ofstream &wf) {
        wf.write(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
    }  


};    