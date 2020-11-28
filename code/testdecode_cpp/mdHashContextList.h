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
#include "external/cityhash/cityhash.h"
#include "external/crc/CRC.h"
#include "external/csiphash.c"
#include "external/fasthash/fasthash.h"
#include "external/fnv/fnv.h"
#include "external/highwayhash/highwayhash.h"
#include "external/xxhash/xxhash32.h"
#include "external/xxhash/xxhash64.h"
#include "external/metro64/metrohash64.h"
#include "external/pengyhash/pengyhash.h"
#include "external/seahash/seahash.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "external/md2.c"
#include <openssl/md4.h>
#include <openssl/md5.h>
#include "external/md6/md6.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include "external/spooky/Spooky.h"
#include "external/wyhash/wyhash.h"

enum htype {HSFILE,HSBLOCKGP,HSBLOCK,HSALL};
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
Hashlist mdHashlist[40] = {
    {1,  "cit64",    "Cityhash 64",           false, 8},
    {1,  "crc32",    "CRC 32",                false, 4},
    {1,  "fast32",   "Fasthash 32",           true,  4},
    {1,  "fast64",   "Fasthash 64",           true,  8},
    {2,  "fnv32",    "FNV-1  32",             false, 4},
    {3,  "fnv32a",   "FNV-1a 32",             false, 4},
    {4,  "fnv64",    "FNV-1  64",             false, 8},
    {5,  "fnv64a",   "FNV-1a 64",             false, 8},
    {6,  "hw64",     "Highway Hash 64",       true,  8},
    {6,  "met641",   "Metro Hash 64 v1",      true,  8},
    {6,  "met642",   "Metro Hash 64 v2",      true,  8},
    {7,  "md2",      "MD2",                   false, 16},
    {8,  "md4",      "MD4",                   false, 16},
    {9,  "md5",      "MD5",                   false, 16},
    {10, "md6",      "MD6",                   false, 20},
    {11, "md62",     "MD6 Quicker",           true,  20},
    {11, "png",      "Pengyhash 64",          true,  8},
    {12, "ripe160",  "Ripe MD 160",           false, 20},
    {12, "sea64",    "Seahash 64",            true,  8},
    {12, "sip64",    "Siphash 64",            true,  8},
    {13, "sha1_64",  "SHA1 64",               false, 8},
    {14, "sha1_128", "SHA1 128",              false, 16},
    {15, "sha1",     "SHA1",                  false, 20},
    {15, "sha256",   "SHA 256",               false, 32},
    {15, "sha384",   "SHA 384",               false, 48},
    {15, "sha512",   "SHA 512",               false, 64},
    {16, "spk32",    "Spooky 32",             true,  4},
    {16, "spk64",    "Spooky 64",             true,  8},
    {16, "xxh32",    "xxHash32",              true,  4},
    {17, "xxh64",    "xxHash64",              true,  8},
    {17, "wy64",     "WYhash 64",             true,  8},
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
    // need to make these an array sized 3 for files and bg and block hash results
    uint64_t city64i;
    uint64_t city64o;
    uint64_t city64seed = 102922;
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
    // pengyhash
    uint64_t png64i;
    uint64_t png64o;
    uint32_t png64seed = 127982;
    // ripe 160
    uint8_t ripe160i[41];
    uint8_t ripe160o[41];
    // seahash
    uint64_t sea64i;
    uint64_t sea64o;
    uint64_t sea64seed = 12312;
    // sha1 family
    uint8_t sha1i[41];
    uint8_t sha1o[41];
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
              if (val <= hashlistsize && val > 0) {
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
              if (val <= hashlistsize && val > 0) {
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
              if (val <= hashlistsize && val > 0) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val-1].name;
                // bhlist << hashPair.second << " ";
                blockhlist.push_back(hashPair);
              }
          }

    }

    // setHashList
    void setBlockHashList(unsigned char *byteblock, int blocksize) {

          for(auto hash  : blockhlist) {
              switch(hash.first) {
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
         for(auto hash  : blockhlist) {
              hashblocksize = mdHashlist[hash.first-1].blocksize;

              switch(hash.first) {
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
    // const std::string& displayHLhashes() {
    std::string displayHLhashes() {

        int i = 0;
        int hashblocksize = 0;
        for(auto hash  : blockhlist) {
              // mdHashlist starts at index 0
              hashblocksize = mdHashlist[hash.first-1].blocksize;
              ss << hash.second << " ";
              // ss << "hash id " << std::to_string(hash.first) << " " << " name " << hash.second << " hashblocksize " << std::to_string(hashblocksize) << " ";

              switch(hash.first) {
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
                           ss << std::uppercase << std::hex << (unsigned int)met641i[i];
                     ss << " ";
                     break;
                  case MET642:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (unsigned int)met642i[i];
                     ss << " ";
                     break;
                  case MD2s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)md2i[i];
                     ss << " ";
                     break;
                  case MD4s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)md4i[i];
                     ss << " ";
                     break;
                  case MD5s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)md5i[i];
                     ss << " ";
                     break;
                  case MD6:
                     for(i=0; i<hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)md6i[i];
                     ss << " ";
                     break;
                  case MD62:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)md62i[i];
                     ss << " ";
                     break;
                  case PNG:
                     ss << std::to_string(png64i) << " ";
                     break;
                  case RIPE160:
                    for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)ripe160i[i];
                    break;
                  case SEA:
                     ss << std::to_string(sea64i) << " ";
                     break;
                  case SIP64:
                     ss << std::to_string(siphash64i) << " ";
                     break;
                  case SHA164:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1128:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA1s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case SHA256s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)sha256i[i];
                     ss << " ";
                     break;
                  case SHA512s:
                     for(i=0; i < hashblocksize; ++i)
                           ss << std::uppercase << std::hex << (int)sha512i[i];
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