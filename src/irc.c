#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "irc.h"
#include "network.h"
#include "utils.h"

irc_config_t config;

void send_command(const char* cmd) {
    if (send(config.sockfd, cmd, strlen(cmd), 0) < 0) {
        log_error("Failed to send command.");
    }
}

void* handle_input(void* arg) {
    char buf[256];
    char cmdbuf[MAX_MSG_LENGTH];

    while (1) {
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            log_error("Failed to read input.");
            continue;
        }

        snprintf(cmdbuf, sizeof(cmdbuf), "PRIVMSG #%s :%s\r\n", config.channel, buf);
        send_command(cmdbuf);

        log_info("Message sent.");
    }
    return NULL;
}

int send_initial_commands(const char* nick, const char* channel) {
    char cmdbuf[MAX_MSG_LENGTH];

    snprintf(cmdbuf, sizeof(cmdbuf), "NICK %s\r\n", nick);
    send_command(cmdbuf);

    snprintf(cmdbuf, sizeof(cmdbuf), "USER %s 0 * :%s\r\n", nick, nick);
    send_command(cmdbuf);

    snprintf(cmdbuf, sizeof(cmdbuf), "JOIN #%s\r\n", channel);
    send_command(cmdbuf);

    return 0;
}

int startup(const char* server, const char* nick, const char* channel) {
    config.sockfd = connect_to_server(server);
    if (config.sockfd < 0) {
        log_error("Failed to connect to server.");
        return 1;
    }

    send_initial_commands(nick, channel);

    pthread_create(&config.input_thread, NULL, handle_input, NULL);

    return 0;
}

int parse_args(int argc, char** argv, irc_config_t* config) {
    int opt;
    while ((opt = getopt(argc, argv, "s:n:c:")) != -1) {
        switch (opt) {
            case 's':
                strncpy(config->server, optarg, INET6_ADDRSTRLEN);
                break;
            case 'n':
                strncpy(config->nick, optarg, MAX_NICK_LENGTH);
                break;
            case 'c':
                strncpy(config->channel, optarg, MAX_CHANNEL_LENGTH);
                break;
            default:
                printf("Usage: %s -s <irc-server> -n <nick> -c <channel>\n", argv[0]);
                return 1;
        }
    }

    return 0;
}
