#include <stdio.h>


#define CLOG_MAIN
#include "clog.h"

const int MY_LOGGER = 0; /* Unique identifier for logger */

int main() {
    int r;

    /* Initialize the logger */
    r = clog_init_path(MY_LOGGER, "my_log.txt");
    if (r != 0) {
        fprintf(stderr, "Logger initialization failed.\n");
        return 1;
    }

    /* Set minimum log level to info (default: debug) */
    clog_set_level(MY_LOGGER, CLOG_INFO);

    /* Write a message */
    clog_info(CLOG(MY_LOGGER), "Hello, %s!", "world");

    /* Clean up */
    clog_free(MY_LOGGER);
    return 0;
}

