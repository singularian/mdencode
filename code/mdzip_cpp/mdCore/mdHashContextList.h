/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Hash Context List Program
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
#include <map>
#include <sstream>
#include <stdint.h>
#include <tuple>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mdFilehash.h"
#include "mdRandom.h"
#include "mdRegisters.h"
#include "../testdecode_cpp/external/cityhash/cityhash.h"
#include "../testdecode_cpp/external/crc32/crc32.h"
#include "../testdecode_cpp/external/crc64/crc64.h"
#include "../testdecode_cpp/external/csiphash.c"
#include "../testdecode_cpp/external/fasthash/fasthash.h"
#include "../testdecode_cpp/external/fnv/fnv.h"
#include "../testdecode_cpp/external/highwayhash/highwayhash.h"
#include "../testdecode_cpp/external/xxhash/xxhash32.h"
#include "../testdecode_cpp/external/xxhash/xxhash64.h"
#include "../testdecode_cpp/external/metro64/metrohash64.h"
#include "../testdecode_cpp/external/mx3/mx3.h"
#include "../testdecode_cpp/external/pengyhash/pengyhash.h"
#include "../testdecode_cpp/external/seahash/seahash.c"
#include "../testdecode_cpp/external/md2.c"
#include <openssl/md4.h>
#include <openssl/md5.h>
#include "../testdecode_cpp/external/md6/md6.h"
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include "../testdecode_cpp/external/spooky/Spooky.h"
#include <openssl/whrlpool.h>
#include "../testdecode_cpp/external/wyhash/wyhash.h"

enum htype {HASHFILE,HASHBLOCKGROUP,HASHBLOCK,HASHLAST};
enum signatures {FIRST, CIT64, CRC32, CRC64, FAST32, FAST64, FNV32, FNV32A, FNV64, FNV64A, HW64, MD2s, MD4s, MD5s, MD6, MD62, MET641, MET642, MX3, PNG, RIPE160, SEA, SIP64, SHA164, SHA1128, SHA1s, SHA256s, SHA384s, SHA512s, SPK32, SPK64, XXH32, XXH64, WP, WYH, LAST};

// should add a speed column to show which signatures are fastest
// maybe add an enabled/disabled option
struct Hashlist {
    int id;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
    int keysize;
};

// could add a zero / non used signature and a last non used
// so it starts at index 1
Hashlist mdHashlist[LAST] = {
    {1,  "cit64",    "Cityhash 64",           true,     8,      8},
    {2,  "crc32",    "CRC 32",                true,     4,      4},
    {3,  "crc64",    "CRC 64",                true,     8,      8},
    {4,  "fast32",   "Fasthash 32",           true,     4,      4},
    {5,  "fast64",   "Fasthash 64",           true,     8,      8},
    {6,  "fnv32",    "FNV-1  32",             false,    4,      0},
    {7,  "fnv32a",   "FNV-1a 32",             false,    4,      0},
    {8,  "fnv64",    "FNV-1  64",             false,    8,      0},
    {9,  "fnv64a",   "FNV-1a 64",             false,    8,      0},
    {10, "hw64",     "Highway Hash 64",       true,     8,      32},
    {11, "md2",      "MD2",                   false,    16,     0},
    {12, "md4",      "MD4",                   false,    16,     0},
    {13, "md5",      "MD5",                   false,    16,     0},
    {14, "md6",      "MD6",                   false,    20,     0},
    {15, "md62",     "MD6 Quicker",           true,     20,     0},
    {16, "met641",   "Metro Hash 64 v1",      true,     8,      4},
    {17, "met642",   "Metro Hash 64 v2",      true,     8,      4},
    {18, "mx3",      "MX3",                   true,     8,      8},
    {19, "png",      "Pengyhash 64",          true,     8,      4},
    {20, "ripe160",  "Ripe MD 160",           false,    20,     0},
    {21, "sea64",    "Seahash 64",            true,     8,      8},
    {22, "sip64",    "Siphash 64",            true,     8,      16},
    {23, "sha1_64",  "SHA1 64",               false,    8,      0},
    {24, "sha1_128", "SHA1 128",              false,    16,     0},
    {25, "sha1",     "SHA1",                  false,    20,     0},
    {26, "sha256",   "SHA 256",               false,    32,     0},
    {27, "sha384",   "SHA 384",               false,    48,     0},
    {28, "sha512",   "SHA 512",               false,    64,     0},
    {29, "spk32",    "Spooky 32",             true,     4,      4},
    {30, "spk64",    "Spooky 64",             true,     8,      8},
    {31, "xxh32",    "xxHash32",              true,     4,      4},
    {32, "xxh64",    "xxHash64",              true,     8,      8},
    {33, "whp",      "Whirlpool",             false,    64,     0},
    {34, "wy64",     "WYhash 64",             true,     8,      136},
    {32, "last",     "Unused Signature",      false,    8,      0}
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
    // hash map
    std::map<std::string, int> hclmap;
public:
    // hash results registers
    // could make the struct public for copying
    hash_list hregister [2];
    hash_key_list keyregister;
    // string stream variables
    std::stringstream bhlist;
    std::stringstream ss;
    std::stringstream hlss;
    std::string hashlist;


    // initialize mdHashContextList
    mdHashContextList() {

        // set the hashlistsize to the last signatures enum value - 1
        hashlistsize = LAST - 1;

        // add the struct hash registry to the hclmap
        // could move this to a separate function
        int hsnum = 0;
        for (hsnum = 0; hsnum < hashlistsize; hsnum++) {
             hclmap[mdHashlist[hsnum].name] = hsnum;
        }
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
    // void setVectorHL(const std::vector<int> &v, int type)
    void setVectorHL(std::vector<int> &v, int type)
    {
          // Deduplicate or unique the hashlist input vector v and preserve the order
          // set the iterator  
          std::vector<int>::iterator ip; 
  
          // Use std::unique on the hash list vector
          ip = std::unique(v.begin(), v.end()); 
  
          // Resizing the input vector to remove the undefined terms 
          v.resize(std::distance(v.begin(), ip)); 

          // create a hashlist tuple vector
          std::tuple<int,std::string,int> hashTuple;
          for(int val  : v) {
              if (val <= hashlistsize && val > 0) {
                hashTuple = make_tuple(val, mdHashlist[val-1].name, mdHashlist[val-1].blocksize);

                // need to check type size
                hashlistvt[type].push_back(hashTuple);
              }
          }

    }

    // setVectorHLstring
    // converts a string list to a int vector and then to the vector tuple
    // the delimeter is ':' currently
    // ie hash string cit64:crc32:fast32
    // ie hash string cit64:crc32:fast32:fast64:fnv32a:fnv64:md2:md4:met641:met642:sha512:whp
    void setVectorHLstring(std::string hashlist, int type) {
          std::vector<int> v;

          int hashnum = 0;
          char delim = ':';
          std::stringstream ss(hashlist);
          std::string token;

          // split the hashlist string by the delimmeter and add the hashmap int to the vector v
          while (std::getline(ss, token, delim)) {

              // check if the hash token is a valid hash signature in the registry hash map
              if ( hclmap.find(token) == hclmap.end() ) {
                 std::cout << "Hash signature " << token << " not found " << std::endl;

              // if it is found add it to the integer vector
              } else {
                 hashnum = hclmap[token];
                 v.push_back(hashnum+1);
              }
          }

          // set the Vector Hash List Tuple
          setVectorHL(v, type);
    }

    // load the Signature keys from a file
    void readKeyList(std::ifstream &rf) {
          int hashblocksize = 0;

          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].city64seed), sizeof(long));
                    break;
                  case CRC32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].crc32seed), sizeof(int));
                    break;
                  case CRC64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].crc64seed), sizeof(long));
                    break;
                  case FAST32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fast32seed), sizeof(int));
                    break;
                  case FAST64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fast64seed), sizeof(long));      
                    break;
                  case FNV32:
                    break;
                  case FNV32A:
                    break;
                  case FNV64:
                    break;
                  case FNV64A:
                    break;
                  case HW64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].hw64key), 32);
                    break;
                  case MD2s:
                    break;
                  case MD4s: 
                    break;
                  case MD5s:
                    break;
                  case MD6:
                    //rf.read(reinterpret_cast<char*>(&hregister[0].md6i), hashblocksize);
                    break;
                  case MD62:
                    // rf.read(reinterpret_cast<char*>(&hregister[0].md62i), hashblocksize);
                    break;
                  case MET641:
                    rf.read(reinterpret_cast<char*>(&hregister[0].met641seed), sizeof(hregister[0].met641seed));
                    break;
                  case MET642:
                    rf.read(reinterpret_cast<char*>(&hregister[0].met642seed), sizeof(hregister[0].met642seed));
                    break;
                  case MX3:
                    rf.read(reinterpret_cast<char*>(&hregister[0].mx3seed), sizeof(hregister[0].mx3seed));
                    break;
                  case PNG:
                    rf.read(reinterpret_cast<char*>(&hregister[0].png64seed), sizeof(uint64_t));
                    break;
                  case RIPE160:
                    break;
                  case SEA:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sea64seed), sizeof(uint64_t));
                    break;
                  case SIP64: 
                    rf.read(reinterpret_cast<char*>(&hregister[0].sipkey), 16);
                    break;
                  case SHA164:
                    break;
                  case SHA1128:
                    break;
                  case SHA1s:
                    break;
                  case SHA256s:
                    break;
                  case SHA384s:
                    break;
                  case SHA512s:
                    break;
                  case SPK32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].spookyseed32), sizeof(int));
                    break;
                  case SPK64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].spookyseed64), sizeof(long));
                    break;
                  case XXH32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].xxseed32), sizeof(int));
                    break;
                  case XXH64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].xxseed64), sizeof(long));
                    break;
                  case WP:
                    break;
                  case WYH:
                    rf.read(reinterpret_cast<char*>(&hregister[0].wyseed64), sizeof(long));
                    rf.read(reinterpret_cast<char*>(&hregister[0].wysecret64), 128);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }   

    // write the Signature keys to a file
    void writeKeyList(std::ofstream &wf) {
          int hashblocksize = 0;

          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].city64seed), sizeof(long));
                    break;
                  case CRC32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].crc32seed), sizeof(int));
                    break;
                  case CRC64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].crc64seed), sizeof(long));
                    break;
                  case FAST32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fast32seed), sizeof(int));
                    break;
                  case FAST64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fast64seed), sizeof(long));      
                    break;
                  case FNV32:
                    break;
                  case FNV32A:
                    break;
                  case FNV64:
                    break;
                  case FNV64A:
                    break;
                  case HW64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].hw64key), 32);
                    break;
                  case MD2s:
                    break;
                  case MD4s: 
                    break;
                  case MD5s:
                    break;
                  case MD6:
                    // wf.write(reinterpret_cast<char*>(&hregister[0].md6i), hashblocksize);
                    break;
                  case MD62:
                    // wf.write(reinterpret_cast<char*>(&hregister[0].md62i), hashblocksize);
                    break;
                  case MET641:
                    wf.write(reinterpret_cast<char*>(&hregister[0].met641seed), sizeof(hregister[0].met641seed));
                    break;
                  case MET642:
                    wf.write(reinterpret_cast<char*>(&hregister[0].met642seed), sizeof(hregister[0].met642seed));
                    break;
                  case MX3:
                    wf.write(reinterpret_cast<char*>(&hregister[0].mx3seed), sizeof(hregister[0].mx3seed));
                    break;
                  case PNG:
                    wf.write(reinterpret_cast<char*>(&hregister[0].png64seed), sizeof(uint64_t));
                    break;
                  case RIPE160:
                    break;
                  case SEA:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sea64seed), sizeof(uint64_t));
                    break;
                  case SIP64: 
                    wf.write(reinterpret_cast<char*>(&hregister[0].sipkey), 16);
                    break;
                  case SHA164:
                    break;
                  case SHA1128:
                    break;
                  case SHA1s:
                    break;
                  case SHA256s:
                    break;
                  case SHA384s:
                    break;
                  case SHA512s:
                    break;
                  case SPK32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].spookyseed32), sizeof(int));
                    break;
                  case SPK64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].spookyseed64), sizeof(long));
                    break;
                  case XXH32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].xxseed32), sizeof(int));
                    break;
                  case XXH64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].xxseed64), sizeof(long));
                    break;
                  case WP:
                    break;
                  case WYH:
                    wf.write(reinterpret_cast<char*>(&hregister[0].wyseed64), sizeof(long));
                    wf.write(reinterpret_cast<char*>(&hregister[0].wysecret64), 128);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }  

    // randomize the Signature keys based on the signature list
    void randomizeKeyList() {
          int hashblocksize = 0;

          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    hregister[0].city64seed = randLong();
                    break;
                  case CRC32:
                    hregister[0].crc32seed = randInt();
                    break;
                  case CRC64:
                    hregister[0].crc64seed = randInt();
                    break;
                  case FAST32:
                    hregister[0].fast32seed = randInt();
                    break;
                  case FAST64:
                    hregister[0].fast64seed = randInt();    
                    break;
                  case FNV32:
                    break;
                  case FNV32A:
                    break;
                  case FNV64:
                    break;
                  case FNV64A:
                    break;
                  case HW64:
                    genRandomLongBlock(hregister[0].hw64key, 4);
                    break;
                  case MD2s:
                    break;
                  case MD4s: 
                    break;
                  case MD5s:
                    break;
                  case MD6:
                    //rf.read(reinterpret_cast<char*>(&hregister[0].md6i), hashblocksize);
                    break;
                  case MD62:
                    // rf.read(reinterpret_cast<char*>(&hregister[0].md62i), hashblocksize);
                    break;
                  case MET641:
                    hregister[0].met641seed = randInt();   
                    break;
                  case MET642:
                    hregister[0].met642seed = randInt();   
                    break;
                  case MX3:
                    hregister[0].mx3seed = randLong(); 
                    break;
                  case PNG:
                    hregister[0].png64seed = randInt(); 
                    break;
                  case RIPE160:
                    break;
                  case SEA:
                     hregister[0].sea64seed = randLong(); 
                    break;
                  case SIP64: 
                    genRandomByteBlock(hregister[0].sipkey, 16); 
                    break;
                  case SHA164:
                    break;
                  case SHA1128:
                    break;
                  case SHA1s:
                    break;
                  case SHA256s:
                    break;
                  case SHA384s:
                    break;
                  case SHA512s:
                    break;
                  case SPK32:
                    hregister[0].spookyseed32 = randInt();   
                    break;
                  case SPK64:
                    hregister[0].spookyseed64 = randLong();   
                    break;
                  case XXH32:
                    hregister[0].xxseed32 = randInt();   
                    break;
                  case XXH64:
                    hregister[0].xxseed64 = randLong();    
                    break;
                  case WP:
                    break;
                  case WYH:
                    hregister[0].wyseed64 = randLong();  
                    genRandomLongBlock(hregister[0].wysecret64, 16);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }    

    // readBlockHashList
    // read the hash list to a ifstream file object
    void readBlockHashList(std::ifstream &rf) { 

          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].city64i), sizeof(long));
                    break;
                  case CRC32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].crc32i), sizeof(int));
                    break;
                  case CRC64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].crc64i), sizeof(long));
                    break;
                  case FAST32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fast32i), sizeof(int));
                    break;
                  case FAST64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fast64i), sizeof(long));      
                    break;
                  case FNV32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fnv32_1i), sizeof(int)); 
                    break;
                  case FNV32A:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fnv32a_1i), sizeof(int)); 
                    break;
                  case FNV64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fnv64_1i), sizeof(long));
                    break;
                  case FNV64A:
                    rf.read(reinterpret_cast<char*>(&hregister[0].fnv64a_1i), sizeof(long));
                    break;
                  case HW64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].hw64i), sizeof(long));
                    break;
                  case MD2s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].md2i), hashblocksize); 
                    break;
                  case MD4s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].md4i), hashblocksize); 
                    break;
                  case MD5s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].md5i), hashblocksize);
                    break;
                  case MD6:
                    rf.read(reinterpret_cast<char*>(&hregister[0].md6i), hashblocksize);
                    break;
                  case MD62:
                    rf.read(reinterpret_cast<char*>(&hregister[0].md62i), hashblocksize);
                    break;
                  case MET641:
                    rf.read(reinterpret_cast<char*>(&hregister[0].met641i), sizeof(hregister[0].met641i));
                    break;
                  case MET642:
                    rf.read(reinterpret_cast<char*>(&hregister[0].met642i), sizeof(hregister[0].met642i));
                    break;
                  case MX3:
                    rf.read(reinterpret_cast<char*>(&hregister[0].mx3i), hashblocksize);
                    break;
                  case PNG:
                    rf.read(reinterpret_cast<char*>(&hregister[0].png64i), hashblocksize);
                    break;
                  case RIPE160:
                    rf.read(reinterpret_cast<char*>(&hregister[0].ripe160i), hashblocksize);
                    break;
                  case SEA:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sea64i), sizeof(long));
                    break;
                  case SIP64: 
                    rf.read(reinterpret_cast<char*>(&hregister[0].siphash64i), sizeof(long));
                    break;
                  case SHA164:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha1i), hashblocksize);
                    break;
                  case SHA1128:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha1i), hashblocksize);
                    break;
                  case SHA1s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha1i), hashblocksize);
                    break;
                  case SHA256s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha256i), hashblocksize);
                    break;
                  case SHA384s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha384i), hashblocksize);
                    break;
                  case SHA512s:
                    rf.read(reinterpret_cast<char*>(&hregister[0].sha512i), hashblocksize);
                    break;
                  case SPK32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].spooky32i), sizeof(int));
                    break;
                  case SPK64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].spooky64i), sizeof(long));
                    break;
                  case XXH32:
                    rf.read(reinterpret_cast<char*>(&hregister[0].xxhash32i), sizeof(int));
                    break;
                  case XXH64:
                    rf.read(reinterpret_cast<char*>(&hregister[0].xxhash64i), sizeof(long));
                    break;
                  case WP:
                    rf.read(reinterpret_cast<char*>(&hregister[0].whp512i), hashblocksize);
                    break;
                  case WYH:
                    rf.read(reinterpret_cast<char*>(&hregister[0].wyhash64i), sizeof(long));
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }

    // writeBlockHashList
    // write the hash list to a ofstream file object
    void writeBlockHashList(std::ofstream &wf) { 

          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].city64i), sizeof(long));
                    break;
                  case CRC32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].crc32i), sizeof(int));
                    break;
                  case CRC64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].crc64i), sizeof(long));
                    break;
                  case FAST32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fast32i), sizeof(int));
                    break;
                  case FAST64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fast64i), sizeof(long));      
                    break;
                  case FNV32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fnv32_1i), sizeof(int)); 
                    break;
                  case FNV32A:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fnv32a_1i), sizeof(int)); 
                    break;
                  case FNV64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fnv64_1i), sizeof(long));
                    break;
                  case FNV64A:
                    wf.write(reinterpret_cast<char*>(&hregister[0].fnv64a_1i), sizeof(long));
                    break;
                  case HW64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].hw64i), sizeof(long));
                    break;
                  case MD2s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].md2i), hashblocksize); 
                    break;
                  case MD4s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].md4i), hashblocksize); 
                    break;
                  case MD5s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].md5i), hashblocksize);
                    break;
                  case MD6:
                    wf.write(reinterpret_cast<char*>(&hregister[0].md6i), hashblocksize);
                    break;
                  case MD62:
                    wf.write(reinterpret_cast<char*>(&hregister[0].md62i), hashblocksize);
                    break;
                  case MET641:
                    wf.write(reinterpret_cast<char*>(&hregister[0].met641i), sizeof(hregister[0].met641i));
                    break;
                  case MET642:
                    wf.write(reinterpret_cast<char*>(&hregister[0].met642i), sizeof(hregister[0].met642i));
                    break;
                  case MX3:
                    wf.write(reinterpret_cast<char*>(&hregister[0].mx3i), hashblocksize);
                    break;
                  case PNG:
                    wf.write(reinterpret_cast<char*>(&hregister[0].png64i), hashblocksize);
                    break;
                  case RIPE160:
                    wf.write(reinterpret_cast<char*>(&hregister[0].ripe160i), hashblocksize);
                    break;
                  case SEA:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sea64i), sizeof(long));
                    break;
                  case SIP64: 
                    wf.write(reinterpret_cast<char*>(&hregister[0].siphash64i), sizeof(long));
                    break;
                  case SHA164:
                    uint8_t sha164[8];
                    memcpy(sha164, hregister[0].sha1i, blocksize);
                    wf.write(reinterpret_cast<char*>(&sha164), hashblocksize);
                    break;
                  case SHA1128:
                    uint8_t sha1128[16];
                    memcpy(sha1128, hregister[0].sha1i, blocksize);
                    wf.write(reinterpret_cast<char*>(&sha1128), hashblocksize);
                    break;
                  case SHA1s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sha1i), hashblocksize);
                    break;
                  case SHA256s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sha256i), hashblocksize);
                    break;
                  case SHA384s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sha384i), hashblocksize);
                    break;
                  case SHA512s:
                    wf.write(reinterpret_cast<char*>(&hregister[0].sha512i), hashblocksize);
                    break;
                  case SPK32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].spooky32i), sizeof(int));
                    break;
                  case SPK64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].spooky64i), sizeof(long));
                    break;
                  case XXH32:
                    wf.write(reinterpret_cast<char*>(&hregister[0].xxhash32i), sizeof(int));
                    break;
                  case XXH64:
                    wf.write(reinterpret_cast<char*>(&hregister[0].xxhash64i), sizeof(long));
                    break;
                  case WP:
                    wf.write(reinterpret_cast<char*>(&hregister[0].whp512i), hashblocksize);
                    break;
                  case WYH:
                    wf.write(reinterpret_cast<char*>(&hregister[0].wyhash64i), sizeof(long));
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }
    
    // setFileHashList
    // this sets the file hash list signatures for a file
    void setFileHashList(std::string& filename) {
          
          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    //city64i = cityhash64_with_seed(byteblock, blocksize, city64seed);
                    hregister[0].city64i = calculateCityhashFile((char *) filename.c_str(), hregister[0].city64seed);
                    break;
                  case CRC32:
                    //hregister[0].crc64i = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                    break;
                  case CRC64:
                   //crc64i = CRC::Calculate(byteblock, blocksize, CRC::CRC_32());
                  case FAST32:
                    //hregister[0].fast32i = fasthash32(byteblock, blocksize, hregister[0].fast32seed);
                    break;
                  case FAST64:
                    //hregister[0].fast64i = fasthash64(byteblock, blocksize, hregister[0].fast64seed);
                    break;
                  case FNV32:
                    //hregister[0].fnv32_1i = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    break;
                  case FNV32A:
                    //hregister[0].fnv32a_1i = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    break;
                  case FNV64:
                    //hregister[0].fnv64_1i = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    break;
                  case FNV64A:
                    //hregister[0].fnv64a_1i = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    break;
                  case HW64:
                    //hregister[0].hw64i = HighwayHash64(byteblock, blocksize, hregister[0].hw64key);
                    break;
                  case MD2s:
                    //md2(byteblock,(size_t)blocksize,md2i);
                    calculateMD2((char *) filename.c_str(), hregister[0].md2i);
                    break;
                  case MD4s:
                    //MD4(byteblock,(long)blocksize,md4i);
                    calculateMD4((char *) filename.c_str(), hregister[0].md4i);
                    break;
                  case MD5s:
                    //MD5(byteblock,(long)blocksize,md5i);
                    calculateMD5((char *) filename.c_str(), hregister[0].md5i);
                    break;
                  case MD6:
                    //md6_hash(160, byteblock,(uint64_t)(blocksize*8), hregister[0].md6i);
                    break;
                  case MD62:
                    //md6_full_hash(160, byteblock,(uint64_t)(blocksize*8), hregister[0].md62key,16,md6_default_L, 4, hregister[0].md62i);
                    break;
                  case MET641:
                    //metrohash64_1(byteblock, (uint64_t) blocksize, met641seed, met641i);
                    calculateMetro64_1((char *) filename.c_str(), hregister[0].met641i);
                    break;
                  case MET642:
                    //metrohash64_2(byteblock, (uint64_t) blocksize, met642seed, met642i);
                    calculateMetro64_2((char *) filename.c_str(), hregister[0].met642i);
                    break;
                  case MX3:
                    // hash(reinterpret_cast<char*>(&mx3i), hashblocksize);
                    break;
                  case PNG:
                    //png64i = pengyhash(byteblock, (size_t) blocksize, hregister[0].png64seed);
                    break;
                  case RIPE160:
                    //RIPEMD160(byteblock, blocksize, ripe160i);
                    calculateRipe160((char *) filename.c_str(), hregister[0].ripe160i);
                    break;
                  case SEA:
                    //sea64i = seahash((const char*)byteblock, blocksize, hregister[0].sea64seed);
                    break;
                  case SIP64: 
                    //siphash64i = siphash24(byteblock, blocksize, hregister[0].sipkey);
                    break;
                  case SHA164:
                    calculateSHA1((char *) filename.c_str(), hregister[0].sha1i);
                    break;
                  case SHA1128:
                    calculateSHA1((char *) filename.c_str(), hregister[0].sha1i);
                    break;
                  case SHA1s:
                    calculateSHA1((char *) filename.c_str(), hregister[0].sha1i);
                    break;
                  case SHA256s:
                    //SHA256(byteblock, blocksize, sha256i);
                    calculateSHA256((char *) filename.c_str(), hregister[0].sha256i);
                    break;
                  case SHA384s:
                    //SHA384(byteblock, blocksize, sha384i);
                    calculateSHA384((char *) filename.c_str(), hregister[0].sha384i);
                    break;
                  case SHA512s:
                    //SHA512(byteblock, blocksize, sha512i);
                    calculateSHA512((char *) filename.c_str(), hregister[0].sha512i);
                    break;
                  case SPK32:
                    //spooky32i = SpookyHash::Hash32(byteblock, blocksize, hregister[0].spookyseed32);
                    break;
                  case SPK64:
                    //spooky64i = SpookyHash::Hash64(byteblock, blocksize, hregister[0].spookyseed64);
                    break;
                  case XXH32:
                    //xxhash32i = XXHash32::hash(byteblock, blocksize, hregister[0].xxseed32);
                    break;
                  case XXH64:
                    //xxhash64i = XXHash64::hash(byteblock, blocksize, hregister[0].xxseed64);
                    break;
                  case WP:
                    // TODO
                    break;
                  case WYH:
                    //wyhash64i = wyhash(byteblock, blocksize, hregister[0].wyseed64, (const uint64_t*) hregister[0].wysecret64);
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }


    // setByteBlockHashList
    // this sets the block hash list signatures for an input byte block
    void setByteBlockHashList(unsigned char *byteblock, int blocksize) {

          int hashblocksize = 0;
          for(auto hash  : hashlistvt[HASHBLOCK]) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                    hregister[0].city64i = cityhash64_with_seed(byteblock, blocksize, hregister[0].city64seed);
                    break;
                  case CRC32:
                    hregister[0].crc32i = crc32(hregister[0].crc32seed, byteblock, blocksize);
                    break;
                  case CRC64:
                    hregister[0].crc64i = crc64(hregister[0].crc64seed, byteblock, blocksize);
                    break;
                  case FAST32:
                    hregister[0].fast32i = fasthash32(byteblock, blocksize, hregister[0].fast32seed);
                    break;
                  case FAST64:
                    hregister[0].fast64i = fasthash64(byteblock, blocksize, hregister[0].fast64seed);
                    break;
                  case FNV32:
                    hregister[0].fnv32_1i = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    break;
                  case FNV32A:
                    hregister[0].fnv32a_1i = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    break;
                  case FNV64:
                    hregister[0].fnv64_1i = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    break;
                  case FNV64A:
                    hregister[0].fnv64a_1i = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    break;
                  case HW64:
                    hregister[0].hw64i = HighwayHash64(byteblock, blocksize, hregister[0].hw64key);
                    break;
                  case MD2s:
                    md2(byteblock,( size_t) blocksize, hregister[0].md2i);
                    break;
                  case MD4s:
                    MD4(byteblock, (long) blocksize, hregister[0].md4i);
                    break;
                  case MD5s:
                    MD5(byteblock, (long) blocksize, hregister[0].md5i);
                    break;
                  case MD6:
                    md6_hash(160, byteblock, (uint64_t)(blocksize*8),hregister[0].md6i);
                    break;
                  case MD62:
                    md6_full_hash(160, byteblock, (uint64_t)(blocksize*8), hregister[0].md62key, 16, md6_default_L, 4, hregister[0].md62i);
                    break;
                  case MET641:
                    metrohash64_1(byteblock, (uint64_t) blocksize, hregister[0].met641seed, hregister[0].met641i);
                    break;
                  case MET642:
                    metrohash64_2(byteblock, (uint64_t) blocksize, hregister[0].met642seed, hregister[0].met642i);
                    break;
                  case MX3:
                    hregister[0].mx3i = mx3::hash(byteblock, blocksize, hregister[0].mx3seed);
                    break;
                  case PNG:
                    hregister[0].png64i = pengyhash(byteblock, (size_t) blocksize, hregister[0].png64seed);
                    break;
                  case RIPE160:
                    RIPEMD160(byteblock, blocksize, hregister[0].ripe160i);
                    break;
                  case SEA:
                    hregister[0].sea64i = seahash((const char*)byteblock, blocksize, hregister[0].sea64seed);
                    break;
                  case SIP64: 
                    hregister[0].siphash64i = siphash24(byteblock, blocksize, hregister[0].sipkey);
                    break;
                  case SHA164:
                    SHA1(byteblock, blocksize, hregister[0].sha1i);
                    break;
                  case SHA1128:
                    SHA1(byteblock, blocksize, hregister[0].sha1i);
                    break;
                  case SHA1s:
                    SHA1(byteblock, blocksize, hregister[0].sha1i);
                    break;
                  case SHA256s:
                    SHA256(byteblock, blocksize, hregister[0].sha256i);
                    break;
                  case SHA384s:
                    SHA384(byteblock, blocksize, hregister[0].sha384i);
                    break;
                  case SHA512s:
                    SHA512(byteblock, blocksize, hregister[0].sha512i);
                    break;
                  case SPK32:
                    hregister[0].spooky32i = SpookyHash::Hash32(byteblock, blocksize, hregister[0].spookyseed32);
                    break;
                  case SPK64:
                    hregister[0].spooky64i = SpookyHash::Hash64(byteblock, blocksize, hregister[0].spookyseed64);
                    break;
                  case XXH32:
                    hregister[0].xxhash32i = XXHash32::hash(byteblock, blocksize, hregister[0].xxseed32);
                    break;
                  case XXH64:
                    hregister[0].xxhash64i = XXHash64::hash(byteblock, blocksize, hregister[0].xxseed64);
                    break;
                  case WP:
                    WHIRLPOOL(byteblock, blocksize, hregister[0].whp512i);
                    break;
                  case WYH:
                    hregister[0].wyhash64i = wyhash(byteblock, blocksize, hregister[0].wyseed64, (const uint64_t*) hregister[0].wysecret64);
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
                    hregister[0].city64o = cityhash64_with_seed(byteblock, blocksize, hregister[0].city64seed);
                    if (hregister[0].city64i != hregister[0].city64o) return false;
                    break;
                  case CRC32:
                    hregister[0].crc32o = crc32(hregister[0].crc32seed, byteblock, blocksize);
                    if (hregister[0].crc32i != hregister[0].crc32o) return false;
                    break;
                  case CRC64:
                    hregister[0].crc64o = crc64(hregister[0].crc64seed, byteblock, blocksize);
                    if (hregister[0].crc64i != hregister[0].crc64o) return false;
                    break;
                  case FAST32:
                    hregister[0].fast32o = fasthash32(byteblock, blocksize, hregister[0].fast32seed);
                    if (hregister[0].fast32i != hregister[0].fast32o) return false;
                    break;
                  case FAST64:
                    hregister[0].fast64o = fasthash64(byteblock, blocksize, hregister[0].fast64seed);
                    if (hregister[0].fast64i != hregister[0].fast64o) return false;
                    break;
                  case FNV32:
                    hregister[0].fnv32_1o = fnv_32_buf(byteblock, blocksize, FNV1_32_INIT);
                    if (hregister[0].fnv32_1i != hregister[0].fnv32_1o) return false;
                    break;
                  case FNV32A:
                    hregister[0].fnv32a_1o = fnv_32a_buf(byteblock, blocksize, FNV1_32A_INIT);
                    if (hregister[0].fnv32a_1i != hregister[0].fnv32a_1o) return false;
                    break;
                  case FNV64:
                    hregister[0].fnv64_1o = fnv_64_buf(byteblock, blocksize, FNV1_64_INIT);
                    if (hregister[0].fnv64_1i != hregister[0].fnv64_1o) return false;
                    break;
                  case FNV64A:
                    hregister[0].fnv64a_1o = fnv_64a_buf(byteblock, blocksize, FNV1A_64_INIT);
                    if (hregister[0].fnv64a_1i != hregister[0].fnv64a_1o) return false;
                    break;
                  case HW64:
                    hregister[0].hw64o = HighwayHash64(byteblock, blocksize, hregister[0].hw64key);
                    if (hregister[0].hw64i != hregister[0].hw64o) return false;
                    break;
                  case MD2s:
                    md2(byteblock,(size_t)blocksize, hregister[0].md2o);
                    if (memcmp(hregister[0].md2i, hregister[0].md2o, 16) != 0) return false;
                    break;
                  case MD4s:
                    MD4(byteblock,(long)blocksize,hregister[0].md4o);
                    if (memcmp(hregister[0].md4i, hregister[0].md4o, 16) != 0) return false;
                    break;
                  case MD5s:
                    MD5(byteblock,(long)blocksize,hregister[0].md5o);
                    if (memcmp(hregister[0].md5i, hregister[0].md5o, 16) != 0) return false;
                    break;
                  case MD6:
                    md6_hash(160, byteblock,(uint64_t)(blocksize*8),hregister[0].md6o);
                    if (memcmp(hregister[0].md6i, hregister[0].md6o, 20) != 0) return false;
                    break;
                  case MD62:
                    md6_full_hash(160, byteblock,(uint64_t)(blocksize*8), hregister[0].md62key,16,md6_default_L, 4, hregister[0].md62o);
                    if (memcmp(hregister[0].md62i, hregister[0].md62o, 20) != 0) return false;
                    break;
                  case MET641:
                    metrohash64_1(byteblock, (uint64_t) blocksize, hregister[0].met641seed, hregister[0].met641o);
                    if (memcmp(hregister[0].met641i, hregister[0].met641o, 8) != 0) return false;
                    break;
                  case MET642:
                    metrohash64_2(byteblock, (uint64_t) blocksize, hregister[0].met642seed, hregister[0].met642o);
                    if (memcmp(hregister[0].met642i, hregister[0].met642o, 8) != 0) return false;
                    break;
                  case MX3:
                    hregister[0].mx3o = mx3::hash(byteblock, blocksize, hregister[0].mx3seed);
                    if (hregister[0].mx3i != hregister[0].mx3o) return false;
                    break;
                  case PNG:
                    hregister[0].png64o = pengyhash(byteblock, (size_t) blocksize, hregister[0].png64seed);
                    if (hregister[0].png64i != hregister[0].png64o) return false;
                    break;
                  case RIPE160:
                    RIPEMD160(byteblock, blocksize, hregister[0].ripe160o);
                    if (memcmp(hregister[0].ripe160i, hregister[0].ripe160o, 20) != 0) return false;
                    break;
                  case SEA:
                    hregister[0].sea64o = seahash((const char*) byteblock, blocksize, hregister[0].sea64seed);
                    if (hregister[0].sea64i != hregister[0].sea64o) return false;
                    break;
                  case SIP64:
                    hregister[0].siphash64o = siphash24(byteblock, blocksize, hregister[0].sipkey);
                    if (hregister[0].siphash64i != hregister[0].siphash64o) return false;
                    break;
                  case SHA164:
                    SHA1(byteblock, blocksize, hregister[0].sha1o);
                    if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 8) != 0) return false;
                    break;
                  case SHA1128:
                    SHA1(byteblock, blocksize, hregister[0].sha1o);
                    if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 16) != 0) return false;
                    break;
                  case SHA1s:
                    SHA1(byteblock, blocksize, hregister[0].sha1o);
                    if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 20) != 0) return false;
                    break;
                  case SHA256s:
                    SHA256(byteblock, blocksize, hregister[0].sha256o);
                    if (memcmp(hregister[0].sha256i, hregister[0].sha256o, 32) != 0) return false;
                    break;
                  case SHA384s:
                    SHA384(byteblock, blocksize, hregister[0].sha384o);
                    if (memcmp(hregister[0].sha384i, hregister[0].sha384o, 48) != 0) return false;
                    break;
                  case SHA512s:
                    SHA512(byteblock, blocksize, hregister[0].sha512o);
                    if (memcmp(hregister[0].sha512i, hregister[0].sha512o, 64) != 0) return false;
                    break;
                  case SPK32:
                    hregister[0].spooky32o = SpookyHash::Hash32(byteblock, blocksize, hregister[0].spookyseed32);
                    if (hregister[0].spooky32i != hregister[0].spooky32o) return false;
                    break;
                  case SPK64:
                    hregister[0].spooky64o = SpookyHash::Hash64(byteblock, blocksize, hregister[0].spookyseed64);
                    if (hregister[0].spooky64i != hregister[0].spooky64o) return false;
                    break;
                  case XXH32:
                    hregister[0].xxhash32o = XXHash32::hash(byteblock, blocksize, hregister[0].xxseed32);
                    if (hregister[0].xxhash32i != hregister[0].xxhash32o) return false;
                    break;
                  case XXH64:
                    hregister[0].xxhash64o = XXHash64::hash(byteblock, blocksize, hregister[0].xxseed64);
                    if (hregister[0].xxhash64i != hregister[0].xxhash64o) return false;
                    break;
                  case WP:
                    WHIRLPOOL(byteblock, blocksize, hregister[0].whp512o);
                    if (memcmp(hregister[0].whp512i, hregister[0].whp512o, 64) != 0) return false;
                    break;
                  case WYH:
                    hregister[0].wyhash64o = wyhash(byteblock, blocksize, hregister[0].wyseed64, (const uint64_t*) hregister[0].wysecret64);
                    if (hregister[0].wyhash64i != hregister[0].wyhash64o) return false;
                    break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

          return true;
    }

    // display the hash list keys
    std::string displayHLhashKeys() {
        // clear the string stream
        ss.str(std::string());

        int i = 0;
        int hashblocksize = 0;
        for(auto hash  : hashlistvt[HASHBLOCK]) {

              hashblocksize = std::get<2>(hash); // should be the key size     
              // ss << "hash id " << std::get<0>(hash) << " " << " name " << std::get<1>(hash) << " hashblocksize " << std::to_string(hashblocksize) << " ";

              switch(std::get<0>(hash)) {
                  case CIT64:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].city64seed) << " ";
                     break;
                  case CRC32:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].crc32seed) << " ";
                     break;
                  case CRC64:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].crc64seed) << " ";
                     break;
                  case FAST32:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].fast32seed) << " ";
                     break;
                 case FAST64:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].fast64seed) << " ";
                     break;
                  case FNV32:
                     break;
                  case FNV32A:
                     break;
                  case FNV64:
                     break;
                  case FNV64A:
                     break;
                  case HW64:
                     ////   addHashToDisplayStream(hregister[0].hw64key, 4); // TODO
                     ss << std::get<1>(hash) << " keys ";
                     for(i=0; i < 4; i++) { ss << std::to_string(hregister[0].hw64key[i]) << " "; }
                     break;
                  case MD2s:
                     break;
                  case MD4s:                     
                     break;
                  case MD5s:
                     break;
                  case MD6:
                     //addHashToDisplayStream(hregister[0].md6i, hashblocksize);
                     break;
                  case MD62:
                     //addHashToDisplayStream(hregister[0].md62i, hashblocksize);
                     break;
                  case MET641:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].met641seed) << " ";
                     break;
                  case MET642:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].met642seed) << " ";
                     break;
                  case MX3:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].mx3seed) << " ";
                     break;
                  case PNG:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].png64seed) << " ";
                     break;
                  case RIPE160:
                     break;
                  case SEA:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].sea64seed) << " ";
                     break;
                  case SIP64:
                     ss << std::get<1>(hash) << " keys ";
                     addHashToDisplayStream((unsigned char*) hregister[0].sipkey, 16);
                     break;
                  case SHA164:
                     break;
                  case SHA1128:
                     break;
                  case SHA1s:
                     break;
                  case SHA256s:
                     break;
                  case SHA384s:
                     break;
                  case SHA512s:
                     break;
                  case SPK32:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].spookyseed32) << " ";
                     break;
                  case SPK64:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].spookyseed64) << " ";
                     break;
                  case XXH32:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].xxseed32) << " ";
                     break;
                  case XXH64:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].xxseed64) << " ";
                     break;
                  case WP:
                     break;
                  case WYH:
                     ss << std::get<1>(hash) << " keys ";
                     ss << std::to_string(hregister[0].wyseed64) << " secret ";
                     // addHashToDisplayStream(hregister[0].wysecret64, 16);
                     int i = 0;
                     for(i=0; i < 16; ++i)
                        ss << std::to_string(hregister[0].wysecret64[i]) << " ";
                        ss << " ";
                     break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
         }
         hashlist = ss.str();
         return hashlist;         
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
                     ss << std::to_string(hregister[0].city64i) << " ";
                     break;
                  case CRC32:
                     ss << std::to_string(hregister[0].crc32i) << " ";
                     break;
                  case CRC64:
                     ss << std::to_string(hregister[0].crc64i) << " ";
                     break;
                  case FAST32:
                     ss << std::to_string(hregister[0].fast32i) << " ";
                     break;
                 case FAST64:
                     ss << std::to_string(hregister[0].fast64i) << " ";
                     break;
                  case FNV32:
                     ss << std::to_string(hregister[0].fnv32_1i) << " ";
                     break;
                  case FNV32A:
                     ss << std::to_string(hregister[0].fnv32a_1i) << " ";
                     break;
                  case FNV64:
                     ss << std::to_string(hregister[0].fnv64_1i) << " ";
                     break;
                  case FNV64A:
                     ss << std::to_string(hregister[0].fnv64a_1i) << " ";
                     break;
                  case HW64:
                     ss << std::to_string(hregister[0].hw64i) << " ";
                     break;
                  case MD2s:
                     addHashToDisplayStream(hregister[0].md2i, hashblocksize);
                     break;
                  case MD4s:
                     addHashToDisplayStream(hregister[0].md4i, hashblocksize);
                     break;
                  case MD5s:
                     addHashToDisplayStream(hregister[0].md5i, hashblocksize);
                     break;
                  case MD6:
                     addHashToDisplayStream(hregister[0].md6i, hashblocksize);
                     break;
                  case MD62:
                     addHashToDisplayStream(hregister[0].md62i, hashblocksize);
                     break;
                  case MET641:
                     addHashToDisplayStream(hregister[0].met641i, hashblocksize);
                     break;
                  case MET642:
                     addHashToDisplayStream(hregister[0].met642i, hashblocksize);
                     break;
                  case MX3:
                     ss << std::to_string(hregister[0].mx3i) << " ";
                     break;
                  case PNG:
                     ss << std::to_string(hregister[0].png64i) << " ";
                     break;
                  case RIPE160:
                     addHashToDisplayStream(hregister[0].ripe160i, hashblocksize);
                     break;
                  case SEA:
                     ss << std::to_string(hregister[0].sea64i) << " ";
                     break;
                  case SIP64:
                     ss << std::to_string(hregister[0].siphash64i) << " ";
                     break;
                  case SHA164:
                     addHashToDisplayStream(hregister[0].sha1i, hashblocksize);
                     break;
                  case SHA1128:
                     addHashToDisplayStream(hregister[0].sha1i, hashblocksize);
                     break;
                  case SHA1s:
                     addHashToDisplayStream(hregister[0].sha1i, hashblocksize);
                     break;
                  case SHA256s:
                     addHashToDisplayStream(hregister[0].sha256i, hashblocksize); 
                     break;
                  case SHA384s:
                     addHashToDisplayStream(hregister[0].sha384i, hashblocksize);
                     break;
                  case SHA512s:
                     addHashToDisplayStream(hregister[0].sha512i, hashblocksize);
                     break;
                  case SPK32:
                     ss << std::to_string(hregister[0].spooky32i) << " ";
                     break;
                  case SPK64:
                     ss << std::to_string(hregister[0].spooky64i) << " ";
                     break;
                  case XXH32:
                     ss << std::to_string(hregister[0].xxhash32i) << " ";
                     break;
                  case XXH64:
                     ss << std::to_string(hregister[0].xxhash64i) << " ";
                     break;
                  case WP:
                     addHashToDisplayStream(hregister[0].whp512i, hashblocksize);
                     break;
                  case WYH:
                     ss << std::to_string(hregister[0].wyhash64i) << " ";
                     break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
         }
         hashlist = ss.str();
         return hashlist;         
    }

    // add a hash list to the hash list stream
    void addHashToDisplayStream(unsigned char *digest, int hashblocksize) {
         int i = 0;
         for(i=0; i < hashblocksize; ++i)
               ss << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (int)digest[i];
         ss << " ";
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
         int blocksize    = 0; 
         int blockkeysize = 0; 

         hlss << std::left << std::setw(12) << "Number";
         hlss << std::left << std::setw(13) << "Hash ID";
         hlss << std::left << std::setw(12) << "Hash Name ";
         hlss << std::left << std::setw(12) << "Blocksize";
         hlss << std::left << std::setw(12) << "Blockkeysize" << std::endl;

         int i = 1;
         for(auto val  : hashlistvt[type]) {
             hlss << std::left << std::setw(12) << i;
             hlss << std::left << std::setw(13) << std::get<0>(val);
             hlss << std::left << std::setw(12) << std::get<1>(val);
             hlss << std::left << std::setw(12) << std::get<2>(val);
             hlss << std::left << std::setw(11) << std::to_string(mdHashlist[std::get<0>(val)-1].keysize) << std::endl;
       
             blocksize += std::get<2>(val);
             blockkeysize += mdHashlist[std::get<0>(val)-1].keysize;
             i++;
         }
         
         hlss << std::right << std::setw(30) << "Total";
         hlss << std::right << std::setw(7)  << " ";
         hlss << std::left << std::setw(12) << blocksize;
         hlss <<  blockkeysize << std::endl;

         return hlss.str(); 
    }

    // get the vector list string of hash names
    // ie aes8:sha1:md5
    // it's easier to store the hash list as a string since the id might change
    // ie aes8:sha1:md5-aes8-keylist
    std::string getHLvectorsStringNames(int type) {
         // clear the hlss string stream
         hlss.str(std::string());

         int block        = 1;
         int hashListsize = 0;
         hashListsize = hashlistvt[type].size();

         for(auto val  : hashlistvt[type]) {
             hlss << std::get<1>(val); 
             if (block < hashListsize) hlss << ":";
             block++;
         }

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

    // calculate the block key size for the correct hashlist type
    // this returns the sum of the hash list vector block sizes
    int calcBlockKeySize(int type) {

        int signum = 0;
        int hashblocksize = 0;
        int sumhashkeyblocksize = 0;
        for(auto hash  : hashlistvt[type]) {
              signum = std::get<0>(hash) - 1;
              hashblocksize = mdHashlist[signum].keysize;
              sumhashkeyblocksize += hashblocksize;
        }

        return sumhashkeyblocksize;

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
