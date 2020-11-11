#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "external/highwayhash.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include "external/csiphash.c"

struct Hashlist {
    int uid;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
};

Hashlist mdHashlist[8] = {{1, "hw64",     "Highway Hash 64",       true, 8},
                          {2, "md5",      "MD5",                   false, 16},
                          {3, "sip64",    "Siphash 64",            true, 8},
                          {4, "sha1_64",  "SHA1 64",               false, 8},
                          {5, "sha1_128", "SHA1 128",              false, 16},
                          {6, "sha1",     "SHA1",                  false, 20},
                          {7, "sha2",     "SHA1 Unused Signature", false, 8}};


class mdHashContextList 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    int hashlistsize;
    std::vector<std::pair<int,std::string>> filehlist;
    std::vector<std::pair<int,std::string>> blockgrouphlist;
    std::vector<std::pair<int,std::string>> blockhlist;
    // hash results
    // need to make these an array sized 6 for files and bg and block hash results
    uint64_t hw64i;
    uint64_t hw64o;
    const uint64_t hw64key[4] = {1, 2, 3, 4};
    uint8_t md5i[41];
    uint8_t md5o[41];
    uint8_t sha1i[41];
    uint8_t sha1o[41];
    uint64_t siphash64i;
    uint64_t siphash64o;
    char sipkey[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
public:
    std::string hash_name;
    std::stringstream ss;

    // initialize mdHashContextList
    mdHashContextList() {
        hashlistsize = sizeof(mdHashlist) / sizeof(struct Hashlist);
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
                  case 1:
                    hw64i = HighwayHash64(byteblock, blocksize, hw64key);
                    break;
                  case 2:
                    MD5(byteblock,(long)blocksize,md5i);
                    break;
                  case 3:
                    siphash64i = siphash24(byteblock, blocksize, sipkey);
                    break;
                  case 4:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case 5:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case 6:
                    SHA1(byteblock, blocksize, sha1i);
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
                  case 1:
                    hw64o = HighwayHash64(byteblock, blocksize, hw64key);
                    if (hw64i != hw64o) return false;
                  break;
                  case 2:
                    MD5(byteblock,(long)blocksize,md5o);
                    if (memcmp(md5i, md5o, 16) != 0) return false;
                  break;
                  case 3:
                    siphash64o = siphash24(byteblock, blocksize, sipkey);
                    if (siphash64i != siphash64o) return false;
                    break;
                  case 4:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 8) != 0) return false;
                    break;
                  case 5:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 16) != 0) return false;
                    break;
                  case 6:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 20) != 0) return false;
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
        std::string hashlist;
        for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case 1:
                     ss << hash.second << " " << std::to_string(hw64i) << " ";
                     break;
                  case 2:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)md5i[i];
                     ss << " ";
                     break;
                  case 3:
                     ss << hash.second << " " << std::to_string(siphash64i) << " ";
                     break;
                  case 4:
                     ss << hash.second << " ";
                     for(int i=0; i<8; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case 5:
                     ss << hash.second << " ";
                     for(int i=0; i<16; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
                     break;
                  case 6:
                     ss << hash.second << " ";
                     for(int i=0; i<20; ++i)
                           ss << std::uppercase << std::hex << (int)sha1i[i];
                     ss << " ";
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

        std::cout << "ID" << std::setw(12) << "Hash Name " << std::setw(30) << "Description" << std::setw(12) << "Key" << std::setw(12) << "Blocksize" << std::endl;

        for (int i = 0; i < hashlistsize; i++) {
           std::cout << mdHashlist[i].uid;
           std::cout << std::setw(12) << mdHashlist[i].name;
           std::cout << std::setw(30) << mdHashlist[i].description;
           std::cout << std::setw(12) << std::boolalpha << mdHashlist[i].haskey;
           std::cout << std::setw(12) << mdHashlist[i].blocksize;
           std::cout << std::endl;
        }
    }

    

};
