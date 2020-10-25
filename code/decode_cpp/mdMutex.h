#include<iostream>
#include<thread>
#include<vector>
#include<mutex>

class mdMutex 
{
private:
    int notFound;
    int matchCount;
    bool isMatched;
    int lastThread;
    int blocksize;
    unsigned char *byteblock;
    std::mutex mutex;
public:

    // initialize mdMutex
    mdMutex() {
        notFound == 0;
        isMatched = false;
    }

    // Destructor
    ~mdMutex()
    {


    } 

    // increment the threads match not found count
    void incNotFound()
    {
        mutex.lock();
        notFound++;
        mutex.unlock();
    }

    int  getNotFound()    {     return notFound; }
    bool getIsMatched()   {     return isMatched; }
};
