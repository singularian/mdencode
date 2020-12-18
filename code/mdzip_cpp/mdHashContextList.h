/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * 
 * mdHashContextList.h is the hash context list class
 *
 * A hash context list manages the hash signature list and associated contexts 
 * A hash context list contains a hash list registry
 * It also manages the hash list input vectors  
 * A hash context list calculates signatures for files and file blockgroups and file blocks
 * This class also manages signature comparisons for the modulus scan
 * 
 * 
 * 
*/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <vector>
#include "filehash.h"
#include "../testdecode_cpp/external/cityhash/cityhash.h"
#include "../testdecode_cpp/external/crc/CRC.h"
#include "../testdecode_cpp/external/csiphash.c"
#include "../testdecode_cpp/external/fasthash/fasthash.h"
#include "../testdecode_cpp/external/fnv/fnv.h"
#include "../testdecode_cpp/external/highwayhash/highwayhash.h"
#include "../testdecode_cpp/external/xxhash/xxhash32.h"
#include "../testdecode_cpp/external/xxhash/xxhash64.h"
#include "../testdecode_cpp/external/metro64/metrohash64.h"
#include "../testdecode_cpp/external/pengyhash/pengyhash.h"
#include "../testdecode_cpp/external/seahash/seahash.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../testdecode_cpp/external/md2.c"
#include <openssl/md4.h>
#include <openssl/md5.h>
#include "../testdecode_cpp/external/md6/md6.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include "../testdecode_cpp/external/spooky/Spooky.h"
#include "../testdecode_cpp/external/wyhash/wyhash.h"

enum htype {HASHFILE,HASHBLOCKGROUP,HASHBLOCK,HASHLAST};
enum signatures {FIRST, CIT64, CRC32, FAST32, FAST64, FNV32, FNV32A, FNV64, FNV64A, HW64, MET641, MET642, MD2s, MD4s, MD5s, MD6, MD62, PNG, RIPE160, SEA, SIP64, SHA164, SHA1128, SHA1s, SHA256s, SHA384s, SHA512s, SPK32, SPK64, XXH32, XXH64, WYH, LAST};

// should add a speed column to show which signatures are fastest
// maybe add an enabled/disabled option
struct Hashlist {
    int id;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
};

// could add a zero / non used signature and a last non used
// so it starts at index 1
Hashlist mdHashlist[LAST] = {
    {1,  "cit64",    "Cityhash 64",           false, 8},
    {2,  "crc32",    "CRC 32",                false, 4},
    {3,  "fast32",   "Fasthash 32",           true,  4},
    {4,  "fast64",   "Fasthash 64",           true,  8},
    {5,  "fnv32",    "FNV-1  32",             false, 4},
    {6,  "fnv32a",   "FNV-1a 32",             false, 4},
    {7,  "fnv64",    "FNV-1  64",             false, 8},
    {8,  "fnv64a",   "FNV-1a 64",             false, 8},
    {9,  "hw64",     "Highway Hash 64",       true,  8},
    {10, "met641",   "Metro Hash 64 v1",      true,  8},
    {11, "met642",   "Metro Hash 64 v2",      true,  8},
    {12, "md2",      "MD2",                   false, 16},
    {13, "md4",      "MD4",                   false, 16},
    {14, "md5",      "MD5",                   false, 16},
    {15, "md6",      "MD6",                   false, 20},
    {16, "md62",     "MD6 Quicker",           true,  20},
    {17, "png",      "Pengyhash 64",          true,  8},
    {18, "ripe160",  "Ripe MD 160",           false, 20},
    {19, "sea64",    "Seahash 64",            true,  8},
    {20, "sip64",    "Siphash 64",            true,  8},
    {21, "sha1_64",  "SHA1 64",               false, 8},
    {22, "sha1_128", "SHA1 128",              false, 16},
    {23, "sha1",     "SHA1",                  false, 20},
    {24, "sha256",   "SHA 256",               false, 32},
    {25, "sha384",   "SHA 384",               false, 48},
    {26, "sha512",   "SHA 512",               false, 64},
    {27, "spk32",    "Spooky 32",             true,  4},
    {28, "spk64",    "Spooky 64",             true,  8},
    {29, "xxh32",    "xxHash32",              true,  4},
    {30, "xxh64",    "xxHash64",              true,  8},
    {31, "wy64",     "WYhash 64",             true,  8},
    {32, "last",     "Unused Signature",      false, 8}
};


class mdHashContextList 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    int hashlistsize;
    // hash list vectors
    // vector with tuple - hashnumber, hashname, blocksize
    std::vector<std::tuple<int,std::string,int>> hashlistvt[3];
    // hash results
    // I think I am going to use a separate object for each and then add the file hash method to 
    // set the file signatures. Then I can use one set of results for the signatures and write them to a file
    // need to make these an array sized 3 for files and bg and block hash results - that doesn't work since hashes normally take one dimensional arrays
    uint64_t city64i;
    uint64_t city64o;
    uint64_t city64seed = 102922;
    // could make this a 2d array
    // uint64_t crc64[2][3]
    // uint64_t crc64[2][type]
    // uint64_t crc64[in][type]
    // uint64_t crc64[out][type]
    // crc32
    uint32_t crc32i;
    uint32_t crc32o;
    // crc64
    uint64_t crc64i;
    uint64_t crc64o;
    // fasthash
    uint32_t fast32i;
    uint32_t fast32o;
    uint32_t fast32seed = 200;
    uint64_t fast64i;
    uint64_t fast64o;
    uint64_t fast64seed = 1;
    // fnv1
    Fnv32_t fnv32_1i; 
    Fnv32_t fnv32_1o;
    Fnv32_t fnv32a_1i;
    Fnv32_t fnv32a_1o;
    Fnv64_t fnv64_1i; 
    Fnv64_t fnv64_1o; 
    Fnv64_t fnv64a_1i; 
    Fnv64_t fnv64a_1o;
    // highway hash
    uint64_t hw64i;
    uint64_t hw64o;
    const uint64_t hw64key[4] = {1, 2, 3, 4};
    // metro64hash
    uint8_t met641i[8];
    uint8_t met641o[8];
    uint32_t met641seed = 1237789;
    uint8_t met642i[8];
    uint8_t met642o[8];
    uint32_t met642seed = 1237789;
    // md2 to md6
    uint8_t md2i[16];
    uint8_t md2o[16];
    uint8_t md4i[16];
    uint8_t md4o[16];
    uint8_t md5i[16];
    uint8_t md5o[16];
    uint8_t md6i[20];
    uint8_t md6o[20];
    uint8_t md62i[20];
    uint8_t md62o[20];
    unsigned char md62key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // pengyhash
    uint64_t png64i;
    uint64_t png64o;
    uint32_t png64seed = 127982;
    // ripe 160
    uint8_t ripe160i[21];
    uint8_t ripe160o[21];
    // seahash
    uint64_t sea64i;
    uint64_t sea64o;
    uint64_t sea64seed = 12312;
    // sha1 family
    uint8_t sha1i[21];
    uint8_t sha1o[21];
    uint8_t sha256i[32];
    uint8_t sha256o[32];
    uint8_t sha384i[64];
    uint8_t sha384o[64];
    uint8_t sha512i[64];
    uint8_t sha512o[64];
    // siphash
    uint64_t siphash64i;
    uint64_t siphash64o;
    char sipkey[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    // spooky 32 / 64
    uint32_t spooky32i;
    uint32_t spooky32o;
    uint32_t spookyseed32;
    uint64_t spooky64i;
    uint64_t spooky64o;
    uint64_t spookyseed64;
    // xxhash
    uint32_t xxhash32i;
    uint32_t xxhash32o;
    uint32_t xxseed32 = 0;
    uint64_t xxhash64i;
    uint64_t xxhash64o;
    uint64_t xxseed64 = 0;
    // wyhash
    uint64_t wyhash64i;
    uint64_t wyhash64o;
    uint64_t wyseed64 = 10232123120;
    uint64_t wysecret64[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
public:
    std::stringstream bhlist;
    std::stringstream ss;
    std::stringstream hlss;
    std::string hashlist;

    // initialize mdHashContextList
    mdHashContextList() {

        // set the hashlistsize to the last signatures enum value - 1
        hashlistsize = LAST - 1;
    }

    // Destructor
    ~mdHashContextList()
    {
    } 

    // set the vector hash list
    // input vector list and hash type
    //
    // vector tuple
    // 1) hashnumber
    // 2) hashname
    // 3) hashblocksize
    void setVectorHL(const std::vector<int> &v, int type)
    {
          std::tuple<int,std::string,int> hashTuple;
          for(int val  : v) {
              if (val <= hashlistsize && val > 0) {
                hashTuple = make_tuple(val, mdHashlist[val-1].name, mdHashlist[val-1].blocksize);

                // need to check type size
                hashlistvt[type].push_back(hashTuple);
              }
          }

    }

    // writeBlockHashList
    void writeBlockHashList(std::ofstream &wf) { 

          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    wf.write(reinterpret_cast<char*>(&city64i), sizeof(long));
                    break;
                  case CRC32:
                    // I think there was an issue with this being 64 bits instead of 32 for this crc implementation
                    crc32i = (uint32_t) crc64i;
                    // wf.write(reinterpret_cast<char*>(&crc64i), sizeof(long));
                    wf.write(reinterpret_cast<char*>(&crc32i), sizeof(int));
                    break;
                  case FAST32:
                    wf.write(reinterpret_cast<char*>(&fast32i), sizeof(long));
                    break;
                  case FAST64:
                    wf.write(reinterpret_cast<char*>(&fast64i), sizeof(long));      
                    break;
                  case FNV32:
                    wf.write(reinterpret_cast<char*>(&fnv32_1i), sizeof(long)); 
                    break;
                  case FNV32A:
                    wf.write(reinterpret_cast<char*>(&fnv32_1i), sizeof(long)); 
                    break;
                  case FNV64:
                    wf.write(reinterpret_cast<char*>(&fnv64_1i), sizeof(long));
                    break;
                  case FNV64A:
                    wf.write(reinterpret_cast<char*>(&fnv64a_1i), sizeof(long));
                    break;
                  case HW64:
                    wf.write(reinterpret_cast<char*>(&hw64i), sizeof(long));
                    break;
                  case MET641:
                    wf.write(reinterpret_cast<char*>(&met641i), sizeof(met641i)); 
                    break;
                  case MET642:
                    wf.write(reinterpret_cast<char*>(&met642i), sizeof(met642i)); 
                    break;
                  case MD2s:
                    wf.write(reinterpret_cast<char*>(&md2i), blocksize); 
                    break;
                  case MD4s:
                    wf.write(reinterpret_cast<char*>(&md4i), blocksize); 
                    break;
                  case MD5s:
                    wf.write(reinterpret_cast<char*>(&md5i), blocksize);
                    break;
                  case MD6:
                    wf.write(reinterpret_cast<char*>(&md6i), blocksize);
                    break;
                  case MD62:
                    wf.write(reinterpret_cast<char*>(&md62i), blocksize);
                    break;
                  case PNG:
                    wf.write(reinterpret_cast<char*>(&png64i), blocksize);
                    break;
                  case RIPE160:
                    wf.write(reinterpret_cast<char*>(&ripe160i), blocksize);
                    break;
                  case SEA:
                    wf.write(reinterpret_cast<char*>(&sea64i), sizeof(long));
                    break;
                  case SIP64: 
                    wf.write(reinterpret_cast<char*>(&siphash64i), sizeof(long));
                    break;
                  case SHA164:
                    uint8_t sha164[8];
                    memcpy(sha164, sha1i, blocksize);
                    wf.write(reinterpret_cast<char*>(&sha1i), blocksize);
                    break;
                  case SHA1128:
                    uint8_t sha1128[16];
                    memcpy(sha1128, sha1i, blocksize);
                    wf.write(reinterpret_cast<char*>(&sha1128), blocksize);
                    break;
                  case SHA1s:
                    wf.write(reinterpret_cast<char*>(&sha1i), blocksize);
                    break;
                  case SHA256s:
                    wf.write(reinterpret_cast<char*>(&sha256i), blocksize);
                    break;
                  case SHA384s:
                    wf.write(reinterpret_cast<char*>(&sha384i), blocksize);
                    break;
                  case SHA512s:
                    wf.write(reinterpret_cast<char*>(&sha512i), blocksize);
                    break;
                  case SPK32:
                    wf.write(reinterpret_cast<char*>(&spooky32i), sizeof(int));
                    break;
                  case SPK64:
                    wf.write(reinterpret_cast<char*>(&spooky64i), sizeof(long));
                    break;
                  case XXH32:
                    wf.write(reinterpret_cast<char*>(&xxhash32i), sizeof(int));
                    break;
                  case XXH64:
                    wf.write(reinterpret_cast<char*>(&xxhash64i), sizeof(long));
                    break;
                  case WYH:
                    wf.write(reinterpret_cast<char*>(&wyhash64i), sizeof(long));
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }
    
    // setFileHashList
    // this sets the file hash list signatures for a file
    void setFileHashList(std::string& filename) {
          
          char file[100] = "file.txt";
          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    //city64i = cityhash64_with_seed(byteblock, blocksize, city64seed);
                    break;
                  case CRC32:
                    //crc64i = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                    break;
                  case FAST32:
                    //fast32i = fasthash32(byteblock, blocksize, fast32seed);
                    break;
                  case FAST64:
                    //fast64i = fasthash64(byteblock, blocksize, fast64seed);
                    break;
                  case FNV32:
                    //fnv32_1i = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    break;
                  case FNV32A:
                    //fnv32a_1i = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    break;
                  case FNV64:
                    //fnv64_1i = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    break;
                  case FNV64A:
                    //fnv64a_1i = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    break;
                  case HW64:
                    //hw64i = HighwayHash64(byteblock, blocksize, hw64key);
                    break;
                  case MET641:
                    //metrohash64_1(byteblock, (uint64_t) blocksize, met641seed, met641i);
                    break;
                  case MET642:
                    //metrohash64_2(byteblock, (uint64_t) blocksize, met642seed, met642i);
                    break;
                  case MD2s:
                    //md2(byteblock,(size_t)blocksize,md2i);
                    break;
                  case MD4s:
                    //MD4(byteblock,(long)blocksize,md4i);
                    break;
                  case MD5s:
                    //MD5(byteblock,(long)blocksize,md5i);
                    break;
                  case MD6:
                    //md6_hash(160, byteblock,(uint64_t)(blocksize*8),md6i);
                    break;
                  case MD62:
                    //md6_full_hash(160, byteblock,(uint64_t)(blocksize*8),md62key,16,md6_default_L, 4, md62i);
                    break;
                  case PNG:
                    //png64i = pengyhash(byteblock, (size_t) blocksize, png64seed);
                    break;
                  case RIPE160:
                    //RIPEMD160(byteblock, blocksize, ripe160i);
                    break;
                  case SEA:
                    //sea64i = seahash((const char*)byteblock, blocksize, sea64seed);
                    break;
                  case SIP64: 
                    //siphash64i = siphash24(byteblock, blocksize, sipkey);
                    break;
                  case SHA164:
                    calculateSHA1((char *) filename.c_str(), sha1i);
                    break;
                  case SHA1128:
                    calculateSHA1((char *) filename.c_str(), sha1i);
                    break;
                  case SHA1s:
                    calculateSHA1((char *) filename.c_str(), sha1i);
                    break;
                  case SHA256s:
                    //SHA256(byteblock, blocksize, sha256i);
                    break;
                  case SHA384s:
                    //SHA384(byteblock, blocksize, sha384i);
                    break;
                  case SHA512s:
                    //SHA512(byteblock, blocksize, sha512i);
                    break;
                  case SPK32:
                    //spooky32i = SpookyHash::Hash32(byteblock, blocksize, spookyseed32);
                    break;
                  case SPK64:
                    //spooky64i = SpookyHash::Hash64(byteblock, blocksize, spookyseed64);
                    break;
                  case XXH32:
                    //xxhash32i = XXHash32::hash(byteblock, blocksize, xxseed32);
                    break;
                  case XXH64:
                    //xxhash64i = XXHash64::hash(byteblock, blocksize, xxseed64);
                    break;
                  case WYH:
                    //wyhash64i = wyhash(byteblock, blocksize, wyseed64, (const uint64_t*) wysecret64);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }


    // setByteBlockHashList
    // this sets the block hash list signatures for an input block
    void setByteBlockHashList(unsigned char *byteblock, int blocksize) {

          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    city64i = cityhash64_with_seed(byteblock, blocksize, city64seed);
                    break;
                  case CRC32:
                    crc64i = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                    break;
                  case FAST32:
                    fast32i = fasthash32(byteblock, blocksize, fast32seed);
                    break;
                  case FAST64:
                    fast64i = fasthash64(byteblock, blocksize, fast64seed);
                    break;
                  case FNV32:
                    fnv32_1i = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    break;
                  case FNV32A:
                    fnv32a_1i = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    break;
                  case FNV64:
                    fnv64_1i = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    break;
                  case FNV64A:
                    fnv64a_1i = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    break;
                  case HW64:
                    hw64i = HighwayHash64(byteblock, blocksize, hw64key);
                    break;
                  case MET641:
                    metrohash64_1(byteblock, (uint64_t) blocksize, met641seed, met641i);
                    break;
                  case MET642:
                    metrohash64_2(byteblock, (uint64_t) blocksize, met642seed, met642i);
                    break;
                  case MD2s:
                    md2(byteblock,(size_t)blocksize,md2i);
                    break;
                  case MD4s:
                    MD4(byteblock,(long)blocksize,md4i);
                    break;
                  case MD5s:
                    MD5(byteblock,(long)blocksize,md5i);
                    break;
                  case MD6:
                    md6_hash(160, byteblock,(uint64_t)(blocksize*8),md6i);
                    break;
                  case MD62:
                    md6_full_hash(160, byteblock,(uint64_t)(blocksize*8),md62key,16,md6_default_L, 4, md62i);
                    break;
                  case PNG:
                    png64i = pengyhash(byteblock, (size_t) blocksize, png64seed);
                    break;
                  case RIPE160:
                    RIPEMD160(byteblock, blocksize, ripe160i);
                    break;
                  case SEA:
                    sea64i = seahash((const char*)byteblock, blocksize, sea64seed);
                    break;
                  case SIP64: 
                    siphash64i = siphash24(byteblock, blocksize, sipkey);
                    break;
                  case SHA164:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case SHA1128:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case SHA1s:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case SHA256s:
                    SHA256(byteblock, blocksize, sha256i);
                    break;
                  case SHA384s:
                    SHA384(byteblock, blocksize, sha384i);
                    break;
                  case SHA512s:
                    SHA512(byteblock, blocksize, sha512i);
                    break;
                  case SPK32:
                    spooky32i = SpookyHash::Hash32(byteblock, blocksize, spookyseed32);
                    break;
                  case SPK64:
                    spooky64i = SpookyHash::Hash64(byteblock, blocksize, spookyseed64);
                    break;
                  case XXH32:
                    xxhash32i = XXHash32::hash(byteblock, blocksize, xxseed32);
                    break;
                  case XXH64:
                    xxhash64i = XXHash64::hash(byteblock, blocksize, xxseed64);
                    break;
                  case WYH:
                    wyhash64i = wyhash(byteblock, blocksize, wyseed64, (const uint64_t*) wysecret64);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }

    // run the hash context list on the current byte block and compare the input hash with the computed output hash
    // if the comparison is true return true otherwise return false
    bool compareBlockHashList(unsigned char *byteblock, int blocksize) {

         int hashblocksize = 0;
         for(auto hash  : hashlistvt[HASHBLOCK]) {
              //hashblocksize = mdHashlist[hash.first-1].blocksize;
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    city64o = cityhash64_with_seed(byteblock, blocksize, city64seed);
                    if (city64i != city64o) return false;
                    break;
                  case CRC32:
                    crc64o = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                    if (crc64i != crc64o) return false;
                    break;
                  case FAST32:
                    fast32o = fasthash32(byteblock, blocksize, fast32seed);
                    if (fast32i != fast32o) return false;
                    break;
                  case FAST64:
                    fast64o = fasthash64(byteblock, blocksize, fast64seed);
                    if (fast64i != fast64o) return false;
                    break;
                  case FNV32:
                    fnv32_1o = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    if (fnv32_1i != fnv32_1o) return false;
                    break;
                  case FNV32A:
                    fnv32a_1o = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    if (fnv32a_1i != fnv32a_1o) return false;
                    break;
                  case FNV64:
                    fnv64_1o = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    if (fnv64_1i != fnv64_1o) return false;
                    break;
                  case FNV64A:
                    fnv64a_1o = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    if (fnv64a_1i != fnv64a_1o) return false;
                    break;
                  case HW64:
                    hw64o = HighwayHash64(byteblock, blocksize, hw64key);
                    if (hw64i != hw64o) return false;
                    break;
                  case MET641:
                    metrohash64_1(byteblock, (uint64_t) blocksize, met641seed, met641o);
                    if (memcmp(met641i, met641o, 8) != 0) return false;
                    break;
                  case MET642:
                    metrohash64_2(byteblock, (uint64_t) blocksize, met642seed, met642o);
                    if (memcmp(met642i, met642o, 8) != 0) return false;
                    break;
                  case MD2s:
                    md2(byteblock,(size_t)blocksize,md2o);
                    if (memcmp(md2i, md2o, 16) != 0) return false;
                    break;
                  case MD4s:
                    MD4(byteblock,(long)blocksize,md4o);
                    if (memcmp(md4i, md4o, 16) != 0) return false;
                    break;
                  case MD5s:
                    MD5(byteblock,(long)blocksize,md5o);
                    if (memcmp(md5i, md5o, 16) != 0) return false;
                    break;
                  case MD6:
                    md6_hash(160, byteblock,(uint64_t)(blocksize*8),md6o);
                    if (memcmp(md6i, md6o, 20) != 0) return false;
                    break;
                  case MD62:
                    md6_full_hash(160, byteblock,(uint64_t)(blocksize*8),md62key,16,md6_default_L, 4, md62o);
                    if (memcmp(md62i, md62o, 20) != 0) return false;
                    break;
                  case PNG:
                    png64o = pengyhash(byteblock, (size_t) blocksize, png64seed);
                    if (png64i != png64o) return false;
                    break;
                  case RIPE160:
                    RIPEMD160(byteblock, blocksize, ripe160o);
                    if (memcmp(ripe160i, ripe160o, 20) != 0) return false;
                    break;
                  case SEA:
                    sea64o = seahash((const char*) byteblock, blocksize, sea64seed);
                    if (sea64i != sea64o) return false;
                    break;
                  case SIP64:
                    siphash64o = siphash24(byteblock, blocksize, sipkey);
                    if (siphash64i != siphash64o) return false;
                    break;
                  case SHA164:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 8) != 0) return false;
                    break;
                  case SHA1128:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 16) != 0) return false;
                    break;
                  case SHA1s:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 20) != 0) return false;
                    break;
                  case SHA256s:
                    SHA256(byteblock, blocksize, sha256o);
                    if (memcmp(sha256i, sha256o, 32) != 0) return false;
                    break;
                  case SHA384s:
                    SHA384(byteblock, blocksize, sha384o);
                    if (memcmp(sha384i, sha384o, 48) != 0) return false;
                    break;
                  case SHA512s:
                    SHA512(byteblock, blocksize, sha512o);
                    if (memcmp(sha512i, sha512o, 64) != 0) return false;
                    break;
                  case SPK32:
                    spooky32o = SpookyHash::Hash32(byteblock, blocksize, spookyseed32);
                    if (spooky32i != spooky32o) return false;
                    break;
                  case SPK64:
                    spooky64o = SpookyHash::Hash64(byteblock, blocksize, spookyseed64);
                    if (spooky64i != spooky64o) return false;
                    break;
                  case XXH32:
                    xxhash32o = XXHash32::hash(byteblock, blocksize, xxseed32);
                    if (xxhash32i != xxhash32o) return false;
                    break;
                  case XXH64:
                    xxhash64o = XXHash64::hash(byteblock, blocksize, xxseed64);
                    if (xxhash64i != xxhash64o) return false;
                    break;
                  case WYH:
                    wyhash64o = wyhash(byteblock, blocksize, wyseed64, (const uint64_t*) wysecret64);
                    if (wyhash64i != wyhash64o) return false;
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

          return true;
    }

    // display the hash list
    std::string displayHLhashes() {
        // clear the string stream
        ss.str(std::string());

        int i = 0;
        int hashblocksize = 0;
        for(auto hash  : hashlistvt[HASHBLOCK]) {

              hashblocksize = std::get<2>(hash);
              ss << std::get<1>(hash) << " ";
              // ss << "hash id " << std::get<0>(hash) << " " << " name " << std::get<1>(hash) << " hashblocksize " << std::to_string(hashblocksize) << " ";

              switch(std::get<0>(hash)) {
                  case CIT64:
                     ss << std::to_string(city64i) << " ";
                     break;
                  case CRC32:
                     ss << std::to_string(crc64i) << " ";
                     break;
                  case FAST32:
                     ss << std::to_string(fast32i) << " ";
                     break;
                 case FAST64:
                     ss << std::to_string(fast64i) << " ";
                     break;
                  case FNV32:
                     ss << std::to_string(fnv32_1i) << " ";
                     break;
                  case FNV32A:
                     ss << std::to_string(fnv32a_1i) << " ";
                     break;
                  case FNV64:
                     ss << std::to_string(fnv64_1i) << " ";
                     break;
                  case FNV64A:
                     ss << std::to_string(fnv64a_1i) << " ";
                     break;
                  case HW64:
                     ss << std::to_string(hw64i) << " ";
                     break;
                  case MET641:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (unsigned int)met641i[i];
                     ss << " ";
                     break;
                  case MET642:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (unsigned int)met642i[i];
                     ss << " ";
                     break;
                  case MD2s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)md2i[i];
                     ss << " ";
                     break;
                  case MD4s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)md4i[i];
                     ss << " ";
                     break;
                  case MD5s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)md5i[i];
                     ss << " ";
                     break;
                  case MD6:
                     for(i=0; i<hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)md6i[i];
                     ss << " ";
                     break;
                  case MD62:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)md62i[i];
                     ss << " ";
                     break;
                  case PNG:
                     ss << std::to_string(png64i) << " ";
                     break;
                  case RIPE160:
                    for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)ripe160i[i];
                    break;
                  case SEA:
                     ss << std::to_string(sea64i) << " ";
                     break;
                  case SIP64:
                     ss << std::to_string(siphash64i) << " ";
                     break;
                  case SHA164:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1128:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA256s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha256i[i];
                     ss << " ";
                     break;
                  case SHA384s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha384i[i];
                     ss << " ";
                    break;
                  case SHA512s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)sha512i[i];
                     ss << " ";
                     break;
                  case SPK32:
                    ss << std::to_string(spooky32i) << " ";
                    break;
                  case SPK64:
                    ss << std::to_string(spooky64i) << " ";
                    break;
                  case XXH32:
                    ss << std::to_string(xxhash32i) << " ";
                    break;
                  case XXH64:
                    ss << std::to_string(xxhash64i) << " ";
                    break;
                  case WYH:
                    ss << std::to_string(wyhash64i) << " ";
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
         }
         hashlist = ss.str();
         return hashlist;         
    }

    // display the vector list
    // 1) file hashlist
    // 2) file block group hash list
    // 3) file block hashlist
    void displayHLvectors() {
         int hashblocksize = 0;

         std::cout << "File hashlist " << std::endl;
         std::cout << getHLvectorsString(HASHFILE) << std::endl;

         std::cout << "File block group hash list " << std::endl;
         std::cout << getHLvectorsString(HASHBLOCKGROUP) << std::endl;

         std::cout << "File block hashlist " << std::endl;
         std::cout << getHLvectorsString(HASHBLOCK) << std::endl;

    }

    // get and create the vector list string associated with the hash type
    std::string getHLvectorsString(int type) {
         // clear the hlss string stream
         hlss.str(std::string());
         int blocksize = 0; 

         hlss << std::left << std::setw(12) << "Number";
         hlss << std::left << std::setw(13) << "Hash ID";
         hlss << std::left << std::setw(12) << "Hash Name ";
         hlss << std::left << std::setw(12) << "Blocksize" << std::endl;

         int i = 1;
         for(auto val  : hashlistvt[type]) {
             hlss << std::left << std::setw(12) << i;
             hlss << std::left << std::setw(13) << std::get<0>(val);
             hlss << std::left << std::setw(12) << std::get<1>(val);
             hlss << std::left << std::setw(12) << std::get<2>(val) << std::endl;
             blocksize += std::get<2>(val);
             i++;
         }
         hlss << std::right << std::setw(39) << blocksize << " Total" << std::endl; 

         return hlss.str(); 
    }

    // display the current registry hash list struct of signature hashes currently supported
    void displayHLRegistry(int format) 
    {
         std::cout << std::left << std::setw(12) << "ID";
         std::cout << std::left << std::setw(12) << "Hash Name ";
         std::cout << std::left << std::setw(30) << "Description";
         std::cout << std::left << std::setw(12) << "Key";
         std::cout << std::left << std::setw(12) << "Blocksize" << std::endl;
         std::cout << "==============================================================================" << std::endl;

         for (int i = 0; i < hashlistsize; i++) {
            if (mdHashlist[i].name == "last") break;
            std::cout << std::left << std::setw(12) << i + 1; 
            std::cout << std::left << std::setw(12) << mdHashlist[i].name;
            std::cout << std::left << std::setw(30) << mdHashlist[i].description;
            std::cout << std::left << std::setw(12) << std::boolalpha << mdHashlist[i].haskey;
            std::cout << std::left << std::setw(12) << mdHashlist[i].blocksize;
            std::cout << std::endl;
         }
    }

    // get the hashlist vector length hashlist based on the input hash type
    int getHLVectorLength(int type) {

        int hashListsize = 0;
        hashListsize = hashlistvt[type].size();

        return hashListsize;

    }


    // calculate the block size for the correct hashlist type
    // this returns the sum of the hash list vector block sizes
    int calcBlockSize(int type) {

        int hashblocksize = 0;
        int sumhashblocksize = 0;
        for(auto hash  : hashlistvt[type]) {
              hashblocksize = std::get<2>(hash);
              sumhashblocksize += hashblocksize;
        }

        return sumhashblocksize;

    }
    

};
