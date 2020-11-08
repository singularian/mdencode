#include <iostream>
#include <vector>
#include <iomanip>


struct Hashlist {
    int uid;
    std::string name;
    std::string description;
    bool haskey;
    int blocksize;
};

Hashlist mdHashlist[5] = {{1, "sha1", "SHA1", false, 20},
                          {2, "sha1", "desc", false, 8},
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

        // int size;
        // size = sizeof(mdHashlist) / sizeof(struct Hashlist);

        for (int i = 0; i < hashlistsize; i++) {
           std::cout << mdHashlist[i].uid;
           std::cout << std::setw(12) << mdHashlist[i].name;
           std::cout << std::setw(12) << mdHashlist[i].description;
           std::cout << std::setw(12) << std::boolalpha << mdHashlist[i].haskey;
           std::cout << std::setw(12) << mdHashlist[i].blocksize;
           std::cout << std::endl;
        }
    }

    

};
