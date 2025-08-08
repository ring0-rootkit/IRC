#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
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
    (void)arg;
    char buf[256];
    char cmdbuf[MAX_MSG_LENGTH];

    while (1) {
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            log_error("Failed to read input.");
            continue;
        }

        // Strip newline character
        buf[strcspn(buf, "\n")] = '\0';

        // Skip empty messages
        if (strlen(buf) == 0) {
            continue;
        }

        // Handle IRC commands starting with '/'
        if (buf[0] == '/') {
            if (strncmp(buf, "/quit", 5) == 0) {
                send_command("QUIT :Leaving\r\n");
                break;
            } else if (strncmp(buf, "/join ", 6) == 0) {
                snprintf(cmdbuf, sizeof(cmdbuf), "JOIN %s\r\n", buf + 6);
                send_command(cmdbuf);
                continue;
            }
        }

        snprintf(cmdbuf, sizeof(cmdbuf), "PRIVMSG #%s :%s\r\n", config.channel, buf);
        send_command(cmdbuf);
    }
    
    return NULL;
}

void* handle_server_response(void* arg) {
    (void)arg;
    char buffer[MAX_MSG_LENGTH];
    int bytes_received;
    
    while ((bytes_received = recv(config.sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        
        // Handle PING messages - IRC servers require PONG responses
        if (strncmp(buffer, "PING", 4) == 0) {
            char pong[MAX_MSG_LENGTH];
            snprintf(pong, sizeof(pong), "PONG%s", buffer + 4);
            send_command(pong);
            log_info("Responded to server PING");
        } else {
            // Print server messages to stdout
            printf("%s", buffer);
            fflush(stdout);
        }
    }
    
    if (bytes_received <= 0) {
        log_error("Server connection lost.");
    }
    
    return NULL;
}

int send_initial_commands(const char* nick, const char* channel) {
    char cmdbuf[MAX_MSG_LENGTH];

    snprintf(cmdbuf, sizeof(cmdbuf), "NICK %s\r\n", nick);
    send_command(cmdbuf);

    snprintf(cmdbuf, sizeof(cmdbuf), "USER %s 0 * :%s\r\n", nick, nick);
    send_command(cmdbuf);

    // Small delay to allow registration
    usleep(100000); // 100ms

    snprintf(cmdbuf, sizeof(cmdbuf), "JOIN #%s\r\n", channel);
    send_command(cmdbuf);

    log_info("Initial IRC commands sent.");
    return 0;
}

int startup(const char* server, const char* nick, const char* channel) {
    config.sockfd = connect_to_server(server);
    if (config.sockfd < 0) {
        log_error("Failed to connect to server.");
        return 1;
    }

    send_initial_commands(nick, channel);

    // Create thread to handle server responses
    if (pthread_create(&config.server_thread, NULL, handle_server_response, NULL) != 0) {
        log_error("Failed to create server response thread.");
        close(config.sockfd);
        return 1;
    }

    // Create thread to handle user input
    if (pthread_create(&config.input_thread, NULL, handle_input, NULL) != 0) {
        log_error("Failed to create input thread.");
        pthread_cancel(config.server_thread);
        close(config.sockfd);
        return 1;
    }

    log_info("IRC client started successfully.");
    return 0;
}

int parse_args(int argc, char** argv, irc_config_t* config) {
    int opt;
    int server_set = 0, nick_set = 0, channel_set = 0;

    while ((opt = getopt(argc, argv, "s:n:c:")) != -1) {
        switch (opt) {
            case 's':
                strncpy(config->server, optarg, INET6_ADDRSTRLEN - 1);
                config->server[INET6_ADDRSTRLEN - 1] = '\0';
                server_set = 1;
                break;
            case 'n':
                strncpy(config->nick, optarg, MAX_NICK_LENGTH - 1);
                config->nick[MAX_NICK_LENGTH - 1] = '\0';
                nick_set = 1;
                break;
            case 'c':
                strncpy(config->channel, optarg, MAX_CHANNEL_LENGTH - 1);
                config->channel[MAX_CHANNEL_LENGTH - 1] = '\0';
                channel_set = 1;
                break;
            default:
                printf("Usage: %s -s <irc-server> -n <nick> -c <channel>\n", argv[0]);
                return 1;
        }
    }

    if (!server_set || !nick_set || !channel_set) {
        printf("Error: All options (-s, -n, -c) are required.\n");
        printf("Usage: %s -s <irc-server> -n <nick> -c <channel>\n", argv[0]);
        return 1;
    }

    return 0;
}
