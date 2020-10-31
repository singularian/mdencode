#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <time.h>

#define CLOG_MAIN
#include "clog.h"

class mdMutexLog 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    const int MY_LOGGER = 0;
    std::mutex mutex;
public:

    // initialize mdMutex
    mdMutexLog(bool logging) {
       islogging = logging;       
       int r;

       char logfilename [100];

       format_time(logfilename);

       if (islogging) {
         /* Initialize the logger */
         // r = clog_init_path(MY_LOGGER, "my_log.txt");
         r = clog_init_path(MY_LOGGER, logfilename);
         if (r != 0) {
           fprintf(stderr, "Logger initialization failed.\n");
           // return 1;
         }

         /* Set minimum log level to info (default: debug) */
         clog_set_level(MY_LOGGER, CLOG_INFO);
       }
    }

    // Destructor
    ~mdMutexLog()
    {
        if (islogging) 
        clog_free(MY_LOGGER);
    } 

    // write the prefix mod scan info
    // modscan floor / ceil / thread info 
    void writePrefixLog()
    {
        mutex.lock();
        mutex.unlock();
    }

    // writeLog and display text 
    void writeLog(char *logtext)
    {
        if (islogging) {
           mutex.lock();

           /* Write a message */
           // clog_info(CLOG(MY_LOGGER), "Hello, %s!", "world");
           // clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, "world");
           clog_info(CLOG(MY_LOGGER), "%s", logtext);
           printf("%s", logtext);
           mutex.unlock();
        }
    }


    // writeLog Thread 
    // void writeLogThread(int thread, mpz_t blockint, char *logtext) 
    void writeLogThread(int thread, mpz_t blockint) 
    {
        char a [1000]; // need to make this fit the byteblock bigint size
        if (islogging) {
           mutex.lock();
           lastThread = thread;

           /* Write a message */
           // clog_info(CLOG(MY_LOGGER), "Hello, %s!", "world");
           // clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, "world");
           mpz_get_str (a, 10, blockint);
           // clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, logtext);
           clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, a);

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
