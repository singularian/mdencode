#include <string>
#include <stdio.h>
#include <iostream>
#include "external/easylogging++.h"

using namespace el;

INITIALIZE_EASYLOGGINGPP

int main() {

//    mdMutexLog log(true);
//    char str1[30] = "Test Message"; 
  //  log.writeLogThread(12, str1);

//    std::string str = "test 22";
//     log.writeLogThread(12, (char *) str.c_str());

    // Load configuration from file
    // el::Configurations conf("y-conf.conf");
    // Reconfigure single logger
    //el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    //el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file

    Configurations c;
    // c.setGlobally(ConfigurationType::Format, "[%logger] %level: %msg");
    c.setGlobally(ConfigurationType::Filename, "test.custom.log");
    // c.setGlobally(ConfigurationType::ToStandardOutput, "false");
    // el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToStandardOutput, "false");
    // el::Loggers::reconfigureLogger("default", consoleConf);

    el::Loggers::getLogger("handler_check_logger");
    el::Loggers::reconfigureLogger("handler_check_logger", el::ConfigurationType::Filename, "max-size.log");
    el::Loggers::reconfigureLogger("handler_check_logger", el::ConfigurationType::ToStandardOutput, "false");
    CLOG(INFO, "handler_check_logger") << "Test ";

    LOG(INFO) << "Set default configuration but existing loggers not updated yet";
    LOG(DEBUG) << "logtest";
    LOG(TRACE) << "trace log";

    return 0;
}

