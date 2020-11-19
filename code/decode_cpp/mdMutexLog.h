#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <time.h>
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

class mdMutexLog 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    std::mutex mutex;
public:
    std::string logger_name;

    // initialize mdMutex
    mdMutexLog(bool logging) {
       islogging = logging;       
       int r;

       char logfilename [100];

       format_time(logfilename);
       logger_name = logfilename;

       if (islogging) { 
       // plog::init(plog::debug, logger_name);
       // plog::init(plog::info, (const char*) logfilename);
         plog::init(plog::debug, (const char*) logfilename);
         PLOGD << "starting log!"; 
      }
    }

    // Destructor
    ~mdMutexLog()
    {
    } 

    // write the prefix mod scan info
    // modscan floor / ceil / thread info 
    void writePrefixLog()
    {
        mutex.lock();
        mutex.unlock();
    }

    // check if logging
    bool checkIfLogging() {
        return islogging;
    }

    // writeLog and display text 
    void writeLog(std::string logmessage)
    {

        std::cout << logmessage << std::endl;
        if (islogging) {
           mutex.lock();

           /* Write logmessage */
           PLOGD << logmessage; 
           mutex.unlock();
        }
    }

    // log the byteblock
    void logMatchByteblock(unsigned char *byteblock, int blocksize, bool ishex) {

        int i;
        std::ostringstream result;
        char hex[3]; 
        for(i=0;i<blocksize;i++)
        {
            if (ishex == false) {
                result << std::setw(4) << std::left << std::setfill(' ') << std::to_string(byteblock[i]);
            } else {
                sprintf(hex, "%02X", byteblock[i]);
                // result << std::setw(4) << std::left << std::uppercase << std::hex << std::setfill(' ') << (int)byteblock[i];
                result << std::setw(4) << std::left << std::setfill(' ') << hex;
            }
        }

        writeLog(result.str());

    }


    // writeLog Thread 
    // void writeLogThread(int thread, mpz_t blockint, char *logtext) 
    void writeLogThread(int thread, mpz_t blockint) 
    {
        char blockInt [2000]; // need to make this fit the byteblock bigint size
        if (islogging) {
           mutex.lock();
           lastThread = thread;

           /* Write a message */
           mpz_get_str (blockInt, 10, blockint);
           PLOGD << "thread " << std::to_string(thread) << " " << blockInt;
           mutex.unlock();
        }
    }

    

    // Store the formatted string of time in the output
    void format_time(char *output){
      time_t rawtime;
      struct tm * timeinfo;

      time ( &rawtime );
      timeinfo = localtime ( &rawtime );

      sprintf(output, "decodeRandom_%d%d%d%d%d%d.log",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
};
