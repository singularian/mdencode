#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <time.h>
#include <plog/Log.h>
#include "../plog/Initializers/RollingFileInitializer.h"

class mdMutexLog 
{
private:
    bool islogging;
    bool logThreadFloor;
    int lastThread;
    int threadcount;
    int blocknumber;
    int blocksize;
    std::mutex mutex;
public:
    std::string logger_name;

    // initialize mdMutex
    mdMutexLog(bool logging) {
       islogging = logging;   
       logThreadFloor = true;    
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

    // writeLog the Thread Modulus thread parameters
    // modulusTotalThreadInt = modulus times totalthreadnumber
    //    if the totalthreadnumber = 32 
    //    modulusTotalThreadInt = modulus * 32
    // modulusThreadInt = modulus * threadnumber
    //    if the totalthreadnumber = 32 
    //    modulusThreadInt0  = modulus * 0
    //    modulusThreadInt31 = modulus * 31
    void writeLogThreadFloor(int thread, int threadcount, mpz_t modulusTotalThreadInt, mpz_t modulusThreadInt) 
    {
        if (islogging && logThreadFloor) {
           mutex.lock();

           /* Log the modulus thread multiples */
           mpz_class modInt(modulusTotalThreadInt);
           mpz_class modThreadInt(modulusThreadInt);

           PLOGD << "tnum " << std::to_string(thread) << "/" << std::to_string(threadcount)
           << "; modthreadtotalint = " << modInt.get_str() << "; modthreadint = mod * " << std::to_string(thread) <<  " = " << modThreadInt.get_str();
 
           // only log this once 
           if (thread == (threadcount - 1)) logThreadFloor = false;
           mutex.unlock();
        }
    }

    // writeLog the Thread blockint for each modscan n million iterations
    void writeLogThread(int thread, mpz_t blockint) 
    {
        if (islogging) {
           mutex.lock();
           lastThread = thread;

           /* Write a message */
           mpz_class blockInt(blockint);
           PLOGD << "thread " << std::to_string(thread) << " " << blockInt.get_str();
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
