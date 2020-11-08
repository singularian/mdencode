#include <iostream>
#include <vector>
#include <iomanip>
#include "external/highwayhash.h"

struct Hashlist {
    int uid;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
};

Hashlist mdHashlist[5] = {{1, "sha1", "SHA1", false, 20},
                          {2, "hw",   "Highway Hash 64", false, 8},
                          {3, "sha1", "desc", false, 8},
                          {4, "sha1", "desc", false, 8},
                          {5, "sha2", "desc", false, 8}};


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
    uint8_t sha1i[41];
    uint8_t sha1o[41];
    uint64_t hw64i;
    uint64_t hw64o;
    const uint64_t hw64key[4] = {1, 2, 3, 4};
public:
    std::string hash_name;

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
              if (val < hashlistsize) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val].name;
                filehlist.push_back(hashPair);
              }
          }

    }

    // set the block group hash list
    void setBlockGroupHashList(const std::vector<int> &v)
    {  
          std::pair<int,std::string> hashPair; 
          for(int val  : v) {
              if (val < hashlistsize) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val].name;
                blockgrouphlist.push_back(hashPair);
              }
          }

    }

    // set the file block hash list
    void setBlockHashList(const std::vector<int> &v)
    {
          std::pair<int,std::string> hashPair;
          for(int val  : v) {
              if (val < hashlistsize) {
                hashPair.first  = val;
                hashPair.second = mdHashlist[val].name;
                blockhlist.push_back(hashPair);
              }
          }

    }

    // setHashList
    void setBlockHashList(unsigned char *byteblock, int blocksize) {

          for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case 1:
                    SHA1(byteblock, blocksize, sha1i);
                    break;
                  case 2:
                    hw64i = HighwayHash64(byteblock, blocksize, hw64key);
                    break;
                  default:
                    std::cout << "Invalid hash" << std::endl;
              }
          }
    }

    // run the hash context list on the current byte block and compare the input hash with the computed output hash
    // if the comparison is true return true otherwise return false
    bool compareBlockHashList(unsigned char *byteblock, int blocksize) {
         for(auto hash  : blockhlist) {
              switch(hash.first) {
                  case 1:
                    SHA1(byteblock, blocksize, sha1o);
                    if (memcmp(sha1i, sha1o, 20) != 0) return false;
                    break;
                  case 2:
                    hw64o = HighwayHash64(byteblock, blocksize, hw64key);
                    if (hw64i != hw64o) return false;
                  break;
                  // default:
                  //  std::cout << "Invalid hash" << std::endl;
              }
          }

          return true;
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

        std::cout << "id" << std::setw(12) << "Hash Name " << std::setw(30) << "Description" << std::setw(12) << "Key" << std::setw(12) << "Blocksize" << std::endl;

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
