#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "irc.h"
#include "network.h"
#include "utils.h"

irc_config_t config;

void cleanup(int sig) {
    (void)sig;
    if (config.sockfd != 0) {
        close(config.sockfd);
    }
    exit(0);
}

int main(int argc, char** argv) {
    if (parse_args(argc, argv, &config) != 0) {
        return 1;
    }

    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);

    if (startup(config.server, config.nick, config.channel) != 0) {
        log_error("Startup failed.");
        return 1;
    }

    pthread_join(config.input_thread, NULL);

    return 0;
}
