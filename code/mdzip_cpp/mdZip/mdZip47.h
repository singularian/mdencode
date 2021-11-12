/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdunzip Program
 * 
 * mdZip47.h zips a file with a mdzip format
 * This currently uses sip40 with a 32 bit modulus and a 7 bit bitstream for the modulus exponent
 *
 * 
 * 
*/
#include <iostream>
#include <sys/stat.h>
#include <gmp.h>
#include <gmpxx.h>
#include "mdCore/mdCommon.h"
#include "mdCore/mdHashContextList.h"
#include "external/bitstream/Bitstream.h"


class mdZip47 
{
private:
    int threadcount       = 8;
    // file variables
    std::string filename;
    size_t inputfilesize  = 0;
    long filesize         = 0;
    // mdzip variables
    std::string mdzipExtension = ".mdz";
    double mdversion      = 1.01;
    long blocksize        = 10;
    long blockcount       = 0;
    long blockremainder   = 0; // the last file block size
    long blocknumber      = 1; // current file block number position
    int modsizebits       = 32;
    int modsizeBytes      = 4;
    int modexponent       = 0;
    long modByteBlockSize = 0; // mdzip bitstream modulus exponent byte size
    // hash context list variables
    int hclfileblocksize  = 0; // hash file string size
    int hclblockblocksize = 0; // hash block string size
    std::string filehashnames;   
    std::string blockhashnames;
    std::vector<int> fhlist;
    std::vector<int> bhlist;
    std::string filesigs;
    std::string blockkeys = "default";
    // file and block hash list data sizes and total size
    std::string filehashvector;
    std::string blockhashvector;
    // boolean variables
    bool randombh         = false;
    bool inc              = false;  
    bool dec              = false;
    // initialize the gmp bigint variables
    int byteorder         = 0;
    int endian            = 0;
    size_t count;
    mpz_t byteblockInt, modulusInt, remainder;
    // log variables
    bool runlogging       = false;
    // mdMutexLog log{false};
 public:    

    // initialize the mdzip object
    mdZip47() {
    }

    mdZip47(std::string fileName, long blockSize, int modBitSize, std::vector<int> &fhList, std::vector<int> &bhList, bool randomBH, bool Inc, bool Dec, bool runLogging) {
        filename    = fileName;
        blocksize   = blockSize;
        modsizebits = modBitSize;
        fhlist      = fhList;
        bhlist      = bhList;
        randombh    = randomBH;
        inc         = Inc;
        dec         = Dec;
        runlogging  = runLogging;

        setModulus();

        //  log.setLogging(runlogging);

    }


    // Destructor
    ~mdZip47() {
    } 
    

    void readZipFile(std::ifstream &nf) {
        // nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
        // nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
        // nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
        // nf.read(reinterpret_cast<char*>(&modsize),   sizeof(int));
    }

    // set the version
    void setVersion(double version) {
        mdversion = version;
    }

    // set the mdzip filename
    void setFilename(std::string fileName) {
        filename = fileName;
    }

    // get the version
    double getVersion() {
        return mdversion;
    }

    // set the block size
    void setBlockSize(long bsize) {
        blocksize = bsize;
    }

    // get the file block size
    long getBlockSize() {
        return blocksize;
    }

    // set the file block count
    void setBlockCount(long count) {
        blockcount = count;
    }

    // get the file block count
    long getBlockCount() {
        return blockcount;
    }

    // set the file block last block size remainder
    void setLastBlockSize() {

        long lastblocksize = filesize % blocksize;

        if (lastblocksize == 0)
        {
            lastblocksize = blocksize;
        }

        blockremainder = lastblocksize;
    }

    // get the file block last block size remainer
    long getLastBlockSize() {
        return blockremainder;
    }    

    // set the modulus byte size
    void setModulus() {

        if (modsizebits == 0) return;
 
        if ((modsizebits % 8) == 0)
        {
            modsizeBytes = modsizebits / 8;
        }
        else if (modsizebits < 8)
        {
            modsizeBytes = 1;
        }
        else
        {
            modsizeBytes = (modsizebits / 8) + 1;
        }
    }

    // get the modulus bit size
    long getModulusBitsSize() {
        return modsizebits;
    }

    // get the modulus bit size
    long getModulusBytesSize() {
        return modsizebits;
    }

    // set the modulus exponent size
    void setModulusExponent(long exponent) {
        modexponent = exponent;
    }

    // get the modulus exponent size
    long getModulusExponent() {
        return modexponent;
    }

    // modulus size
    void incBlockNumber() {
        blocknumber++;
    }

    // get the current file block number
    long getBlockNumber() {
        return blocknumber;
    }

    // mdzipsip47 an input file
    // current mdzip extension is .mdz
    // this is currently little endian and 64 bit for the longs
    // 
    // 5 bytes sip40
    // 4 bytes 32-bit modulus
    // 3-7 bits for the modulus exponent (2^80 max or 80 max)
    // 75-79 bits to encode an 80 bit file block
    // 9.7/10 encoding
    int mdzipfile() {

        // check if filename is defined
        // might readd cli11 checks it

        filesize = GetFileSize(filename);
        // need to add the mdformat

        if (filesize < blocksize) blocksize = filesize;
        if (filesize == 0) return 0;

        // calculate the file block count and last block size
        blockcount = CalcFileBlocks(filesize, blocksize);

        // calculate the last block size
        // long blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);
        setLastBlockSize();

        // the curent block size
        long currentblocksize = blocksize;

        // set the mdzip filename with the extension
        std::string mdzipfile = filename + mdzipExtension;

        const char *fname = mdzipfile.c_str();
        std::remove(fname);

        // set the file hash list
        mdHashContextList hclfile;
        hclfile.setVectorHL(fhlist, HASHBLOCK);
        hclfile.setFileHashList(filename);

        // create the ifstream and ofstream objects
        std::ifstream nf(filename, std::ios::in | std::ios::binary);
        std::ofstream mdzip(mdzipfile, std::ios::out | std::ios::binary);  
        if(!mdzip) {
            std::cout << "Cannot open mdzip file!" << std::endl;
            return 1;
        }

        // initialize the gmp bigint variables
        //size_t count;
        //mpz_t byteblockInt, modulusInt, remainder;
        mpz_init_set_str(byteblockInt, "0", 10);
        mpz_init_set_str(modulusInt, "1", 10);
        mpz_init_set_str(remainder, "0", 10);

        // calculate the modulus bigint as 2 ^ modsize - 1
        calcModulusInt(modulusInt, modsizebits);

        // if the increment or decrement is set change the mdVersion 
        // bool incrementKey = false;
        int incKey = NOINC;
        if (dec) { mdversion = 1.11; incKey = DEC; };
        if (inc) { mdversion = 1.12; incKey = INC; };
        
        // need to make sure these are byte order independent
        // the block size header is currently 28 bytes
        mdzip.write(reinterpret_cast<char*>(&mdversion),    sizeof(double));
        mdzip.write(reinterpret_cast<char*>(&filesize),     sizeof(long));
        mdzip.write(reinterpret_cast<char*>(&blocksize),    sizeof(blocksize));
        mdzip.write(reinterpret_cast<char*>(&modsizebits),  sizeof(int));

        // initialize the block hash context list
        mdHashContextList hclblock;
        // set the block hash list with the block hash list vector 
        hclblock.setVectorHL(bhlist, HASHBLOCK);
        // display the block hash list vector
        std::string vectorlist = hclblock.getHLvectorsString(HASHBLOCK);

        // write the hash list strings
        // TODO need to write the keylist as well
        // ie aes8:sha1:md5-aes8-keylist
        int hclsize;
        filehashnames  = hclfile.getHLvectorsStringNames(HASHBLOCK);
        blockhashnames = hclblock.getHLvectorsStringNames(HASHBLOCK);

        // write the file hash list names
        hclsize = filehashnames.size();
        mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
        // wf.write(reinterpret_cast<char*>(&filehashnames),   hclsize);
        mdzip.write(filehashnames.c_str(),   hclsize);

        // write the block hash list names string 
        hclsize = blockhashnames.size();
        mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
        // wf.write(reinterpret_cast<char*>(&blockhashnames),   hclsize);
        mdzip.write(blockhashnames.c_str(),   hclsize);

        // write the file hash list to the mdzip file
        // TODO write the file hash key random list
        hclfile.writeBlockHashList(mdzip);
        //std::cout << "file hash list " << std::endl;
        //std::cout << hclfile.displayHLhashes() << std::endl;
        filesigs = hclfile.displayHLhashes();

        // TODO need to also write the block group hash list if implemented

        // write the block hash keys list
        // if randomblock or randbh is set it uses the blockhashnames
        // TODO allow the user to specify a block hash key list 
        // For now I can make it use the same list as the blockhash list
        if (randombh == true) {
            hclsize = blockhashnames.size();
            mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int));
            // std::cout << "bhash " <<  blockhashnames << std::endl;
            // wf.write(blockhashnames.c_str(),   hclsize); 
            hclblock.randomizeKeyList();
            hclblock.writeKeyList(mdzip);
            blockkeys = hclblock.displayHLhashKeys();
        } else {
            // write the hash size as a zero byte
            hclsize = 0;
            mdzip.write(reinterpret_cast<char*>(&hclsize),   sizeof(int)); 
        }   
    
        // create the byteblock buffer with the blocksize 
        unsigned char *byteblock  = new unsigned char[blocksize];

        // calculate the correct modulus byte size in case of a odd modulus size 33
        // the modulus parameter is in bits and this converts it to bytes 
        // int modsizeBytes = calcModulusBytes(modsize); // TODO remove
        unsigned char *modulusint = new unsigned char[modsizeBytes];

        // calculate the file and file block hash list size
        hclfileblocksize  = hclfile.calcBlockSize(HASHBLOCK);
        hclblockblocksize = hclblock.calcBlockSize(HASHBLOCK);

        // set the file hash list parameters and hash block size
        filehashvector = hclfile.getHLvectorsString(HASHBLOCK);

        // TODO set the hashblockgroup 
        //  std::string bghashvector = getHLvectorsString(HASHBLOCKGROUP);

        // set the file block hash list
        blockhashvector = hclblock.getHLvectorsString(HASHBLOCK);

        // display the mdzip file info 
        displayInfo(true);

        // set last block  
        long lastblk = blockcount;

        // create the 7 bit modulus exponent bitstream block
        uint8_t *modExpBlock = mdzipModExponentBitstream(nf);
        // write the 7 bit modulus exponent bitstream to the mdzip file
        mdzip.write(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
        // seek to beginning of file
        nf.seekg (0, nf.beg);
        // initialize the bitstreamreader with the modulus exponent block and size 
        BitstreamReader bsr(modExpBlock, modByteBlockSize);
        
        blocknumber = 1;
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

            // read the current byteblock from the input file and generate a signature
            nf.read(reinterpret_cast<char*>(byteblock), (size_t) currentblocksize);

            // increment the hash context list block number 
            hclblock.incrementBlockNum(incKey);
            // set the byte block hash list
            hclblock.setByteBlockHashList((unsigned char*) byteblock, currentblocksize);
            // write the signature list to the mdzip file
            hclblock.writeBlockHashList(mdzip);

            // create a bigint number for the byte block
            mpz_import (byteblockInt, currentblocksize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

            // calculate the modulus remainder 
            // modulus remainder = byteblockInt mod modulusInt
            mpz_mod (remainder, byteblockInt, modulusInt);

            // get the modulus exponent from the bitstream reader buffer
            modexponent = bsr.get<7>();

            // export the gmp modulus int remainder to a modulus int byte block
            mpz_export(modulusint, &count, byteorder, sizeof(modulusint[0]), endian, 0, remainder);

            // if the export count is less than the byte block size
            // pad the GMP modulusint byte block 
            padBlockBytes(count, modsizeBytes, modulusint);
            
            // write the modulus int remainder byte block
            mdzip.write(reinterpret_cast<char*>(modulusint),   sizeof(char) * modsizeBytes);
            
            // display the byte block info
            // TODO Need to add the logging       
            displayBlockInfo("Zipping", byteblock, currentblocksize, blocksize, blocknumber, lastblk, modexponent, remainder, hclblock); 

            // if this is the last block stop processing 
            if (blocknumber == blockcount) break;

            }   

            blocknumber++;
        }

        delete byteblock;
        delete modulusint;
        delete modExpBlock;	

        nf.close();
        mdzip.close();

        if(!mdzip.good()) {
        cout << "Error occurred at writing time!" << endl;
        // return 1;
        }

        /* free used memory */
        mpz_clear(byteblockInt);
        mpz_clear(modulusInt);
        mpz_clear(remainder);

        return 0;

    }

    // need to clean this up
    // change the global variable for modByteBlockSize
    // converts the modulus exponent to a 7 bit stream
    uint8_t *mdzipModExponentBitstream(std::ifstream &nf) {

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

        // calculate modulus exponent bit block size
        long modBitBlockSize = blockcount * 7;
        // long modByteBlockSize = (modBitBlockSize / 8); // need to round up one for blocks a decimal result
        modByteBlockSize = (modBitBlockSize / 8); // need to round up one for blocks a decimal result
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

            // if this is the last block stop processing 
            if (blocknumber == blockcount) break;

            }
            blocknumber++;
        }

        delete byteblock;	

        return modExpBlock;
    }


    // display the mdlist mdzip file info
    void displayInfo(bool mdlist) {

        std::cout << std::left << std::setw(20) << "Filename Details: "     << filename << std::endl;

        std::cout << std::left << std::setw(20) << "Version: "              << mdversion << std::endl;
        std::cout << std::left << std::setw(20) << "Filesize: "             << filesize  << std::endl;
        std::cout << std::left << std::setw(20) << "Blocksize: "            << blocksize << std::endl;


        std::cout << std::left << std::setw(20) << "Blockcount: "           << blockcount << std::endl;
        std::cout << std::left << std::setw(20) << "Blockremainder: "       << blockremainder << std::endl;

        std::cout << std::left << std::setw(20) << "Modsize: "              << modsizebits << std::endl;
        std::cout << std::left << std::setw(20) << "Modsize Bytes: "        << modsizeBytes << std::endl;
        std::cout << std::left << std::setw(20) << "Filehashlist: "         << filehashnames << std::endl;
        std::cout << std::left << std::setw(20) << "Blockhashlist: "        << blockhashnames << std::endl;
        std::cout << std::left << std::setw(20) << "Filehashlist size: "    << filehashnames.size() << std::endl;
        std::cout << std::left << std::setw(20) << "Blockhashlist size: "   << blockhashnames.size() << std::endl;
        std::cout << std::left << std::setw(20) << "Blockhash key list: "   << blockkeys << std::endl;

        std::cout << std::left << std::setw(20) << "File Hash Bytes: "      << hclfileblocksize << std::endl;
        std::cout << std::left << std::setw(20) << "Block Hash Bytes: "     << hclblockblocksize << std::endl;
        if (mdlist == false) std::cout << std::left << std::setw(20) << "Threadcount:" << threadcount << std::endl;
        std::cout << std::left << std::setw(20) << "Platform:" << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;

        // display the file hash list parameters and hash block size
        std::cout << std::endl;
        std::cout << "File hashlist " << std::endl;
        std::cout << filehashvector   << std::endl;

        // TODO display the hashblockgroup 
        // std::cout << "File block group hash list " << std::endl;
        // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

        // display the file block hash list
        std::cout << "File block hashlist " << std::endl;
        std::cout << blockhashvector << std::endl;

        std::cout << "File Signatures " << std::endl;
        std::cout << filesigs << std::endl;
        std::cout << std::endl;
    }

    // displayBlockInfo
    //
    // display the signature block info
    //
    // action, blocknumber, currentblocksize
    // signature list
    // modulus exponent
    // modulus remainder
    //
    // Example
    // Unzipping Block 127 Bytes Size 14/14
    // fast64 11350068956456432289 
    // Modulus Exponent 65
    // Modulus Remainder 4243591440
    void displayBlockInfo(std::string action, unsigned char *byteblock, long currentblocksize, long blocksize, int blk, int lastblk, int modexponent, mpz_t modulusIntRemainder, mdHashContextList &hclblock) {
    
        int blknum   = blk;
        std::cout << action << " Block " << blknum << " Bytes Size ";

        // display the current block size fraction
        // if this is a 12 byte block
        // 8/12 if the block size is 8 out of a 12 byte block size
        // 12/12 if the blocksize is equal
        std::cout << currentblocksize << "/" << blocksize << std::endl;

        // print the byte block
        std::cout << std::left << std::setw(20) << "Block Bytes ";
        printByteblock(byteblock, currentblocksize, true);

        // display the hash block signatures
        std::cout << std::left << std::setw(20) << "Signatures "        << hclblock.displayHLhashes() << std::endl;

        // display the hash block signatures keys
        std::cout << std::left << std::setw(20) << "Signatures keys "   << hclblock.displayHLhashKeys() << std::endl;

        // display the modulus exponent
        std::cout << std::left << std::setw(20) << "Modulus Exponent "  << modexponent << std::endl;

        // display the modulus remainder
        mpz_class modint(modulusIntRemainder);
        std::cout << std::left << std::setw(20) << "Modulus Remainder " << modint << std::endl << std::endl;
    }

};