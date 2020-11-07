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

Hashlist mdHashlist[5] = {{1, "sha1", "desc", false, 8},
                          {1, "sha1", "desc", false, 8},
                          {1, "sha1", "desc", false, 8},
                          {1, "sha1", "desc", false, 8},
                          {2, "sha2", "desc", false, 8}};


class mdHashContextList 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    std::vector<int> filehlist;
    std::vector<int> blockgrouphlist;
    std::vector<int> blockhlist;
public:
    std::string hash_name;

    // initialize mdMutex
    mdHashContextList() {
    }

    // Destructor
    ~mdHashContextList()
    {
    } 

    void setHashList()
    {
    }

    void displayHashList(int format) 
    {

        int size;
        size = sizeof(mdHashlist) / sizeof(struct Hashlist);

        for (int i = 0; i < size; i++) {
           std::cout << mdHashlist[i].uid;
           std::cout << std::setw(12) << mdHashlist[i].name;
           std::cout << std::setw(12) << mdHashlist[i].description;
           std::cout << std::setw(12) << std::boolalpha << mdHashlist[i].haskey;
           std::cout << std::setw(12) << mdHashlist[i].blocksize;
           std::cout << std::endl;
        }
    }

    

};
