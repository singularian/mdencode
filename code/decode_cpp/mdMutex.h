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
    bool isRunning;
    int lastThread;
    int blocksize;
    unsigned char *byteblock;
    std::mutex mutex;
public:

    // initialize mdMutex
    mdMutex() {
        notFound   = 0;
        matchCount = 0;
        isMatched  = false;
        isRunning  = false;
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

    // set matched
    void setMatched() 
    {
        mutex.lock();
        isMatched = true;
        matchCount++;
        mutex.unlock();
    }

    
    int  getNotFound()    {     return notFound; }
    bool getIsMatched()   {     return isMatched; }
};
