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
#include "mdIncrementKey.h"

// TODO should add a keylist type for setting the keylist separately from the hash block or file list
enum htype {HASHFILE,HASHBLOCKGROUP,HASHBLOCK,HASHLAST};
// signatures enum list
enum signatures {FIRST, CIT64, CRC32, CRC64, EDN224, FAST32, FAST64, FNV32, FNV32A, FNV64, FNV64A, HAS160, HW40, HW48, HW64, MD2s, MD4s, MD5s, MD6, 
                MD62, MET641, MET642, MX3, PNG, RIPE128, RIPE160, RIPE256, RIPE320, SEA, SIP32, SIP322, SIP40, SIP48, SIP64, SIP128, 
                SHA164, SHA1128, SHA1s, SHA256s, SHA384s, SHA512s, 
                SPK32, SPK64, TIGER192, XXH32, XXH64, WP, WYH, LAST};

// key incrementer
enum keyincrementer {NOINC, DEC, INC};                

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
    {4,  "edn224",   "EDON-R 224",            false,    28,     0},
    {5,  "fast32",   "Fasthash 32",           true,     4,      4},
    {6,  "fast64",   "Fasthash 64",           true,     8,      8},
    {7,  "fnv32",    "FNV-1  32",             false,    4,      0},
    {8,  "fnv32a",   "FNV-1a 32",             false,    4,      0},
    {9,  "fnv64",    "FNV-1  64",             false,    8,      0},
    {10, "fnv64a",   "FNV-1a 64",             false,    8,      0},
    {11, "has160",   "HAS 160",               false,    20,     0},
    {12, "hw40",     "Highway Hash 40",       true,     5,      32},
    {13, "hw48",     "Highway Hash 48",       true,     6,      32},
    {14, "hw64",     "Highway Hash 64",       true,     8,      32},
    {15, "md2",      "MD2",                   false,    16,     0},
    {16, "md4",      "MD4",                   false,    16,     0},
    {17, "md5",      "MD5",                   false,    16,     0},
    {18, "md6",      "MD6",                   false,    20,     0},
    {19, "md62",     "MD6 Quicker",           true,     20,     0},
    {20, "met641",   "Metro Hash 64 v1",      true,     8,      4},
    {21, "met642",   "Metro Hash 64 v2",      true,     8,      4},
    {22, "mx3",      "MX3",                   true,     8,      8},
    {23, "png",      "Pengyhash 64",          true,     8,      4},
    {24, "ripe128",  "Ripe MD 128",           false,    16,     0},
    {25, "ripe160",  "Ripe MD 160",           false,    20,     0},
    {26, "ripe256",  "Ripe MD 256",           false,    32,     0},
    {27, "ripe320",  "Ripe MD 320",           false,    40,     0},
    {28, "sea64",    "Seahash 64",            true,     8,      8},
    {29, "sip32",    "Siphash 32",            true,     4,      16},
    {30, "sip322",   "Siphash 32b",           true,     4,      16},
    {31, "sip40",    "Siphash 40",            true,     5,      16},
    {32, "sip48",    "Siphash 48",            true,     6,      16},
    {33, "sip64",    "Siphash 64",            true,     8,      16},
    {34, "sip128",   "Siphash 128",           true,     16,     16},
    {35, "sha1_64",  "SHA1 64",               false,    8,      0},
    {36, "sha1_128", "SHA1 128",              false,    16,     0},
    {37, "sha1",     "SHA1",                  false,    20,     0},
    {38, "sha256",   "SHA 256",               false,    32,     0},
    {39, "sha384",   "SHA 384",               false,    48,     0},
    {40, "sha512",   "SHA 512",               false,    64,     0},
    {41, "spk32",    "Spooky 32",             true,     4,      4},
    {42, "spk64",    "Spooky 64",             true,     8,      8},
    {43, "tgr",      "Tiger 192",             false,    24,     0},
    {44, "xxh32",    "xxHash32",              true,     4,      4},
    {45, "xxh64",    "xxHash64",              true,     8,      8},
    {46, "whp",      "Whirlpool",             false,    64,     0},
    {47, "wy64",     "WYhash 64",             true,     8,      48},
    {48, "last",     "Unused Signature",      false,    8,      0}
};


class mdHashContextList 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    // blocknumber is for making a per block signature key
    // need to add a get and set for this
    // TODO it can increment or decrement the signature key (possibly integer) to make the block keys unique per block
    // It adds or increments/decrements the blocknumber to the hash key
    // It can also set a pseudo random number generator seed to set each block with a different signature key.
    long blocknumber = 0; 
    int incrementKey = NOINC;
    // set the hashlistsize to the last signatures enum value - 1
    int hashlistsize = LAST - 1;
    // hash list vectors
    // number, hashname, blocksize, keysize, random keyset boolean true or false // default to false for default or randomized key
    std::vector<std::tuple<int,std::string,int,int,bool>> hashlistvt;
    // keylist vector
    // TODO Remove this
    // TODO Need to add the commandline user specified keys
    std::vector<std::tuple<int,std::string,int,int,bool>> hashlistvtkey;
    // hash map
    std::map<std::string, int> hclmap;
    // hash context list type
    int hashtype = HASHBLOCK;
public:
    // hash results registers
    // public for struct to struct copying
    hash_list hregister [2];
    // string stream variables
    std::stringstream bhlist;
    std::stringstream ss;
    std::stringstream hlss;
    std::string hashlist;

    // alternate intializer set with the default hashblock type
    mdHashContextList() {

        // add the struct hash registry to the hclmap
        // could move this to a separate function
        int hsnum = 0;
        for (hsnum = 0; hsnum < hashlistsize; hsnum++) {
             hclmap[mdHashlist[hsnum].name] = hsnum;
        }
    }


    // initialize mdHashContextList with the type
    mdHashContextList(int type) {

        // set the hash type
        hashtype = type;

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
    void setVectorHL(std::vector<int> &v)
    {
          // need to check type size
          // hashlistvt = setVectorTuple(v);
          // set the keylist to default to the hashlistvt
          // hashlistvtkey = hashlistvt;

          // Deduplicate or unique the hashlist input vector v and preserve the order
          use_std_hash_remove_dup(v);

          // create a hashlist tuple vector
          std::tuple<int,std::string,int,int,bool> hashTuple;
          for(int val  : v) {
              if (val <= hashlistsize && val > 0) {
                // hashTuple = make_tuple(val, mdHashlist[val-1].name, mdHashlist[val-1].blocksize);
                hashTuple = make_tuple(val, mdHashlist[val-1].name, mdHashlist[val-1].blocksize, mdHashlist[val-1].keysize, true);

                // need to check type size
                hashlistvt.push_back(hashTuple);
              }
          }

    }

    // set the hash list key tuple
    // this should convert a boolean bitstream to set the default or random key flags in the hashlist for files or block groups or blocks
    // true it's random and false it's default
    // this is optional and allows the keylist to be set 
    // make this a bitstream arg 
    // store this as a bitstream parameter of booleans
    // this will turn on or off the key randomization for a hash and use the signature list provided
    // --randbh2=101110001 or --randfh2=10110011111
    // byteblock bitstream and bitstream starting position
    void setVectorKeyHL(std::vector<int> &v)
    {
          // need to check type size of vector tuple so it's not zero
          // I'm using a unified hashlist vector with a key list randomized bool
          // true it's random and false it's default
          // while hashlistvt 
          // if number = input vector number set it to true??? others to false??? if it's not in the list
          // I think the true or false would only apply to the read keylist or write keylist and generate the hash list string
          for(auto hash  : hashlistvt) {

            // std::get<4>(hash) = keyval;

          }  

    }

    // setVectorHLstring
    // converts a string list to a int vector and then to the vector tuple
    // the delimeter is ':' currently
    // ie hash string cit64:crc32:fast32
    // ie hash string cit64:crc32:fast32:fast64:fnv32a:fnv64:md2:md4:met641:met642:sha512:whp
    void setVectorHLstring(std::string hashlist) {
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
          setVectorHL(v);
    }

    // load the Signature keys from a file
    void readKeyList(std::ifstream &rf) {
          int hashblocksize = 0;
          bool randhashkey  = true;

          for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);
              randhashkey   = std::get<4>(hash); // if this is true it reads the hash key and if it's false it skips it

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
                  case EDN224:
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
                  case HAS160:
                      break;  
                  case HW40:
                      rf.read(reinterpret_cast<char*>(&hregister[0].hw40key), 32);
                      break;  
                  case HW48:
                      rf.read(reinterpret_cast<char*>(&hregister[0].hw48key), 32);
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
                  case RIPE128:
                      break;  
                  case RIPE160:
                      break;
                  case RIPE256:
                      break;  
                  case RIPE320:
                      break; 
                  case SEA:
                      rf.read(reinterpret_cast<char*>(&hregister[0].sea64seed), sizeof(uint64_t));
                      break;
                  case SIP32: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey32), 16);
                      break;  
                  case SIP322: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey322), 16);
                      break;  
                  case SIP40: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey40), 16);
                      break;       
                  case SIP48: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey48), 16);
                      break;  
                  case SIP64: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey64), 16);
                      break;
                  case SIP128: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].sipkey128), 16);
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
                  case TIGER192:
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
                      rf.read(reinterpret_cast<char*>(&hregister[0].wysecret64), 40);
                      break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }   

    // write the Signature keys to a file
    void writeKeyList(std::ofstream &wf) {
          int hashblocksize = 0;
          bool randhashkey  = true;

          for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);
              randhashkey   = std::get<4>(hash); // if this is true it writes the hash key and if it's false it skips it

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
                  case EDN224:
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
                  case HAS160:
                      break;  
                  case HW40:
                      wf.write(reinterpret_cast<char*>(&hregister[0].hw40key), 32);
                      break;    
                  case HW48:
                      wf.write(reinterpret_cast<char*>(&hregister[0].hw48key), 32);
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
                  case RIPE128:
                      break;  
                  case RIPE160:
                      break;
                  case RIPE256:
                      break;  
                  case RIPE320:
                      break; 
                  case SEA:
                      wf.write(reinterpret_cast<char*>(&hregister[0].sea64seed), sizeof(uint64_t));
                      break;
                  case SIP32: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey32), 16);
                      break;
                  case SIP322: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey322), 16);
                      break; 
                  case SIP40: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey40), 16);
                      break;      
                  case SIP48: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey48), 16);
                      break;                        
                  case SIP64: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey64), 16);
                      break;
                  case SIP128: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].sipkey128), 16);
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
                  case TIGER192:
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
                      wf.write(reinterpret_cast<char*>(&hregister[0].wysecret64), 40);
                      break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }  

    // randomize the Signature keys based on the signature list
    void randomizeKeyList() {
          int hashblocksize = 0;
          bool randhashkey  = true;

          for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);
              randhashkey   = std::get<4>(hash); // if this is true it randomizes the hash key and if it's false it skips it and uses the default

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
                  case EDN224:
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
                  case HAS160:
                      break;  
                  case HW40:
                      genRandomLongBlock(hregister[0].hw40key, 4);
                      break;    
                  case HW48:
                      genRandomLongBlock(hregister[0].hw48key, 4);
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
                  case RIPE128:
                      break;  
                  case RIPE160:
                      break;
                  case RIPE256:
                      break;  
                  case RIPE320:
                      break; 
                  case SEA:
                      hregister[0].sea64seed = randLong(); 
                      break;
                  case SIP32:
                      genRandomUnsignedByteBlock2(hregister[0].sipkey32, 16); 
                      break;
                  case SIP322:
                      genRandomUnsignedByteBlock2(hregister[0].sipkey322, 16); 
                      break;    
                  case SIP40:
                      genRandomUnsignedByteBlock2(hregister[0].sipkey40, 16); 
                      break;    
                  case SIP48:
                      genRandomUnsignedByteBlock2(hregister[0].sipkey48, 16); 
                      break;    
                  case SIP64: 
                      genRandomByteBlock(hregister[0].sipkey64, 16); 
                      break;
                  case SIP128: 
                      genRandomUnsignedByteBlock2(hregister[0].sipkey128, 16);
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
                  case TIGER192:
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
                      genRandomLongBlock(hregister[0].wysecret64, 5);
                      break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

    }    

    // increment the Block Signature keys based on the signature list for the next file block
    // It can either add the block number to the signature key
    // or
    // It can also use a Pseudo Random Number Generator and seed to make each file block signature key different
    //
    // Alternatively it can use a large signature key of 16 bytes or larger like siphash
    // Incrementing for adding
    // Decrementing for subtraction
    // I can use the double mdVersion to specify if it is incrementing or decrementing or a PSG generator
    // It could also add others to increment by 1000 or 1 million or a billions or even different i/d numbers
    int incrementBlockKeyList() {
          int hashblocksize = 0;

          // skip the first block
          // if (blocknumber == 1) return 0;

          for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case SIP40:
                      incrementByteblock(16, hregister[0].sipkey40, blocknumber, incrementKey);
                      // cout << "testing byteblock incrementer st " << std::endl;
                      // printByteblock(hregister[0].sipkey40, 16, true); 
                      break;  
                  case SIP48:
                      incrementByteblock(16, hregister[0].sipkey48, blocknumber, incrementKey);
                      // cout << "testing byteblock incrementer st " << std::endl;
                      // printByteblock(hregister[0].sipkey48, 16, true); 
                      break;   
              }      

          }   

          return 0;

    }   

    // readBlockHashList
    // read the hash list from a ifstream file object
    void readBlockHashList(std::ifstream &rf) { 

          int hashblocksize = 0;
          for(auto hash  : hashlistvt) {
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
                  case EDN224:
                      rf.read(reinterpret_cast<char*>(&hregister[0].edn224i), hashblocksize);
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
                  case HAS160:
                      rf.read(reinterpret_cast<char*>(&hregister[0].has160i), hashblocksize); 
                      break;  
                  case HW40:
                      //rf.read(reinterpret_cast<char*>(&hregister[0].hw40i), hashblocksize);
                      rf.read(reinterpret_cast<char*>(&hregister[0].hw40bi), hashblocksize);
                      break;  
                   case HW48:
                      // rf.read(reinterpret_cast<char*>(&hregister[0].hw40i), hashblocksize);
                      rf.read(reinterpret_cast<char*>(&hregister[0].hw48bi), hashblocksize);
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
                  case RIPE128:  
                      rf.read(reinterpret_cast<char*>(&hregister[0].ripe128i), hashblocksize);
                      break;
                  case RIPE160:
                      rf.read(reinterpret_cast<char*>(&hregister[0].ripe160i), hashblocksize);
                      break;
                  case RIPE256:
                      rf.read(reinterpret_cast<char*>(&hregister[0].ripe256i), hashblocksize);
                      break;  
                  case RIPE320:
                      rf.read(reinterpret_cast<char*>(&hregister[0].ripe320i), hashblocksize);
                      break;    
                  case SEA:
                      rf.read(reinterpret_cast<char*>(&hregister[0].sea64i), sizeof(long));
                      break;
                  case SIP32: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash32i), hashblocksize);
                      break; 
                  case SIP322: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash322i), hashblocksize);
                      break; 
                  case SIP40: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash40i), hashblocksize);
                      break;    
                  case SIP48: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash48i), hashblocksize);
                      break;   
                  case SIP64: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash64i), sizeof(long));
                      break;
                  case SIP128: 
                      rf.read(reinterpret_cast<char*>(&hregister[0].siphash128i), hashblocksize);
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
                  case TIGER192:
                      rf.read(reinterpret_cast<char*>(&hregister[0].tiger192i), hashblocksize);
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
          for(auto hash  : hashlistvt) {
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
                  case EDN224:
                      wf.write(reinterpret_cast<char*>(&hregister[0].edn224i), hashblocksize);
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
                  case HAS160:
                      wf.write(reinterpret_cast<char*>(&hregister[0].has160i), hashblocksize); 
                      break;  
                  case HW40:
                      // wf.write(reinterpret_cast<char*>(&hregister[0].hw40i), hashblocksize);
                      wf.write(reinterpret_cast<char*>(&hregister[0].hw40bi), hashblocksize);
                      break;   
                   case HW48:
                      // wf.write(reinterpret_cast<char*>(&hregister[0].hw40i), hashblocksize);
                      wf.write(reinterpret_cast<char*>(&hregister[0].hw48bi), hashblocksize);
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
                  case RIPE128:  
                      wf.write(reinterpret_cast<char*>(&hregister[0].ripe128i), hashblocksize);
                      break;
                  case RIPE160:
                      wf.write(reinterpret_cast<char*>(&hregister[0].ripe160i), hashblocksize);
                      break;
                  case RIPE256:
                      wf.write(reinterpret_cast<char*>(&hregister[0].ripe256i), hashblocksize);
                      break;  
                  case RIPE320:
                      wf.write(reinterpret_cast<char*>(&hregister[0].ripe320i), hashblocksize);
                      break; 
                  case SEA:
                      wf.write(reinterpret_cast<char*>(&hregister[0].sea64i), sizeof(long));
                      break;
                  case SIP32: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash32i), hashblocksize);
                      break;   
                  case SIP322: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash322i), hashblocksize);
                      break; 
                  case SIP40: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash40i), hashblocksize);
                      break;   
                  case SIP48: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash48i), hashblocksize);
                      break;  
                  case SIP64: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash64i), sizeof(long));
                      break; 
                  case SIP128: 
                      wf.write(reinterpret_cast<char*>(&hregister[0].siphash128i), hashblocksize);
                      break;  
                  case SHA164:
                      uint8_t sha164[8];
                      memcpy(sha164, hregister[0].sha1i, hashblocksize);
                      wf.write(reinterpret_cast<char*>(&sha164), hashblocksize);
                      break;
                  case SHA1128:
                      uint8_t sha1128[16];
                      memcpy(sha1128, hregister[0].sha1i, hashblocksize);
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
                  case TIGER192:
                      wf.write(reinterpret_cast<char*>(&hregister[0].tiger192i), hashblocksize);
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
          for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                      hregister[0].city64i = getFileHashCityhash((char *) filename.c_str(), hregister[0].city64seed);
                      break;
                  case CRC32:
                      hregister[0].crc32i = getFileHashCRC32((char *) filename.c_str(), hregister[0].crc32seed);
                      break;
                  case CRC64:
                      hregister[0].crc64i = getFileHashCRC64((char *) filename.c_str(), hregister[0].crc64seed);
                      break;
                  case EDN224:
                      getFileHashEDONR224((char *) filename.c_str(), hregister[0].edn224i);
                      break;
                  case FAST32:
                      hregister[0].fast32i = getFileHashFast32((char *) filename.c_str(), hregister[0].fast32seed);
                      break;
                  case FAST64:
                      hregister[0].fast64i = getFileHashFast64((char *) filename.c_str(), hregister[0].fast64seed);
                      break;
                  case FNV32:
                      hregister[0].fnv32_1i = getFileHashFNV32((char *) filename.c_str());
                      break;
                  case FNV32A:
                      hregister[0].fnv32a_1i = getFileHashFNV32A((char *) filename.c_str());
                      break;
                  case FNV64:
                      hregister[0].fnv64_1i = getFileHashFNV64((char *) filename.c_str());
                      break;
                  case FNV64A:
                      hregister[0].fnv64a_1i = getFileHashFNV64A((char *) filename.c_str());
                      break;
                  case HAS160:
                      getFileHashHAS160((char *) filename.c_str(), hregister[0].has160i);
                      break;  
                  case HW40:
                      hregister[0].hw40i = getFileHashHW64((char *) filename.c_str(), hregister[0].hw40key);
                      convertLongToBytes(hregister[0].hw40i, hregister[0].hw40bi);
                      break;  
                   case HW48:
                      hregister[0].hw48i = getFileHashHW64((char *) filename.c_str(), hregister[0].hw48key);
                      convertLongToBytes(hregister[0].hw48i, hregister[0].hw48bi);
                      break;   
                  case HW64:
                      hregister[0].hw64i = getFileHashHW64((char *) filename.c_str(), hregister[0].hw64key);
                      break;
                  case MD2s:                    
                      getFileHashMD2((char *) filename.c_str(), hregister[0].md2i);
                      break;
                  case MD4s:
                      getFileHashMD4((char *) filename.c_str(), hregister[0].md4i);
                      break;
                  case MD5s:
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md5i);
                      break;
                  // TODO Need to remove MD6 since it's not supported  
                  case MD6:
                      //md6_hash(160, byteblock,(uint64_t)(blocksize*8), hregister[0].md6i);
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md6i);
                      break;
                  case MD62:
                      //md6_full_hash(160, byteblock,(uint64_t)(blocksize*8), hregister[0].md62key,16,md6_default_L, 4, hregister[0].md62i);
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md62i);
                      break;
                  case MET641:
                      getFileHashMetro64_1((char *) filename.c_str(), hregister[0].met641i, hregister[0].met641seed);
                      break;
                  case MET642:
                      getFileHashMetro64_2((char *) filename.c_str(), hregister[0].met642i, hregister[0].met642seed);
                      break;
                  case MX3:
                      hregister[0].mx3i = getFileHashMX3((char *) filename.c_str(), hregister[0].mx3seed);
                      break;
                  case PNG:
                      hregister[0].png64i = getFileHashPNG((char *) filename.c_str(), hregister[0].png64seed);
                      break;
                  case RIPE128:
                      getFileHashRipe128((char *) filename.c_str(), hregister[0].ripe128i);
                      break;  
                  case RIPE160:
                      getFileHashRipe160((char *) filename.c_str(), hregister[0].ripe160i);
                      break;
                  case RIPE256:
                      getFileHashRipe256((char *) filename.c_str(), hregister[0].ripe256i);
                      break;  
                  case RIPE320:
                      getFileHashRipe320((char *) filename.c_str(), hregister[0].ripe320i);
                      break;    
                  case SEA:
                      hregister[0].sea64i = getFileHashSeahash((char *) filename.c_str(), hregister[0].sea64seed);
                      break;
                  case SIP32: 
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash32i, hregister[0].sipkey32);
                      break;  
                  case SIP322: 
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash322i, hregister[0].sipkey322);
                      break;   
                  case SIP40: 
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash40i, hregister[0].sipkey40);
                      break;                                       
                  case SIP48: 
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash48i, hregister[0].sipkey48);
                      break;    
                  case SIP64: 
                      hregister[0].siphash64i = getFileHashSiphash((char *) filename.c_str(), hregister[0].sipkey64);
                      break;
                  case SIP128: 
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash128i, hregister[0].sipkey128);
                      break;  
                  case SHA164:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1i);
                      break;
                  case SHA1128:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1i);
                      break;
                  case SHA1s:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1i);
                      break;
                  case SHA256s:
                      getFileHashSHA256((char *) filename.c_str(), hregister[0].sha256i);
                      break;
                  case SHA384s:
                      getFileHashSHA384((char *) filename.c_str(), hregister[0].sha384i);
                      break;
                  case SHA512s:
                      getFileHashSHA512((char *) filename.c_str(), hregister[0].sha512i);
                      break;
                  case SPK32:
                      hregister[0].spooky32i = getFileHashSpooky32((char *) filename.c_str(), hregister[0].spookyseed32);
                      break;
                  case SPK64:        
                      hregister[0].spooky64i = getFileHashSpooky64((char *) filename.c_str(), hregister[0].spookyseed64);
                      break;
                  case TIGER192:
                      getFileHashTiger((char *) filename.c_str(), hregister[0].tiger192i);
                      break;                       
                  case XXH32:
                      hregister[0].xxhash32i = getFileHashXXH32((char *) filename.c_str(), hregister[0].xxseed32);
                      break;
                  case XXH64:
                      hregister[0].xxhash64i = getFileHashXXH64((char *) filename.c_str(), hregister[0].xxseed64);
                      break;
                  case WP:
                      getFileHashWP((char *) filename.c_str(), hregister[0].whp512i);
                      break;
                  case WYH:
                      hregister[0].wyhash64i = getFileHashWyhash((char *) filename.c_str(), hregister[0].wyseed64, hregister[0].wysecret64);
                      break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }
    }

    // compare the mdzip file hash list with the output file hash list 
    // if the comparison is true return true otherwise return false
    bool compareFileHashList(std::string& filename) {

         int hashblocksize = 0;
         for(auto hash  : hashlistvt) {
              //hashblocksize = mdHashlist[hash.first-1].blocksize;
              hashblocksize = std::get<2>(hash);

              switch(std::get<0>(hash)) {
                  case CIT64:
                      hregister[0].city64o = getFileHashCityhash((char *) filename.c_str(), hregister[0].city64seed);
                      if (hregister[0].city64i != hregister[0].city64o) return false;
                      break;
                  case CRC32:
                      hregister[0].crc32o = getFileHashCRC32((char *) filename.c_str(), hregister[0].crc32seed);
                      if (hregister[0].crc32i != hregister[0].crc32o) return false;
                      break;
                  case CRC64:
                      hregister[0].crc64o = getFileHashCRC64((char *) filename.c_str(), hregister[0].crc64seed);
                      if (hregister[0].crc64i != hregister[0].crc64o) return false;
                      break;
                  case EDN224:
                      getFileHashEDONR224((char *) filename.c_str(), hregister[0].edn224o);
                      if (memcmp(hregister[0].edn224i, hregister[0].edn224o, 28) != 0) return false;
                      break;  
                  case FAST32:
                      hregister[0].fast32o = getFileHashFast32((char *) filename.c_str(), hregister[0].fast32seed);
                      if (hregister[0].fast32i != hregister[0].fast32o) return false;
                      break;
                  case FAST64:
                      hregister[0].fast64o = getFileHashFast64((char *) filename.c_str(), hregister[0].fast64seed);
                      if (hregister[0].fast64i != hregister[0].fast64o) return false;
                      break;
                  case FNV32:
                      hregister[0].fnv32_1o = getFileHashFNV32((char *) filename.c_str());
                      if (hregister[0].fnv32_1i != hregister[0].fnv32_1o) return false;
                      break;
                  case FNV32A:
                      hregister[0].fnv32a_1o = getFileHashFNV32A((char *) filename.c_str());
                      if (hregister[0].fnv32a_1i != hregister[0].fnv32a_1o) return false;
                      break;
                  case FNV64:
                      hregister[0].fnv64_1o = getFileHashFNV64((char *) filename.c_str());
                      if (hregister[0].fnv64_1i != hregister[0].fnv64_1o) return false;
                      break;
                  case FNV64A:
                      hregister[0].fnv64a_1o = getFileHashFNV64A((char *) filename.c_str());
                      if (hregister[0].fnv64a_1i != hregister[0].fnv64a_1o) return false;
                      break;
                  case HAS160:
                      getFileHashHAS160((char *) filename.c_str(), hregister[0].has160o);
                      if (memcmp(hregister[0].has160i, hregister[0].has160o, 20) != 0) return false;
                      break;  
                  case HW40:
                      hregister[0].hw40o = getFileHashHW64((char *) filename.c_str(), hregister[0].hw40key);
                      convertLongToBytes(hregister[0].hw40o, hregister[0].hw40bo);
                      if (memcmp(hregister[0].hw40bi, hregister[0].hw40bo, hashblocksize) != 0) return false;
                      break;  
                  case HW48:
                      hregister[0].hw48o = getFileHashHW64((char *) filename.c_str(), hregister[0].hw48key);
                      convertLongToBytes(hregister[0].hw48o, hregister[0].hw48bo);
                      if (memcmp(hregister[0].hw48bi, hregister[0].hw48bo, hashblocksize) != 0) return false;
                      break;  
                  case HW64:
                      hregister[0].hw64o = getFileHashHW64((char *) filename.c_str(), hregister[0].hw64key);
                      if (hregister[0].hw64i != hregister[0].hw64o) return false;
                      break;
                  case MD2s:
                      getFileHashMD2((char *) filename.c_str(), hregister[0].md2o);
                      if (memcmp(hregister[0].md2i, hregister[0].md2o, 16) != 0) return false;
                      break;
                  case MD4s:
                      getFileHashMD4((char *) filename.c_str(), hregister[0].md4o);
                      if (memcmp(hregister[0].md4i, hregister[0].md4o, 16) != 0) return false;
                      break;
                  case MD5s:
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md5o);
                      if (memcmp(hregister[0].md5i, hregister[0].md5o, 16) != 0) return false;
                      break;
                  // TODO Need to remove MD6 since it's not supported  
                  case MD6:
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md6o);
                      if (memcmp(hregister[0].md6i, hregister[0].md6o, 20) != 0) return false;
                      break;
                  case MD62:
                      getFileHashMD5((char *) filename.c_str(), hregister[0].md62o);
                      if (memcmp(hregister[0].md62i, hregister[0].md62o, 20) != 0) return false;
                      break;
                  case MET641:
                      getFileHashMetro64_1((char *) filename.c_str(), hregister[0].met641o, hregister[0].met641seed);
                      if (memcmp(hregister[0].met641i, hregister[0].met641o, 8) != 0) return false;
                      break;
                  case MET642:
                      getFileHashMetro64_2((char *) filename.c_str(), hregister[0].met642o, hregister[0].met642seed);
                      if (memcmp(hregister[0].met642i, hregister[0].met642o, 8) != 0) return false;
                      break;
                  case MX3:
                      hregister[0].mx3o = getFileHashMX3((char *) filename.c_str(), hregister[0].mx3seed);
                      if (hregister[0].mx3i != hregister[0].mx3o) return false;
                      break;
                  case PNG:
                      hregister[0].png64o = getFileHashPNG((char *) filename.c_str(), hregister[0].png64seed);
                      if (hregister[0].png64i != hregister[0].png64o) return false;
                      break;
                  case RIPE128:
                      getFileHashRipe128((char *) filename.c_str(), hregister[0].ripe128o);
                      if (memcmp(hregister[0].ripe128i, hregister[0].ripe128o, 16) != 0) return false;
                      break;  
                  case RIPE160:
                      getFileHashRipe160((char *) filename.c_str(), hregister[0].ripe160o);
                      if (memcmp(hregister[0].ripe160i, hregister[0].ripe160o, 20) != 0) return false;
                      break;
                  case RIPE256:
                      getFileHashRipe256((char *) filename.c_str(), hregister[0].ripe256o);
                      if (memcmp(hregister[0].ripe256i, hregister[0].ripe256o, 32) != 0) return false;
                      break; 
                  case RIPE320:
                      getFileHashRipe320((char *) filename.c_str(), hregister[0].ripe320o);
                      if (memcmp(hregister[0].ripe320i, hregister[0].ripe320o, 40) != 0) return false;
                      break;    
                  case SEA:
                      hregister[0].sea64o = getFileHashSeahash((char *) filename.c_str(), hregister[0].sea64seed);
                      if (hregister[0].sea64i != hregister[0].sea64o) return false;
                      break;
                  case SIP32:
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash32o, hregister[0].sipkey32);
                      if (memcmp(hregister[0].siphash32i, hregister[0].siphash32o, 4) != 0) return false;
                      break;  
                  case SIP322:
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash322o, hregister[0].sipkey322);
                      if (memcmp(hregister[0].siphash322i, hregister[0].siphash322o, 4) != 0) return false;
                      break; 
                  case SIP40:
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash40o, hregister[0].sipkey40);
                      if (memcmp(hregister[0].siphash40i, hregister[0].siphash40o, 5) != 0) return false;
                      break;      
                  case SIP48:
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash48o, hregister[0].sipkey48);
                      if (memcmp(hregister[0].siphash48i, hregister[0].siphash48o, 6) != 0) return false;
                      break;   
                  case SIP64:
                      hregister[0].siphash64o = getFileHashSiphash((char *) filename.c_str(), hregister[0].sipkey64);
                      if (hregister[0].siphash64i != hregister[0].siphash64o) return false;
                      break;
                   case SIP128:
                      getFileHashSiphash128((char *) filename.c_str(), hregister[0].siphash128o, hregister[0].sipkey128);
                      if (memcmp(hregister[0].siphash128i, hregister[0].siphash128o, 16) != 0) return false;
                      break; 
                  case SHA164:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1o);
                      if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 8) != 0) return false;
                      break;
                  case SHA1128:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1o);
                      if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 16) != 0) return false;
                      break;
                  case SHA1s:
                      getFileHashSHA1((char *) filename.c_str(), hregister[0].sha1o);
                      if (memcmp(hregister[0].sha1i, hregister[0].sha1o, 20) != 0) return false;
                      break;
                  case SHA256s:
                      getFileHashSHA256((char *) filename.c_str(), hregister[0].sha256o);
                      if (memcmp(hregister[0].sha256i, hregister[0].sha256o, 32) != 0) return false;
                      break;
                  case SHA384s:
                      getFileHashSHA384((char *) filename.c_str(), hregister[0].sha384o);
                      if (memcmp(hregister[0].sha384i, hregister[0].sha384o, 48) != 0) return false;
                      break;
                  case SHA512s:
                      getFileHashSHA512((char *) filename.c_str(), hregister[0].sha512o);
                      if (memcmp(hregister[0].sha512i, hregister[0].sha512o, 64) != 0) return false;
                      break;
                  case SPK32:
                      hregister[0].spooky32o = getFileHashSpooky32((char *) filename.c_str(), hregister[0].spookyseed32);
                      if (hregister[0].spooky32i != hregister[0].spooky32o) return false;
                      break;
                  case SPK64:
                      hregister[0].spooky64o = getFileHashSpooky64((char *) filename.c_str(), hregister[0].spookyseed64);
                      if (hregister[0].spooky64i != hregister[0].spooky64o) return false;
                      break;
                  case TIGER192:
                      getFileHashTiger((char *) filename.c_str(), hregister[0].tiger192o);
                      if (memcmp(hregister[0].tiger192i, hregister[0].tiger192o, 24) != 0) return false;
                      break;     
                  case XXH32:
                      hregister[0].xxhash32o = getFileHashXXH32((char *) filename.c_str(), hregister[0].xxseed32);
                      if (hregister[0].xxhash32i != hregister[0].xxhash32o) return false;
                      break;
                  case XXH64:
                      hregister[0].xxhash64o = getFileHashXXH64((char *) filename.c_str(), hregister[0].xxseed64);
                      if (hregister[0].xxhash64i != hregister[0].xxhash64o) return false;
                      break;
                  case WP:
                      getFileHashWP((char *) filename.c_str(), hregister[0].whp512o);
                      if (memcmp(hregister[0].whp512i, hregister[0].whp512o, 64) != 0) return false;
                      break;
                  case WYH:
                      hregister[0].wyhash64o = getFileHashWyhash((char *) filename.c_str(), hregister[0].wyseed64, hregister[0].wysecret64);
                      if (hregister[0].wyhash64i != hregister[0].wyhash64o) return false;
                      break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

          return true;
    }

    // setByteBlockHashList
    // this sets the block hash list signatures for an input byte block
    void setByteBlockHashList(unsigned char *byteblock, int blocksize) {

          int hashblocksize = 0;
          for(auto hash  : hashlistvt) {
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
                  case EDN224:
                      getEDONR224(byteblock, blocksize, hregister[0].edn224i);
                      break;  
                  case FAST32:
                      hregister[0].fast32i = fasthash32(byteblock, blocksize, hregister[0].fast32seed);
                      break;
                  case FAST64:
                      hregister[0].fast64i = fasthash64(byteblock, blocksize, hregister[0].fast64seed);
                      break;
                  case FNV32:
                      hregister[0].fnv32_1i = fnv32_1(byteblock, blocksize, FNV1_32_INIT);
                      break;
                  case FNV32A:
                      hregister[0].fnv32a_1i = fnv32_1a(byteblock, blocksize, FNV1_32A_INIT);
                      break;
                  case FNV64:
                      hregister[0].fnv64_1i  = fnv64_1(byteblock, blocksize, FNV1_64_INIT);
                      break;
                  case FNV64A:
                      hregister[0].fnv64a_1i = fnv64_1a(byteblock, blocksize, FNV1A_64_INIT);
                      break;
                  case HAS160:
                      getHAS160(byteblock, blocksize, hregister[0].has160i);
                      break;  
                  case HW40:
                      hregister[0].hw40i = HighwayHash64(byteblock, blocksize, hregister[0].hw40key);
                      convertLongToBytes(hregister[0].hw40i, hregister[0].hw40bi);
                      // convertLongToBytes(HighwayHash64(byteblock, blocksize, hregister[0].hw40key), hregister[0].hw40bi);
                      break;
                  case HW48:
                      hregister[0].hw48i = HighwayHash64(byteblock, blocksize, hregister[0].hw48key);
                      convertLongToBytes(hregister[0].hw48i, hregister[0].hw48bi);
                      // convertLongToBytes(HighwayHash64(byteblock, blocksize, hregister[0].hw40key), hregister[0].hw40bi);
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
                  case RIPE128:
                      getRIPEMD128(byteblock, blocksize, hregister[0].ripe128i);
                      break;
                  case RIPE160:
                      RIPEMD160(byteblock, blocksize, hregister[0].ripe160i);
                      break;
                  case RIPE256:
                      getRIPEMD256(byteblock, blocksize, hregister[0].ripe256i);
                      break;
                  case RIPE320:
                      getRIPEMD320(byteblock, blocksize, hregister[0].ripe320i);
                      break;  
                  case SEA:
                      hregister[0].sea64i = seahash((const char*)byteblock, blocksize, hregister[0].sea64seed);
                      break;
                  case SIP32:
                      siphash(byteblock, blocksize, hregister[0].sipkey32, hregister[0].siphash32i, 16);
                      break; 
                  case SIP322:
                      siphash(byteblock, blocksize, hregister[0].sipkey322, hregister[0].siphash322i, 16);
                      break;   
                  case SIP40:
                      siphash(byteblock, blocksize, hregister[0].sipkey40, hregister[0].siphash40i, 16);
                      break;   
                  case SIP48:
                      siphash(byteblock, blocksize, hregister[0].sipkey48, hregister[0].siphash48i, 16);
                      break;       
                  case SIP64: 
                      hregister[0].siphash64i = siphash24(byteblock, blocksize, hregister[0].sipkey64);
                      break;
                  case SIP128:
                      siphash(byteblock, blocksize, hregister[0].sipkey128, hregister[0].siphash128i, hashblocksize);
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
                  case TIGER192:
                      getTiger(byteblock, blocksize, hregister[0].tiger192i);
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
         for(auto hash  : hashlistvt) {
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
                  case EDN224:
                      getEDONR224(byteblock, (long) blocksize, hregister[0].edn224o);
                      if (memcmp(hregister[0].edn224i, hregister[0].edn224o, hashblocksize) != 0) return false;
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
                      hregister[0].fnv32_1o = fnv32_1(byteblock, blocksize, FNV1_32_INIT);
                      if (hregister[0].fnv32_1i != hregister[0].fnv32_1o) return false;
                      break;
                  case FNV32A:
                      hregister[0].fnv32a_1o = fnv32_1a(byteblock, blocksize, FNV1_32A_INIT);
                      if (hregister[0].fnv32a_1i != hregister[0].fnv32a_1o) return false;
                      break;
                  case FNV64:
                      hregister[0].fnv64_1o = fnv64_1(byteblock, blocksize, FNV1_64_INIT);
                      if (hregister[0].fnv64_1i != hregister[0].fnv64_1o) return false;
                      break;
                  case FNV64A:
                      hregister[0].fnv64a_1o = fnv64_1a(byteblock, blocksize, FNV1A_64_INIT);
                      if (hregister[0].fnv64a_1i != hregister[0].fnv64a_1o) return false;
                      break;
                  case HAS160:
                      getHAS160(byteblock, blocksize, hregister[0].has160o);
                      if (memcmp(hregister[0].has160i, hregister[0].has160o, 20) != 0) return false;
                      break;
                  case HW40:
                      // hregister[0].hw40o = HighwayHash64(byteblock, blocksize, hregister[0].hw40key);
                      convertLongToBytes(HighwayHash64(byteblock, blocksize, hregister[0].hw40key), hregister[0].hw40bo);
                      if (memcmp(hregister[0].hw40bi, hregister[0].hw40bo, hashblocksize) != 0) return false;
                      break;
                   case HW48:
                      // hregister[0].hw48o = HighwayHash64(byteblock, blocksize, hregister[0].hw48key);
                      convertLongToBytes(HighwayHash64(byteblock, blocksize, hregister[0].hw48key), hregister[0].hw48bo);
                      if (memcmp(hregister[0].hw48bi, hregister[0].hw48bo, hashblocksize) != 0) return false;
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
                  case RIPE128:
                      getRIPEMD128(byteblock, blocksize, hregister[0].ripe128o);
                      if (memcmp(hregister[0].ripe128i, hregister[0].ripe128o, hashblocksize) != 0) return false;
                      break; 
                  case RIPE160:
                      RIPEMD160(byteblock, blocksize, hregister[0].ripe160o);
                      if (memcmp(hregister[0].ripe160i, hregister[0].ripe160o, 20) != 0) return false;
                      break;
                  case RIPE256:
                      getRIPEMD256(byteblock, blocksize, hregister[0].ripe256o);
                      if (memcmp(hregister[0].ripe256i, hregister[0].ripe256o, hashblocksize) != 0) return false;
                      break; 
                  case RIPE320:
                      getRIPEMD320(byteblock, blocksize, hregister[0].ripe320o);
                      if (memcmp(hregister[0].ripe320i, hregister[0].ripe320o, hashblocksize) != 0) return false;
                      break;                       
                  case SEA:
                      hregister[0].sea64o = seahash((const char*) byteblock, blocksize, hregister[0].sea64seed);
                      if (hregister[0].sea64i != hregister[0].sea64o) return false;
                      break;
                  case SIP32:
                      siphash(byteblock, blocksize, hregister[0].sipkey32, hregister[0].siphash32o, 16);
                      if (memcmp(hregister[0].siphash32i, hregister[0].siphash32o, hashblocksize) != 0) return false;
                      break;
                  case SIP322:
                      siphash(byteblock, blocksize, hregister[0].sipkey322, hregister[0].siphash322o, 16);
                      if (memcmp(hregister[0].siphash322i, hregister[0].siphash322o, hashblocksize) != 0) return false;
                      break;
                  case SIP40:
                      siphash(byteblock, blocksize, hregister[0].sipkey40, hregister[0].siphash40o, 16);
                      if (memcmp(hregister[0].siphash40i, hregister[0].siphash40o, hashblocksize) != 0) return false;
                      break;     
                  case SIP48:
                      siphash(byteblock, blocksize, hregister[0].sipkey48, hregister[0].siphash48o, 16);
                      if (memcmp(hregister[0].siphash48i, hregister[0].siphash48o, hashblocksize) != 0) return false;
                      break;     
                  case SIP64:
                      hregister[0].siphash64o = siphash24(byteblock, blocksize, hregister[0].sipkey64);
                      if (hregister[0].siphash64i != hregister[0].siphash64o) return false;
                      break;
                  case SIP128:
                      siphash(byteblock, blocksize, hregister[0].sipkey128, hregister[0].siphash128o, 16);
                      if (memcmp(hregister[0].siphash128i, hregister[0].siphash128o, 16) != 0) return false;
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
                  case TIGER192:
                      getTiger(byteblock, blocksize, hregister[0].tiger192o);
                      if (memcmp(hregister[0].tiger192i, hregister[0].tiger192o, hashblocksize) != 0) return false;
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
                  //  std::cout << "Invalid hash " << std::endl;
              }
          }

          return true;
    }

    // display the hash list keys
    std::string displayHLhashKeys() {
        // clear the string stream
        ss.str(std::string());

        int i = 0;
        std::string hashname;
        int hashblocksize    = 0;
        int hashkeyblocksize = 0;
        bool randhashkey     = true;
        for(auto hash  : hashlistvt) {

              hashname          = std::get<1>(hash);
              hashblocksize     = std::get<2>(hash);    
              hashkeyblocksize  = std::get<3>(hash); 
              randhashkey       = std::get<4>(hash); 
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
                  case EDN224:
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
                  case HAS160:
                      break;   
                  case HW40: 
                      ss << std::get<1>(hash) << " keys ";
                      for(i=0; i < 4; i++) { ss << std::to_string(hregister[0].hw40key[i]) << " "; }
                      break;  
                  case HW48: 
                      ss << std::get<1>(hash) << " keys ";
                      for(i=0; i < 4; i++) { ss << std::to_string(hregister[0].hw48key[i]) << " "; }
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
                  case RIPE128:
                      break;   
                  case RIPE160:
                      break;
                  case RIPE256:
                      break;
                  case RIPE320:
                      break;      
                  case SEA:
                      ss << std::get<1>(hash) << " keys ";
                      ss << std::to_string(hregister[0].sea64seed) << " ";
                      break;
                  case SIP32:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey32, 16);
                      break;  
                  case SIP322:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey322, 16);
                      break;   
                  case SIP40:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey40, 16);
                      break;       
                  case SIP48:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey48, 16);
                      break;        
                  case SIP64:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey64, 16);
                      break;
                  case SIP128:
                      ss << std::get<1>(hash) << " keys ";
                      addHashToDisplayStream((unsigned char*) hregister[0].sipkey128, 16);
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
                  case TIGER192:
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
                      for(i=0; i < 5; ++i)
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
        for(auto hash  : hashlistvt) {

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
                  case EDN224:
                      addHashToDisplayStream(hregister[0].edn224i, hashblocksize);
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
                  case HAS160:
                      addHashToDisplayStream(hregister[0].has160i, hashblocksize);
                      break;   
                  case HW40:
                      addHashToDisplayStream(hregister[0].hw40bi, hashblocksize);
                      break;   
                  case HW48: 
                      addHashToDisplayStream(hregister[0].hw48bi, hashblocksize);
                      break;     
                  case HW64:
                      ss << std::to_string(hregister[0].hw64i) << " ";
                      uint8_t  hw40bi[8];
                      convertLongToBytes(hregister[0].hw64i, hw40bi);
                      addHashToDisplayStream(hw40bi, hashblocksize);
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
                  case RIPE128:
                      addHashToDisplayStream(hregister[0].ripe128i, hashblocksize);
                      break;   
                  case RIPE160:
                      addHashToDisplayStream(hregister[0].ripe160i, hashblocksize);
                      break;
                  case RIPE256:
                      addHashToDisplayStream(hregister[0].ripe256i, hashblocksize);
                      break;  
                  case RIPE320:
                      addHashToDisplayStream(hregister[0].ripe320i, hashblocksize);
                      break;     
                  case SEA:
                      ss << std::to_string(hregister[0].sea64i) << " ";
                      break;
                  case SIP32:
                      addHashToDisplayStream(hregister[0].siphash32i, hashblocksize);
                      break;  
                  case SIP322:
                      addHashToDisplayStream(hregister[0].siphash322i, hashblocksize);
                      break;  
                  case SIP40:
                      addHashToDisplayStream(hregister[0].siphash40i, hashblocksize);
                      break;        
                  case SIP48:
                      addHashToDisplayStream(hregister[0].siphash48i, hashblocksize);
                      break;      
                  case SIP64:
                      ss << std::to_string(hregister[0].siphash64i) << " ";
                      break;
                  case SIP128:
                      addHashToDisplayStream(hregister[0].siphash128i, hashblocksize);
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
                  case TIGER192:
                      addHashToDisplayStream(hregister[0].tiger192i, hashblocksize);
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

         // add the type here
         // it can be file or block group or block or keylist
         // HASHFILE,HASHBLOCKGROUP,HASHBLOCK,HASHKEY???
         std::string hashname = "Block";
         if (hashtype == HASHFILE) hashname = "File";
         if (hashtype == HASHBLOCKGROUP) hashname = "Blockgroup";

         std::cout << hashname << " hashlist " << std::endl;
         std::cout << getHLvectorsString() << std::endl;

    }

    // get and create the vector list string associated with the hash type
    // could add column for is the key being randomized or the defaut key and calculate the correct composite size
    //
    // Hash Block Vector
    // Number      Hash ID      Hash Name   Blocksize   Blockkeysize
    // 1           1            cit64       8           8          
    // 2           2            crc32       4           4          
    // 3           3            crc64       8           8          
    // 4           4            edn224      28          0          
    // 5           5            fast32      4           4          
    //                          Total       52          24
    std::string getHLvectorsString() {
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
         for(auto val  : hashlistvt) {
             hlss << std::left << std::setw(12) << i;
             hlss << std::left << std::setw(13) << std::get<0>(val);
             hlss << std::left << std::setw(12) << std::get<1>(val);
             hlss << std::left << std::setw(12) << std::get<2>(val);
             hlss << std::left << std::setw(11) << std::to_string(mdHashlist[std::get<0>(val)-1].keysize) << std::endl;
       
             blocksize    += std::get<2>(val);
             blockkeysize += std::get<3>(val);
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
    // maybe make this a bool for the keylist or hashlist string or have a separate one for the key list
    std::string getHLvectorsStringNames() {
         // clear the hlss string stream
         hlss.str(std::string());

         int block        = 1;
         int hashListsize = 0;
         hashListsize = hashlistvt.size();

         for(auto val  : hashlistvt) {
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
         std::cout << std::left << std::setw(16) << "Blocksize";
         std::cout << std::left << std::setw(16) << "Blockkeysize" << std::endl;
         std::cout << "===============================================================================================" << std::endl;

         for (int i = 0; i < hashlistsize; i++) {
            if (mdHashlist[i].name == "last") break;
            std::cout << std::left << std::setw(12) << i + 1; 
            std::cout << std::left << std::setw(12) << mdHashlist[i].name;
            std::cout << std::left << std::setw(30) << mdHashlist[i].description;

            // std::cout << std::left << std::setw(12) << std::boolalpha << mdHashlist[i].haskey;
            if (mdHashlist[i].haskey == true)  std::cout << std::left << std::setw(12) << "True";
            if (mdHashlist[i].haskey == false) std::cout << std::left << std::setw(12) << "False"; 

            std::cout << std::left << std::setw(16) << mdHashlist[i].blocksize;
            std::cout << std::left << std::setw(16) << mdHashlist[i].keysize;
            std::cout << std::endl;
         }
    }

    // get the hashlist vector length hashlist based on the input hash type
    int getHLVectorLength() {

        int hashListsize = 0;
        hashListsize = hashlistvt.size();

        return hashListsize;

    }

    // calculate the block key size for the correct hashlist type
    // this returns the sum of the hash list vector block sizes
    // TODO Check if the hash key boolean is true or false
    int calcBlockKeySize() {

        int hashblocksize = 0;
        int sumhashkeyblocksize = 0;
        for(auto hash  : hashlistvt) {
              hashblocksize = std::get<3>(hash);
              sumhashkeyblocksize += hashblocksize;
        }

        return sumhashkeyblocksize;

    }

    // calculate the block size for the correct hashlist type
    // this returns the sum of the hash list vector block sizes
    int calcBlockSize() {

        int hashblocksize = 0;
        int sumhashblocksize = 0;
        for(auto hash  : hashlistvt) {
              hashblocksize = std::get<2>(hash);
              sumhashblocksize += hashblocksize;
        }

        return sumhashblocksize;

    }

    // get the registry hash list size
    // hashlistsize = LAST - 1;
    int getRegistrySize() {

        return hashlistsize;

    }
    
    // increment the file block number
    // it starts at zero
    // in a 10 block file this means 
    // 0 is the start number
    // 9 is the end number 
    void incrementBlockNum(int incKey) {
        incrementKey = incKey;

        if (blocknumber >= 1 && incKey != NOINC) incrementBlockKeyList();
        blocknumber++;
        // std::cout << "HCL Block Number " << blocknumber << std::endl;
        
    }

    // get the file block number
    long getHclBlockNum() {
        return blocknumber;
    }
};
