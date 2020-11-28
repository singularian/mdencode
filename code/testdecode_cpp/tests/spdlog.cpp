#include <string>
#include <stdio.h>
// #include "mdMutexLog.h"
#include "../spdlog/spdlog.h"

int main() {

//    mdMutexLog log(true);
//    char str1[30] = "Test Message"; 
  //  log.writeLogThread(12, str1);

//    std::string str = "test 22";
//     log.writeLogThread(12, (char *) str.c_str());

    // el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename, "max-size.log");
    // If we log before configuration, we will end up with heaps of internal errors because ELPP_NO_DEFAULT_LOG_FILE is defined before include
    //el::Configurations confFromFile("../default-logger.conf");


    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(spdlog::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");

    return 0;
}

