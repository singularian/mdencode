#include<iostream>
#include<thread>
#include<vector>
#include<mutex>

class mdMutex 
{
private:
    int isMatched;
    int notFound;
    int matchCount;
    bool isRunning;
    int lastThread;
    int threadCount;
    int blocksize;
    unsigned char *byteblock;
    std::mutex mutex;
public:

    // initialize mdMutex
    mdMutex(int threadCnt) {
        isMatched   = 0;
        notFound    = 0;
        matchCount  = 0;
        threadCount = threadCnt;
        isRunning   = false;
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
        if (notFound == threadCount) {
            isMatched = 1;
        }
        mutex.unlock();
    }

    // set matched
    // ismatched value
    // searching = 0 // searching for the value with the modscan
    // not found = 1 // modscan mutext match result
    // found     = 2 // modscan mutext match result
    void setMatched(int thread) 
    {
        mutex.lock();
        isMatched = 2;
        lastThread = thread;
        matchCount++;
        mutex.unlock();
    }

    // reset the mutex status for the next mdzip block
    void resetMatched()
    {
        mutex.lock();
        isMatched  = 0;
        lastThread = 0;
        notFound   = 0;
        matchCount = 0;
        mutex.unlock();

    }

    
    int  getMatchThread() {     return lastThread; }
    int  getNotFound()    {     return notFound; }
    // check if there is a match
    int getIsMatched()    {     return isMatched; }
};
