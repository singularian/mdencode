#include <string>
#include <stdio.h>
#include "mdMutexLog.h"

int main() {

    mdMutexLog log(true);
    char str1[30] = "Test Message"; 
  //  log.writeLogThread(12, str1);

    std::string str = "test 22";
//     log.writeLogThread(12, (char *) str.c_str());
    return 0;
}

