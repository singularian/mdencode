#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <time.h>
//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/basic_file_sink.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

class mdMutexLog 
{
private:
    bool islogging;
    int lastThread;
    int threadcount;
    int blocksize;
    // std::shared_ptr<spdlog::logger> log; 
    // std::shared_ptr* log; 
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

       // if (islogging) {
          // auto log = spdlog::basic_logger_mt("mdencode_logger", "logs/basic-log.txt");
          //auto log = spdlog::basic_logger_mt("mdencode_logger", logger_name);
          // registerLogger(log);
          // auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
          // auto log = spdlog::basic_logger_mt("mdencode_logger", logfilename); 
          //log->warn("testlog");
          //spdlog::set_default_logger(log);
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

    // writeLog and display text 
    void writeLog(char *logtext)
    {
        if (islogging) {
           mutex.lock();

           /* Write a message */
           // clog_info(CLOG(MY_LOGGER), "Hello, %s!", "world");
           // clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, "world");
           // clog_info(CLOG(MY_LOGGER), "%s", logtext);
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
           // clog_info(CLOG(MY_LOGGER), "Thread %d %s!", thread, a);
           // log->warn("test2 ", thread);
           //try {
           // log->warn("test2 ");
            //  auto m_logger = spdlog::get("mdencode_logger");
            //  m_logger->warn("test 33333333333333");
              // log->info("slkfjlskdfjskdjfsdfksdfjklsdfjklsdfkjlsfdkjlsdfjkl");
              // spdlog::get("mdencode_logger")->info("LoggingTest::ctor");
           //} catch (const spdlog::spdlog_ex &ex) {
           //   std::cout << ex.what();
           //}
           //log->spdlog::logger::warn("test");
           ////////////////log->warn("lskdfjskdfjsdlkflskdjfsdklf");
           PLOGD << "thread " << thread << " " << a;
           // std::cout << "logging thread" << std::endl;
           mutex.unlock();
        }
    }

    

    // Store the formatted string of time in the output
    void format_time(char *output){
      time_t rawtime;
      struct tm * timeinfo;

      time ( &rawtime );
      timeinfo = localtime ( &rawtime );

      sprintf(output, "logs/decodeRandom_%d%d%d%d%d%d.log",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
};
