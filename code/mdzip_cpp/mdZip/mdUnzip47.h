/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode C++ Modulus Scan mdunzip Program
 * 
 * mdUnzip47.h 
 * validates an mdzip file
 * displays the mdzip block list
 * unzips a mdzip file 
 * 
 * This currently uses 
 * sip40 with a 32 bit modulus and a 2-7 bit bitstream for the modulus exponent
 * sip48/hw48 with a 24 bit modulus and a 2-7 bit bitstream for the modulus exponent
 *
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <sys/stat.h>
#include <thread>
#include <gmp.h>
#include <gmpxx.h>
#include "mdCore/mdCommon.h"
// #include "mdCore/mdHashContextList.h"
#include "mdCore/mdHashContextListNew.h"
#include "mdCore/mdMutex.h"
#include "mdCore/mdMutexLog.h"
#include "mdCore/modscanFile.h"
#include "mdCore/mdBitstream.h"

class mdUnzip47 
{
private:
    int threadcount                 = 8;
    // file variables
    std::string filename;
    std::string unzipfilename;
    size_t inputfilesize            = 0;
    long filesize                   = 0;
    // mdzip variables
    std::string mdzipExtension      = ".mdz";
    std::string mdunzipExtension    = ".out";
    double mdversion                = 1.01;
    long blocksize                  = 10;
    long blockcount                 = 0;
    long blockremainder             = 0; // the last file block size
    long blocknumber                = 1; // current file block number position
    // modulus variables
    int modsizebits                 = 32; // should make this modsizeBits
    int modsizeBytes                = 4;
    int modexponent                 = 0;
    // modexp bitstream variables
    long modByteBlockSize           = 0; // mdzip bitstream modulus exponent byte size
    int modexponentbase             = 0; // the modexponent base for 2 to 6 bit encoding  
    mdBitstream mb;
    // hash context list variables
    int hclfilesize                 = 0; // redundant
    int hclfileblocksize            = 0; // hash file size
    int hclblocksize                = 0; // redundant
    int hclblockblocksize           = 0; // hash file size
    int hclblockkeysize             = 0; // variable for mdunzip 
    std::string filehashnames;   
    std::string blockhashnames;
    std::vector<int> fhlist;
    std::vector<int> bhlist;
    std::string filesigs;
    std::string blockkeys           = "default";
    // file and block hash list data sizes and total size
    std::string filehashvector;
    std::string blockhashvector;
    // mdzip booleans
    bool randombh;
    bool inc;
    bool dec;
    // mdunzip variables
    bool overwrite                  = false;
    bool runlogging                 = false;
    bool validate                   = false;
    bool listfile                   = false; 
    // initialize the gmp bigint variables
    int byteorder                   = 0;
    int endian                      = 0;
    size_t count;
    mpz_t byteblockInt, modulusInt, modulusIntRemainder, remainder;
    // initialize the mutex and log objects
    mdMutex    mutex{8};
    mdMutexLog log{false};
 public:    

    // initialize the mdzip object
    mdUnzip47() {
    }

    // filename, threadcount, overwrite, runlogging, validate
    mdUnzip47(std::string fileName, int threadCount, bool overWrite, bool runLogging, bool Validate, bool listFile) {
        filename    = fileName;
        threadcount = threadCount;
        overwrite   = overWrite;
        runlogging  = runLogging;
        validate    = Validate;
        listfile    = listFile;
                
        // initialize the mutex and log object
        mutex.setThreadCount(threadcount);
        log.setLogging(runlogging);
    }

    // Destructor
    ~mdUnzip47() {
    } 
    
    // read from the zip file
    // TODO
    void readZipFile(std::ifstream &nf) {
        // nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
        // nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
        // nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
        // nf.read(reinterpret_cast<char*>(&modsize),   sizeof(int));
    }

    // set the input filename
    void setFilename(std::string fileName) {
        filename = fileName;
    }

    // set the unzip filename
    void setUnzipFilename(std::string fileName) {
        unzipfilename = filename + mdunzipExtension;
    }

    // set the version
    void setVersion(double version) {
        mdversion = version;
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

        if (modsizebits == 0) {
            modsizeBytes = 0;
            return;    
        }    
 
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

    // increment the block number
    void incBlockNumber() {
        blocknumber++;
    }

    // get the current file block number
    long getBlockNumber() {
        return blocknumber;
    }

    // validate the mdzip file format
    int validateMDzip() {

        // Check the file extension
        // Might want to use compression number ie .100mdz as an extension
        // It can be a mdz extension or out extension for recursion
        std::string fileExt = fileExtension(filename);
        if((fileExt != "mdz") && (fileExt != "out")) {
            std::cout << "Invalid MDzip File!" << std::endl;
            return 1;
        }

        // check if the input file is below the minimum
        // the header is about 36 bytes minimum
        inputfilesize = getFilesize(filename);
        if (inputfilesize < 48) {
            std::cout << "Filename size below mdzip minimum!" << std::endl;
            return 1;
        }

        // open the mdzip file 
        std::ifstream nf(filename, std::ios::in | std::ios::binary);
        if(!nf) {
            std::cout << "Cannot open file!" << std::endl;
            return 1;
        }

        // begin reading in the mdzip file data
        nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
        nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
        nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
        nf.read(reinterpret_cast<char*>(&modsizebits),   sizeof(int));

        // validate the mdversion
        if ((mdversion != 1.11) && (mdversion != 1.12) && (mdversion != 1.01)) {
            std::cout << "MDzip File version doesn't validate" << std::endl; 
            return 1;
        }

        // initialize the modulusbytes size to store the modulo exponent
        setModulus();

        // read the file hash list string from the mdzip file
        // nf.read(reinterpret_cast<char*>(&filehashnames),  hclfilesize);
        nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int)); // 36
        char* buf1 = new char[hclfilesize];
        if (hclfilesize > 0) {
            nf.read(buf1,  hclfilesize);
            filehashnames.assign(buf1, hclfilesize);
            delete buf1;
        }

        // read the file block hash list string from the mdzip file
        nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int)); // 40
        char* buf2 = new char[hclblocksize];
        nf.read(buf2,  hclblocksize);
        blockhashnames.assign(buf2, hclblocksize);
        delete buf2;

        // TODO load the file hash keylist 

        // calculate the file block count and last block size
        blockcount = CalcFileBlocks(filesize, blocksize);
        blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);
        
        mdHashContextList hclfile(HASHFILE);
        mdHashContextList hclblock;

        // set the hash list vector tuple for file and hash blocks
        hclfile.setVectorHLstring(filehashnames);
        hclblock.setVectorHLstring(blockhashnames);

        // calculate the file and file block hash list size
        hclfileblocksize  = hclfile.calcBlockSize();
        hclblockblocksize = hclblock.calcBlockSize();

        // set the file hash list parameters and hash block size
        filehashvector = hclfile.getHLvectorsString();

        // TODO set the hashblockgroup 
        //  std::string bghashvector = getHLvectorsString();

        // set the file block hash list
        blockhashvector = hclblock.getHLvectorsString();

        // TODO load the random file hash keys and key size
        // load the file hash list 
        hclfile.readBlockHashList(nf);

        // Load in the block hash random key size
        nf.read(reinterpret_cast<char*>(&hclblockkeysize),   sizeof(int)); // 44

        // set the filesigs string
        filesigs = hclfile.displayHLhashes();

        // if the hclblockkeylist size is greater than zero 
        // set hclblockkeysize to the hclblockkeysize calculated by the hash context list
        if (hclblockkeysize > 0) {
            hclblockkeysize = hclblock.calcBlockKeySize();
        }  

        // calculate the modulus exponent bitstream block size
        // move the file pointer past the block keylist
        // this simulates a file block hash key read
        nf.seekg (hclblockkeysize,  ios_base::cur);

        // read in the bitstream format type which is a byte currently
        uint8_t format = 0;
        long    mformat = 0;
        nf.read(reinterpret_cast<char*>(&format),  sizeof(char));
        mformat = format; // set this to a (long) 
       
        if (format > 7) {
            std::cout << "MDzip File bitstream format doesn't validate" << std::endl; 
            return 1;
        }

        // initialize the mdBitstream object
        mb.mdInitBitstream(blocksize, blockcount, blockremainder);
        // set the current bitstream format and calculate the bitstream block size
        mb.setFormat(format);
        mb.calcBitstreamSize();
        // add one to the bitstream byte block size for the bitstream format byte
        long modByteBlockSize = mb.getByteBlockSize() + 1;

        // calculate the total hash blcock size
        int hashblocksize  = (hclblock.calcBlockSize() + modsizeBytes);
        int totalblocksize = (hashblocksize * blockcount);

        // calculate the mdzip file size
        long prefixsize  = 40;
        long sumfilesize = prefixsize;
        // add the hash string list size
        sumfilesize += hclfilesize;
        sumfilesize += hclblocksize;

        // set the head size
        long headersize = sumfilesize;

        // add the modexponent block byte size for the bitsteam data
        sumfilesize += modByteBlockSize;

        // add the file block size and key block size and hash block size
        sumfilesize += hclfile.calcBlockSize();
        // if it is the default key hclblockkeysize is zero
        // if it is not the default key then hclblockkeysize is the calculated key block size
        sumfilesize += hclblockkeysize; 
        // add the total block size
        sumfilesize += totalblocksize;

        nf.close();

        if (sumfilesize == inputfilesize) {
            std::cout << "MDzip File " << filename      << " validates " << std::endl; 
            std::cout << "MDzip File " << sumfilesize   << " = " << inputfilesize << std::endl; 
        } else {
            std::cout << "MDzip File doesn't validate " << std::endl << std::endl; 
            std::cout << "MDzip File header size      " << headersize << std::endl; 
            std::cout << "MDzip File hash size        " << hclfile.calcBlockSize() << std::endl; 
            std::cout << "MDzip File block size       " << totalblocksize << std::endl; 
            std::cout << "MDzip File block key size   " << hclblockkeysize << std::endl;
            std::cout << "MDzip File bitstream size   " << modByteBlockSize << std::endl;
            std::cout << std::endl;
            std::cout << "MDzip File Sum " << sumfilesize << " != " << inputfilesize << std::endl;
            return 1;
        }

        return 0;
    }   

    // mdlist
    // display the mdzip block contents of a file
    int mdlist() {

        // check if the listfile is true otherwise don't run the mdlist
        // it is currently running mdlist() and checking if this boolean is true before continuing
        if (!listfile) return 0;

        // Check the file extension
        // Might want to use compression number ie .100mdz as an extension
        // It can be a mdz extension or out extension for recursion
        std::string fileExt = fileExtension(filename);
        if((fileExt != "mdz") && (fileExt != "out")) {
            std::cout << "Invalid MDzip File!" << std::endl;
            return 1;
        }

        // check if the input file is below the minimum
        // the header is about 36 bytes minimum
        inputfilesize = getFilesize(filename);
        if (inputfilesize < 48) {
            std::cout << "Filename size below mdzip minimum!" << std::endl;
            return 1;
        }

        // open the mdzip file 
        std::ifstream nf(filename, std::ios::in | std::ios::binary);
        if(!nf) {
            std::cout << "Cannot open file!" << std::endl;
            return 1;
        }

        // begin reading in the mdzip file data
        nf.read(reinterpret_cast<char*>(&mdversion), sizeof(double));
        nf.read(reinterpret_cast<char*>(&filesize),  sizeof(long));
        nf.read(reinterpret_cast<char*>(&blocksize), sizeof(blocksize));
        nf.read(reinterpret_cast<char*>(&modsizebits),   sizeof(int));

        // validate the mdversion
        if ((mdversion != 1.11) && (mdversion != 1.12) && (mdversion != 1.01)) {
            std::cout << "MDzip File version doesn't validate" << std::endl; 
            return 1;
        }

        // if the increment or decrement is set change the mdVersion 
        // bool incrementKey = false;
        int incKey = NOINC;
        if (mdversion == 1.11) incKey = DEC;
        if (mdversion == 1.12) incKey = INC;

        // initialize the modulusbytes array and bigint to store the modulo remainder
        setModulus(); 
        unsigned char *modulusbytes = new unsigned char[modsizeBytes];
        mpz_init_set_str(modulusIntRemainder, "1", 10);

        // read the file hash list string from the mdzip file
        nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int));
        char* buf = new char[hclfilesize];
        if (hclfilesize > 0) {
            nf.read(buf,  hclfilesize);
            filehashnames.assign(buf, hclfilesize);
            delete buf;
        }

        // read the file block hash list string from the mdzip file
        nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int));
        char* buf2 = new char[hclblocksize];
        nf.read(buf2,  hclblocksize);
        blockhashnames.assign(buf2, hclblocksize);
        delete buf2;

        // TODO load the file hash keylist 

        // calculate the file block count and last block size
        blockcount = CalcFileBlocks(filesize, blocksize);
        blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);
        
        mdHashContextList hclfile(HASHFILE);
        mdHashContextList hclblock;

        // set the hash list vector tuple for file and hash blocks
        hclfile.setVectorHLstring(filehashnames);
        hclblock.setVectorHLstring(blockhashnames);

        // calculate the file and file block hash list size
        hclfileblocksize  = hclfile.calcBlockSize();
        hclblockblocksize = hclblock.calcBlockSize();

        // set the file hash list parameters and hash block size
        filehashvector = hclfile.getHLvectorsString();

        // TODO set the hashblockgroup 
        //  std::string bghashvector = getHLvectorsString();

        // set the file block hash list
        blockhashvector = hclblock.getHLvectorsString();

        // TODO load the random file hash keys and key size
        // load the file hash list 
        hclfile.readBlockHashList(nf);

        // Load in the block hash random key size
        nf.read(reinterpret_cast<char*>(&hclblockkeysize),   sizeof(int));

        // set the filesigs string
        filesigs = hclfile.displayHLhashes();

        // if the hclblockkeylist size is greater than zero load in the random block hash keylist 
        // otherwise the blockkeys will be set as default
        if (hclblockkeysize > 0) {
            hclblock.readKeyList(nf);
            blockkeys = hclblock.displayHLhashKeys();
        }  

        // display the file block hash block list
        // block signatures / modulus exponent / modulus remainder
        int blk = 0;
        int lastblk = blockcount - 1;

        // read in the bitstream format type which is a byte currently
        uint8_t format  = 0;
        long    mformat = 0;
        nf.read(reinterpret_cast<char*>(&format),  sizeof(char));
        mformat = format;

        // initialize the mdBitstream object
        mb.mdInitBitstream(blocksize, blockcount, blockremainder);
        // set the bitstream format
        mb.setFormat(format);
        mb.setBitstreamBitSize();
        // calculate the bitstream size base on the bitstream format
        mb.calcBitstreamSize();
        // calculate the bitstream block size
        long modByteBlockSize = mb.getByteBlockSize();

        // create the bitstream array and read in the modulus exponent bitstream data
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        nf.read(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
        // initialize the bitstream reader
        BitstreamReader bsr(modExpBlock, modByteBlockSize);

        // set the mod exponent base if the bitstream format it 2 to 6 bits
        if (mformat < 7) {
           modexponentbase = bsr.get<7>();
        }    

        // display the mdzip mdlist file info 
        displayInfo(false);

        // read each of the mdzip file signature blocks
        for (blk = 0; blk < blockcount; blk++) {
                if (blocksize == 0) break;

                // read the file block hash list 
                hclblock.readBlockHashList(nf);
                // increment the hash context list block number 
                hclblock.incrementBlockNum(incKey);

                modexponent = 0;

                // read the modulus exponent from the bitstream
                // 2-7 bits at a time
                // currently it runs on it runs from 0 to the last byte block < block count
                if (blk < (blockcount - 1)) {
                    // check the bitformat and read the modexponent from the bitstreamreader
                    // the template needs a constant int for the bitsize
                    // switch (bitformat) {
                    switch (mformat) {
                        case 2:
                            modexponent = bsr.get<2>() + modexponentbase;
                            break;
                        case 3:
                            modexponent = bsr.get<3>() + modexponentbase;
                            break;
                        case 4:
                            modexponent = bsr.get<4>() + modexponentbase;
                            break;
                        case 5:
                            modexponent = bsr.get<5>() + modexponentbase;
                            break;
                        case 6:
                            modexponent = bsr.get<6>() + modexponentbase;
                            break;    
                        case 7:
                            modexponent = bsr.get<7>();
                            break;     
                    } 
                    // std::cout << "exponent " << modexponent << std::endl;
                } else {
                    modexponent = bsr.get<7>();
                    // std::cout << "last exponent " << modexponent << std::endl;
                }

                // read the modulus remainder
                nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
                mpz_import (modulusIntRemainder, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);

                // display the byte block info
                displayBlockInfo("Displaying", blocksize, blk, lastblk, blockremainder, modexponent, hclblock);
        }

        delete modulusbytes;
        mpz_clear(modulusIntRemainder);
        nf.close();

        return 0;

    }

    // mdunzipfile
    // mdunzip a valid mdzip file 
    // ie a file with the *.mdz extension
    // ie file.mdz file
    // the output unzipped file is currently file.mdz.out or extension .out
    int mdunzipfile() {

        std:string mdunzipfile = filename + mdunzipExtension;

        // Check the file extension
        // Might want to use compression number ie .100mdz as an extension
        // It can be a mdz extension or out extension for recursion
        std::string fileExt = fileExtension(filename);
        if((fileExt != "mdz") && (fileExt != "out")) {
            std::cout << "Invalid MDzip File!" << std::endl;
            return 1;
        }

        // check if the input file is below the minimum
        // the header is about 36 bytes minimum
        inputfilesize = getFilesize(filename);
        if (inputfilesize < 48) {
            std::cout << "Filename size below mdzip minimum!" << std::endl;
            return 1;
        }

        // if the overwrite is specified remove the old mdunzip output file if it exists 
        const char *fname = mdunzipfile.c_str();
        if ((overwrite)) {
            if (CheckIfFileExists(mdunzipfile)) std::remove(fname);
        } else {
            if (CheckIfFileExists(mdunzipfile)) {
                std::cout << "Mdunzip " << mdunzipfile << " file exists " << std::endl;
                std::cout << "Please specify the overwrite file option " << std::endl;
                std::cout << "mdunzip --file=" << filename << " --over" << std::endl;
                return 1;
            }
        } 

        // open the mdzip file 
        std::ifstream nf(filename, std::ios::in | std::ios::binary);
        std::ofstream wf(mdunzipfile, std::ios::out | std::ios::binary);
        if(!nf) {
            std::cout << "Cannot open mdzip file!" << std::endl;
            return 1;
        }

        // open the mdunzip output file
        if(!wf) {
            std::cout << "Cannot open mdzip output file!" << std::endl;
            return 1;
        }

        // begin reading in the mdzip file data
        nf.read(reinterpret_cast<char*>(&mdversion),    sizeof(double));
        nf.read(reinterpret_cast<char*>(&filesize),     sizeof(long));
        nf.read(reinterpret_cast<char*>(&blocksize),    sizeof(blocksize));
        nf.read(reinterpret_cast<char*>(&modsizebits),  sizeof(int));

        // validate the mdversion
        if ((mdversion != 1.11) && (mdversion != 1.12) && (mdversion != 1.01)) {
            std::cout << "MDzip File version doesn't validate" << std::endl; 
            return 1;
        }

        // if the increment or decrement is set change the mdVersion 
        // bool incrementKey = false;
        int incKey = NOINC;
        if (mdversion == 1.11) incKey = DEC;
        if (mdversion == 1.12) incKey = INC;

        // initialize the modulusbytes array and bigint to store the modulo remainder
        setModulus(); 
        unsigned char *modulusbytes = new unsigned char[modsizeBytes];
        mpz_init_set_str(modulusInt, "1", 10);
        mpz_init_set_str(modulusIntRemainder, "1", 10);

        // calculate the modulus 2 ^ modsize - 1
        calcModulusInt(modulusInt, modsizebits);

        // read the file hash list string from the mdzip file
        nf.read(reinterpret_cast<char*>(&hclfilesize),    sizeof(int));
        char* buf = new char[hclfilesize];
        if (hclfilesize > 0) {
            nf.read(buf,  hclfilesize);
            filehashnames.assign(buf, hclfilesize);
            delete buf;
        }

        // read the file block hash list string from the mdzip file
        nf.read(reinterpret_cast<char*>(&hclblocksize),   sizeof(int));
        char* buf2 = new char[hclblocksize];
        nf.read(buf2,  hclblocksize);
        blockhashnames.assign(buf2, hclblocksize);
        delete buf2;

        // calculate the file block count and last block size
        blockcount = CalcFileBlocks(filesize, blocksize);
        // calculate the last block size
        blockremainder  = CalcFileBlocksRemainder(filesize, blocksize);

        mdHashContextList hclfile(HASHFILE);
        mdHashContextList hclblock;

        // set the hash list vector tuple for file and hash blocks
        hclfile.setVectorHLstring(filehashnames);
        hclblock.setVectorHLstring(blockhashnames);

        // calculate the file and file block hash list size
        hclfileblocksize  = hclfile.calcBlockSize();
        hclblockblocksize = hclblock.calcBlockSize();

        // set the file hash list parameters and hash block size
        filehashvector = hclfile.getHLvectorsString();

        // TODO set the hashblockgroup 
        //  std::string bghashvector = getHLvectorsString();

        // set the file block hash list
        blockhashvector = hclblock.getHLvectorsString();

        // TODO load the random file hash keys and key size
        // load the file hash list 
        hclfile.readBlockHashList(nf);

        // set the filesigs string
        filesigs = hclfile.displayHLhashes();

        // Load in the block hash random key size
        nf.read(reinterpret_cast<char*>(&hclblockkeysize),   sizeof(int));

        // if the hclblockkeylist size is greater than zero load in the random block hash keylist
        // otherwise the blockkeys will be set as default
        if (hclblockkeysize > 0) {
            hclblock.readKeyList(nf);
            blockkeys = hclblock.displayHLhashKeys();
        }  

        // display the file block hash block list
        // block signatures / modulus exponent / modulus remainder
        int blk = 0;
        int lastblk = blockcount - 1;

        // initialize the modulus scan array
        // this allows it to run mulithreaded 
        modscan* mst = new modscan[threadcount];

        // initialize the modulus scan thread objects with the block hash list string
        for(int tnum = 0; tnum < threadcount; tnum++) {
                mst[tnum].hcl.setVectorHLstring(blockhashnames);
        }

        // calculate modulus exponent bitstream block size
        // read in the bitstream format type which is a byte currently
        // TODO Need to add the bitstream info to the displayInfo(true) and move it after
        uint8_t format = 0;
        long    mformat = 0;
        nf.read(reinterpret_cast<char*>(&format),  sizeof(char));
        mformat = format; // set this to a (long) 

        // initialize the mdBitstream object
        mb.mdInitBitstream(blocksize, blockcount, blockremainder);
        // set the bitstream format
        mb.setFormat(format);
        mb.setBitstreamBitSize();
        // calculate the bitstream size base on the bitstream format
        mb.calcBitstreamSize();
        // calculate the bitstream block size
        long modByteBlockSize = mb.getByteBlockSize();

        // create the bitstream array and read in the modulus exponent bitstream data
        uint8_t *modExpBlock = new uint8_t[modByteBlockSize];
        nf.read(reinterpret_cast<char*>(modExpBlock),  modByteBlockSize);
        // initialize the bitstream reader
        BitstreamReader bsr(modExpBlock, modByteBlockSize);

        // set the mod exponent base if the bitstream format it 2 to 6 bits
        if (mformat < 7) {
           modexponentbase = bsr.get<7>();
        }    

        // display the mdzip file info
        displayInfo(true);

        // read each of the mdzip file signature blocks
        for (blk = 0; blk < blockcount; blk++) {

                // read the file block hash list 
                hclblock.readBlockHashList(nf);

                modexponent = 0;

                // read the modulus exponent from the bitstream
                // 2-7 bits at a time
                // currently it runs on it runs from 0 to the last byte block < block count
                if (blk < (blockcount - 1)) {
                    // check the bitformat and read the modexponent from the bitstreamreader
                    // the template needs a constant int for the bitsize
                    // switch (bitformat) {
                    switch (mformat) {
                        case 2:
                            modexponent = bsr.get<2>() + modexponentbase;
                            break;
                        case 3:
                            modexponent = bsr.get<3>() + modexponentbase;
                            break;
                        case 4:
                            modexponent = bsr.get<4>() + modexponentbase;
                            break;
                        case 5:
                            modexponent = bsr.get<5>() + modexponentbase;
                            break;
                        case 6:
                            modexponent = bsr.get<6>() + modexponentbase;
                            break;    
                        case 7:
                            modexponent = bsr.get<7>();
                            break;     
                    } 
                } else {
                    modexponent = bsr.get<7>();
                }

                // read the modulus remainder
                nf.read(reinterpret_cast<char*>(modulusbytes),   sizeof(char) * modsizeBytes);
                mpz_import (modulusIntRemainder, modsizeBytes, byteorder, sizeof(modulusbytes[0]), endian, 0, modulusbytes);

                // check if this is the last byte block
                // if it is set the blocksize to the last block size
                long currblocksize = blocksize;
                if ((blk == lastblk) && (blockremainder != blocksize)) {
                if (blocksize == 0) break;
                    blocksize = blockremainder;           
                } 

                // after the first block increment the Hash Context List signature keys and block numbers
                // TODO increment the hash context list block number for each signature supported
                // TODO increment the Pseudo Random Number Generator to set the next signature block keys
                //
                // This will potentially allow each block to have a different signature key
                // probably need to add a parameter zip/unzipfile to increment the signature keys and store it in the file
                //
                // increment the block number and signature keys if the signature incrementer is enabled and block number is greater than one
                // needs to increment just once
                // std::cout << "Incrementing block " << blk << "/" << blockcount << std::endl;
                hclblock.incrementBlockNum(incKey); 

                // display the byte block info
                displayBlockInfo("Unzipping", currblocksize, blk, lastblk, blockremainder, modexponent, hclblock);
        
                // set the thread modulus scan objects
                for(int tnum = 0; tnum < threadcount; tnum++) {
                    // if this is the first block run the full initialization
                    if (blk < 1) { 
                        mst[tnum].setModscan(&log, byteorder, endian, modulusIntRemainder, modulusInt, modexponent, 1, blocksize, tnum, threadcount, &mutex);
                    } else {
                        // modulusInt remainder is the mod remainder
                        // modulusInt is the 2 ^ mod exponent
                        mst[tnum].resetThread(modulusIntRemainder, modulusInt, modexponent, blocksize, tnum);
                    }  
                    // set the hash context list and the signatures based on the current byte block
                    // copy the register struct to the thread register
                    mst[tnum].hcl.hregister[0] = hclblock.hregister[0];
                }

                // initialize the modulus scan threads vector
                std::vector<std::thread> threads;
                for(int tnum = 0; tnum < threadcount; tnum++){
                    threads.push_back(std::thread(&modscan::decode, std::ref(mst[tnum])));
                }

                // start the count
                // auto start = std::chrono::high_resolution_clock::now();

                // execute the threads
                for(int tnum = 0; tnum < threads.size(); tnum++)
                {
                    threads.at(tnum).detach();
                } 

                // check the mutex ismatched for three states
                // searching      = 0 // searching for the value with the modscan
                // not found      = 1 // modscan mutext match result
                // found          = 2 // modscan mutext match result 
                //
                // multiple found = 3 // not currently being used
                while (mutex.getIsMatched() == SEARCHING) {
                }

                // if the result is not found display not found
                if (mutex.getIsMatched() == NOTFOUND) {
                    log.writeLog("Modulus Scan Match Not Found");
                    // break; // need to check the other blocks
                // match is found   
                } else {

                    // find the last thread match
                    int threadMatchNumber = mutex.getMatchThread(); 

                    // check the modulus scan results
                    std::ostringstream result; 
                    unsigned char *modbyteblock;
                    modbyteblock = mst[threadMatchNumber].getModscanByteBlock();
                    // result << endl << "Found Match" << endl << endl;
                    // result << "Elapsed Time (s) " << std::to_string(elapsed_time/1e9) << endl;
                    result << "Modulus Scan Thread " << threadMatchNumber << " Match " << std::endl;
                    log.writeLog(result.str());

                    log.logMatchByteblock(modbyteblock, blocksize, false);
                    log.logMatchByteblock(modbyteblock, blocksize, true);

                    // add a newline before the next block
                    std::cout << std::endl << std::endl;

                    // Write the modulus scan byteblock to a file
                    wf.write(reinterpret_cast<char*>(modbyteblock),   blocksize);
                }

                // stop the threads
                for(int tnum = 0; tnum < threadcount; tnum++) {
                    mst[tnum].stopThread();
                    while (!mst[tnum].isStoppedThread()) { }
                }
                
                mutex.resetMatched();
                threads.clear();
        }

        // delete the bitstream array
        delete modExpBlock;
        
        // delete the modscan objects 
        delete[] mst;

        // delete the gmp bigints
        mpz_clear(modulusInt);
        mpz_clear(modulusIntRemainder);
        
        nf.close();
        wf.close();

        if (validate) {
            if (hclfile.compareFileHashList(mdunzipfile)) {  
                log.writeLog("The mdunzip output file validates");
            } else {
                log.writeLog("The mdunzip failed");
                return 1;
            } 
        }

        return 0;

    }

    // display the mdzip file info
    // TODO Add the Bitstream info
    void displayInfo(bool mdzip) {

        std::cout << std::left << std::setw(20) << "Zip Filename: "     << filename << std::endl;
        std::cout << std::left << std::setw(20) << "Unzip Filename: "   << filename << ".out" << std::endl;

        std::cout << std::left << std::setw(20) << "Version: "          << mdversion << std::endl;
        std::cout << std::left << std::setw(20) << "Filesize: "         << filesize  << std::endl;
        std::cout << std::left << std::setw(20) << "Blocksize: "        << blocksize << std::endl;


        std::cout << std::left << std::setw(20) << "Blockcount: "       << blockcount << std::endl;
        std::cout << std::left << std::setw(20) << "Blockremainder: "   << blockremainder << std::endl;
        
        std::cout << std::left << std::setw(20) << "Modsize: "          << modsizebits << std::endl;
        std::cout << std::left << std::setw(20) << "Modsize Bytes: "    << modsizeBytes << std::endl;
        // ==================================================================================================
        std::cout << std::left << std::setw(20) << "Bitstream Format: "     << mb.getFormat() << std::endl;
        std::cout << std::left << std::setw(20) << "Bitstream Bitsize: "    << mb.getBitstreamBitSize() << std::endl;
        std::cout << std::left << std::setw(20) << "Bitstream Bits: "       << mb.getBitBlockSize() << std::endl;
        std::cout << std::left << std::setw(20) << "Bitstream Bytes: "      << mb.getByteBlockSize() << std::endl;
        std::cout << std::left << std::setw(20) << "Bitstream Exp Base: "   << modexponentbase << std::endl;
        // std::cout << std::left << std::setw(20) << "Bitstream Diff Exp: "   << mb.getDiff() << std::endl;
        std::cout << std::left << std::setw(20) << "Bitstream Diff Exp: "   << mb.getFormatDiff() << std::endl;
        // ==================================================================================================
        std::cout << std::left << std::setw(20) << "Filehashlist: "     << filehashnames << std::endl;
        std::cout << std::left << std::setw(20) << "Blockhashlist: "    << blockhashnames << std::endl;
        std::cout << std::left << std::setw(20) << "Blockkeylist: "     << blockkeys << std::endl;

        std::cout << std::left << std::setw(20) << "File Hash Bytes: "  << hclfileblocksize << std::endl;
        std::cout << std::left << std::setw(20) << "Block Hash Bytes: " << hclblockblocksize << std::endl;
        if (mdzip) {
        std::cout << std::left << std::setw(20) << "Threadcount:"       << threadcount << std::endl; }
        std::cout << std::left << std::setw(20) << "Logging: "          << boolalpha << log.checkIfLogging() << std::endl;
        std::cout << std::left << std::setw(20) << "Platform:"          << (is_big_endian()? "Big": "Little") << " Endian" << std::endl;

        // display the file hash list parameters and hash block size
        std::cout << std::endl;
        std::cout << "File hashlist " << std::endl;
        std::cout << filehashvector << std::endl;

        // TODO display the hashblockgroup 
        // std::cout << "File block group hash list " << std::endl;
        // std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

        // display the file block hash list parameters and hash block size
        std::cout << "File block hashlist " << std::endl;
        std::cout << blockhashvector << std::endl;

        // display the file signature list before the file block level signatures
        // cit64 16527838090914794844 crc32 706283486
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
    void displayBlockInfo(std::string action, int blocksize, int blk, int lastblk, long blockremainder, int modexponent, mdHashContextList &hclblock) {
    
        int blknum   = blk + 1;
        int cblksize = blocksize;
        std::cout << action << " Block " << blknum << " Bytes Size ";

        // if this is the last block
        // set the blocksize to the last block size
        if ((blk == lastblk) && (blockremainder != blocksize)) {
                cblksize = blockremainder;      
        } 

        // display the current block size fraction
        // if this is a 12 byte block
        // 8/12 if the block size is 8 out of a 12 byte block size
        // 12/12 if the blocksize is equal
        std::cout << cblksize << "/" << blocksize << std::endl;

        // display out the hash block signatures
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