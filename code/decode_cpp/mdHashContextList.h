/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * 
 * mdHashContextList.h is the hash context list class
 * 
 * 
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "external/CRC.h"
#include "external/highwayhash.h"
#include "external/md2.c"
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include "external/csiphash.c"
#include "fnv/fnv.h"
#include "external/xxhash32.h"
#include "external/xxhash64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "external/md6.h"

enum signatures {FIRST, CRC32, FNV32, FNV32A, FNV64, FNV64A, HW64, MD2s, MD4s, MD5s, MD6, MD62, SIP64, SHA164, SHA1128, SHA1s, XXH32, XXH64, LAST};

// should add a speed column to show which signatures are fastest
// maybe add an enabled/disabled option
struct Hashlist {
    int id;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
};

Hashlist mdHashlist[18] = {
    {1,  "crc32",    "CRC 32",                false, 4},
    {2,  "fnv32",    "FNV-1 32",              false, 4},
    {3,  "fnv32a",   "FNV-1a 32",             false, 4},
    {4,  "fnv64",    "FNV-1 64",              false, 8},
    {5,  "fnv64a",   "FNV-1a 64",             false, 8},
    {6,  "hw64",     "Highway Hash 64",       true,  8},
    {7,  "md2",      "MD2",                   false, 16},
    {8,  "md4",      "MD4",                   false, 16},
    {9,  "md5",      "MD5",                   false, 16},
    {10, "md6",      "MD6",                   false, 20},
    {11, "md62",     "MD6 Quicker",           true,  20},
    {12, "sip64",    "Siphash 64",            true,  8},
    {13, "sha1_64",  "SHA1 64",               false, 8},
    {14, "sha1_128", "SHA1 128",              false, 16},
    {15, "sha1",     "SHA1",                  false, 20},
    {16, "xxh32",    "xxHash32",              true,  4},
    {17, "xxh64",    "xxHash64",              true,  8},
    {18, "last",     "Unused Signature",      false, 8}
};


class mdHashContextList 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    int hashlistsize;
    // should make this a tuple and add the block size
    // https://www.geeksforgeeks.org/tuples-in-c/
    std::vector<std::pair<int,std::string>> filehlist;
    std::vector<std::pair<int,std::string>> blockgrouphlist;
    std::vector<std::pair<int,std::string>> blockhlist;
    // hash results
    // need to make these an array sized 6 for files and bg and block hash results
    uint64_t crc64i;
    uint64_t crc64o;
    Fnv32_t fnv32_1i; 
    Fnv32_t fnv32_1o;
    Fnv32_t fnv32a_1i;
    Fnv32_t fnv32a_1o;
    Fnv64_t fnv64_1i; 
    Fnv64_t fnv64_1o; 
    Fnv64_t fnv64a_1i; 
    Fnv64_t fnv64a_1o; 
    uint64_t hw64i;
    uint64_t hw64o;
    const uint64_t hw64key[4] = {1, 2, 3, 4};
    uint8_t md2i[41];
    uint8_t md2o[41];
    uint8_t md4i[41];
    uint8_t md4o[41];
    uint8_t md5i[41];
    uint8_t md5o[41];
    uint8_t md6i[41];
    uint8_t md6o[41];
    uint8_t md62i[41];
    uint8_t md62o[41];
    unsigned char md62key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    uint8_t sha1i[41];
    uint8_t sha1o[41];
    uint32_t siphash64i;
    uint32_t siphash64o;
    char sipkey[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    uint32_t xxhash32i;
    uint32_t xxhash32o;
    uint32_t xxseed32 = 0;
    uint64_t xxhash64i;
    uint64_t xxhash64o;
    uint64_t xxseed64 = 0;
public:
    std::stringstream ss;
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

    // set the file hash list
    void setFileHashList(const std::vector<int> &v)
    {  
          std::pair<int,std::string> hashPair;
          for(int val  : v) {
              if (val < hashlistsize && val > 0) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val-1].name;
                filehlist.push_back(hashPair);
              }
          }

    }

    // set the block group hash list
    void setBlockGroupHashList(const std::vector<int> &v)
    {  
          std::pair<int,std::string> hashPair; 
          for(int val  : v) {
              if (val < hashlistsize && val > 0) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val-1].name;
                blockgrouphlist.push_back(hashPair);
              }
          }

    }

    // set the file block hash list
    void setBlockHashList(const std::vector<int> &v)
    {
          std::pair<int,std::string> hashPair;
          for(int val  : v) {
              if (val < hashlistsize && val > 0) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val-1].name;
                blockhlist.push_back(hashPair);
              }
          }

    }

    // setHashList
    void setBlockHashList(unsigned char *byteblock, int blocksize) {

          for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case CRC32:
                    crc64i = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
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
                  case XXH32:
                    xxhash32i = XXHash32::hash(byteblock, blocksize, xxseed32);
                    break;
                  case XXH64:
                    xxhash64i = XXHash64::hash(byteblock, blocksize, xxseed64);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }

    // run the hash context list on the current byte block and compare the input hash with the computed output hash
    // if the comparison is true return true otherwise return false
    bool compareBlockHashList(unsigned char *byteblock, int blocksize) {
         for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case CRC32:
                    crc64o = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                    if (crc64i != crc64o) return false;
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
                  case XXH32:
                    xxhash32o = XXHash32::hash(byteblock, blocksize, xxseed32);
                    if (xxhash32i != xxhash32o) return false;
                    break;
                  case XXH64:
                    xxhash64o = XXHash64::hash(byteblock, blocksize, xxseed64);
                    if (xxhash64i != xxhash64o) return false;
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

          return true;
    }

    // display the hash list
    // const std::string& displayHLhashes() {
    std::string displayHLhashes() {
        for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case CRC32:
                     ss << hash.second << " " << std::to_string(crc64i) << " ";
                     break;
                  case FNV32:
                     ss << hash.second << " " << std::to_string(fnv32_1i) << " ";
                     break;
                  case FNV32A:
                     ss << hash.second << " " << std::to_string(fnv32a_1i) << " ";
                     break;
                  case FNV64:
                     ss << hash.second << " " << std::to_string(fnv64_1i) << " ";
                     break;
                  case FNV64A:
                     ss << hash.second << " " << std::to_string(fnv64a_1i) << " ";
                     break;
                  case HW64:
                     ss << hash.second << " " << std::to_string(hw64i) << " ";
                     break;
                  case MD2s:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)md2i[i];
                     ss << " ";
                     break;
                  case MD4s:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)md4i[i];
                     ss << " ";
                     break;
                  case MD5s:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)md5i[i];
                     ss << " ";
                     break;
                  case MD6:
                     ss << hash.second << " ";
                     for(int i=0; i<20; ++i)
                           ss << std::uppercase << std::hex << (int)md6i[i];
                     ss << " ";
                     break;
                  case MD62:
                     ss << hash.second << " ";
                     for(int i=0; i<20; ++i)
                           ss << std::uppercase << std::hex << (int)md62i[i];
                     ss << " ";
                     break;
                  case SIP64:
                     ss << hash.second << " " << std::to_string(siphash64i) << " ";
                     break;
                  case SHA164:
                     ss << hash.second << " ";
                     for(int i=0; i<8; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1128:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1s:
                     ss << hash.second << " ";
                     for(int i=0; i<20; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case XXH32:
                    ss << hash.second << " " << std::to_string(xxhash32i) << " ";
                    break;
                  case XXH64:
                    ss << hash.second << " " << std::to_string(xxhash64i) << " ";
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
         std::cout << "file hashlist " << std::endl;
         for(auto val  : filehlist) 
             std::cout << val.first << " " << val.second << std::endl;

         std::cout << "file block group hash list " << std::endl;
         for(auto val  : blockgrouphlist)  
             std::cout << val.first << " " << val.second << std::endl;

         std::cout << "file block hashlist " << std::endl;
         for(auto val  : blockhlist)  
             std::cout << val.first << " " << val.second << std::endl;

    }

    // display the current list of signature hashes currently supported
    void displayHashList(int format) 
    {

        std::cout << std::left << std::setw(12) << "ID";
        std::cout << std::left << std::setw(12) << "Hash Name ";
        std::cout << std::left << std::setw(30) << "Description";
        std::cout << std::left << std::setw(12) << "Key";
        std::cout << std::left << std::setw(12) << "Blocksize" << std::endl;

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

    

};
