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
    // bitstream variables
    int bitformat                   = 7;
    int bitsize                     = 7; // the modexponent bitstream bit size
    int minExponent                 = 255;
    int minExponentMinusLastBlock   = 255;
    int maxExponent                 = 0;
    int diffExponent                = 0;
    uint8_t *modExpBlock;
    long modBitBlockSize            = 0;
    long modByteBlockSize           = 0;
    // initialize the gmp bigint variables
    int byteorder                   = 0;
    int endian                      = 0;
    size_t count;
    mpz_t byteblockInt, modulusInt, remainder;
public:
    BitstreamReader *bsr;
    BitstreamWriter *bsw;

    // initialize the mdBitstream object
    mdBitstream() {
    }

    // initialize the mdBitstream Object with the blocksize and blockcount and last block size
    mdBitstream(long blockSize, long blockCount, long blockRemainder) {
        blocksize      = blockSize;
        blockcount     = blockCount;
        blockremainder = blockRemainder;

        bsr = new BitstreamReader(modExpBlock, modByteBlockSize);
    }


    // Destructor
    ~mdBitstream() {
        // std::cout << "Destroying mdBitstream" << std::endl;
        // delete the modulus exponent bitstream byte block
        if (modByteBlockSize > 0) delete modExpBlock;	
        // delete the bitstream readers and writers;
        // if (modByteBlockSize > 0) 
        delete bsr;
        // the writer is not currently used
        // delete bsw;
        // std::cout << "Destroyed" << std::endl;
    } 

    // initialize the mdBitstream
    // this object method allows the object to be created and then initialized later
    void mdInitBitstream(long blockSize, long blockCount, long blockRemainder) {
        blocksize      = blockSize;
        blockcount     = blockCount;
        blockremainder = blockRemainder;

        bsr = new BitstreamReader(modExpBlock, modByteBlockSize);
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

    // set the bitsteam format based on the exponent diff
    // currently from 2 to 7
    // I think there can be a 0 for just one modexponent value for all the blocks
    // and 1 bit for just 2 values + the modexponent base
    void setDiffFormat() {
        bitformat = 7;
        if (diffExponent <= 3) bitformat = 2;
        if ((diffExponent > 3) &&  (diffExponent <= 7))  bitformat = 3;
        if ((diffExponent > 8) &&  (diffExponent <= 15)) bitformat = 4;
        if ((diffExponent > 16) && (diffExponent <= 31)) bitformat = 5;
        if ((diffExponent > 32) && (diffExponent <= 63)) bitformat = 6;

    }

    // set the bitformat
    void setFormat(long bitFormat) {
        bitformat = bitFormat;
    }    

    // get the format
    int getFormat() {
        return bitformat;
    }

    // calculate modulus exponent bitstream bit size
    long setBitstreamBitSize() {

        bitsize = 7;
        switch (bitformat) {
            case 2:
                bitsize = 2;
                break;
            case 3:
                bitsize = 3;
                break;
            case 4:
                bitsize = 4;
                break;
            case 5:
                bitsize = 5;
                break;
            case 6:
                bitsize = 6;
                break;    
        }    

        return bitsize;
    }    

    // get the modulus exponent bitstream bit size
    long getBitstreamBitSize() {
        return bitsize;
    }    

    // calculate modulus exponent bitstream block size
    void calcBitstreamSize() {

        modBitBlockSize = 0;
        modByteBlockSize = 0;

        modBitBlockSize = blockcount * bitformat;

        // add 7 bits for the minimum modulus exponent
        // add 7 bits for the last byte block 
        // TODO change this to skip the first block if the block size is 1 it duplicates it in this instance
        if (bitformat < 7) modBitBlockSize = modBitBlockSize + 14;

        // modByteBlockSize = (modBitBlockSize / 8);
        modByteBlockSize = (modBitBlockSize / 8);
        // need to round up one for blocks with a decimal result
        if ((modBitBlockSize % 8) > 0) modByteBlockSize = modByteBlockSize + 1;

        /* cout << "set format " << bitformat << '\n';
        cout << "set block count " << blockcount << '\n';
        cout << "set bit size " << modBitBlockSize << '\n';
        cout << "set byte size " << modByteBlockSize << '\n';
        */
    }   

    long getBitBlockSize() {
        return modBitBlockSize;
    } 

    long getByteBlockSize() {
        return modByteBlockSize;
    }

    // return the modExpBlock
    uint8_t *getmodExpBlock() {
        return modExpBlock;
    }    

    // test get function for the byte reader
    uint16_t getBits(int size) {
        long bits = 0;
/*
        if (size == 2) bits = bsr->get<2>();
        if (size == 3) bits = bsr->get<3>();
        if (size == 4) bits = bsr->get<4>();
        if (size == 5) bits = bsr->get<5>();
        if (size == 6) bits = bsr->get<6>();
        if (size == 7) bits = bsr->get<7>();
*/
       // std::cout << "1111111111111111111111111111111\n";
        // BitstreamReader *bsr = new BitstreamReader(modExpBlock, modByteBlockSize);
        // uint16_t bits = bsr->get<7>();
        // uint16_t bits = bsr->get<6>();
       // std::cout << "Test " << bsr->size() << "\n";
        // bs.write(0,  7,  5);
        // bits = bsr->getWithOffset<10>(6);
        // std::cout << "1111111111111111111111111111111\n";
        return bits;

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

    // calculate the minimum and maximum exponent for a filestream
    // and compute the difference skip the last byte block
    // diff 0   = 0 bits -- TODO
    // diff 1/2 = 1 bit  -- TODO
    // diff 3   = 2 bits
    // diff 7   = 3 bits 
    // diff 15  = 4 bits
    // diff 31  = 5 bits
    // diff 63  = 6 bits
    // diff 63+ = 7 bits
    void calcMinMaxExponent(std::ifstream &nf) {

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
                // mpz_class modint(byteblockInt);
        
                /* std::cout << "Block " << blocknumber << " ";
                std::cout << currentblocksize << "/" << blocksize;
                std::cout << " modexponent " << modexponent;
                // std::cout << " modint " << modint << std::endl;
                printByteblock(byteblock, currentblocksize, true);
                 std::cout << "\n";
                */ 
       
                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        // diffExponent = maxExponent - minExponent;
        diffExponent = maxExponent - minExponentMinusLastBlock;
        //std::cout << "exponent max " << diffExponent << std::endl;
        // std::cout << "exponent max " << maxExponent - minExponent << std::endl;

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

        // set the file position to beginning
        nf.seekg (0, nf.beg);

        // calculate modulus exponent bit block size
        setDiffFormat();
        setBitstreamBitSize();
        // calcBitstreamSize();

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
        // setFormat();
        // setBitstreamBitSize();
        calcBitstreamSize();

        // Create bitStream object
        modExpBlock = new uint8_t[modByteBlockSize];
        BitstreamWriter bsw(modExpBlock, modByteBlockSize);
        // bsw.put<bitsize>(minExponent);
        // int const TWO = 2;
        // bsw.put<7>(minExponent);
        bsw.put<7>(minExponentMinusLastBlock);


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
 
                // write the byte block mod exponent
                if (blocknumber < blockcount) {
                    // modexponent = modexponent - minExponent;
                    modexponent = modexponent - minExponentMinusLastBlock; 

                    // check the bitformat and write the modexponent to the bitstreamwriter
                    // the template needs a constant int for the bitsize
                    switch (bitformat) {
                        case 2:
                            bsw.put<2>(modexponent);
                            break;
                        case 3:
                            bsw.put<3>(modexponent);
                            break;
                        case 4:
                            bsw.put<4>(modexponent);
                            break;
                        case 5:
                            bsw.put<5>(modexponent);
                            break;
                        case 6:
                            bsw.put<6>(modexponent); 
                            break;    
                    }    
                // write the last byte block mod exponent
                } else {
                    bsw.put<7>(modexponent);
                }   
                // std::cout << "block number " << blocknumber << " bitformat " << bitformat << " bitsize " << bitsize << " modexponent " << modexponent << " " << modexponent + minExponentMinusLastBlock << std::endl;

                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

        // set the file position to beginning
        nf.seekg (0, nf.beg);

        // create the bitstream reader
        // delete the existing bitstream reader and create a new object
        delete bsr;
        bsr = new BitstreamReader(modExpBlock, modByteBlockSize);

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
        // modBitBlockSize = blockcount * 7;
        // modByteBlockSize = (modBitBlockSize / 8);
        // need to round up one for blocks with a decimal result
        // if ((modBitBlockSize % 8) > 0) modByteBlockSize += 1;
        calcBitstreamSize();

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

                // if this is the last block stop processing 
                if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

        /* free used memory */
        mpz_clear(byteblockInt);

        // set the file position to beginning
        nf.seekg (0, nf.beg);

        // create the bitstream reader
        // delete the existing bitstream reader and create a new object
        delete bsr;
        bsr = new BitstreamReader(modExpBlock, modByteBlockSize);

    }

    // load the 7 bit bitstream
    // TODO
    void loadMdzipBitstream(std::ifstream &nf) {

        // Create bitStream object
        // Create the modulus exponent bitstream buffer
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        // load the modulus exponent bitstream block from the file
        nf.read(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);

        // initialize the bitstreamreader
        ///// bsr = BitstreamReader bsr(modExpBlock, modByteBlockSize);

    }    

    // write the 7 bit bitstream
    void writeBitstream(std::ofstream &wf) {
        wf.write(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
    }  


};    