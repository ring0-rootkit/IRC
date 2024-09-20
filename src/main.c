#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <getopt.h>

#define MAX_MSG_LENGTH 512
#define MAX_NICK_LENGTH 32
#define MAX_CHANNEL_LENGTH 64

typedef struct {
    char nick[MAX_NICK_LENGTH];
    char channel[MAX_CHANNEL_LENGTH];
    char server[INET6_ADDRSTRLEN];
} irc_config_t;

irc_config_t config;

void cleanup(int sig) {
    (void)sig;
    if (config.client_sockfd != 0) {
        close(config.client_sockfd);
    }
    exit(0);
}

void send_command(char* cmd) {
    send(config.sockfd, cmd, strlen(cmd), 0);
}

void handle_input(const char* channel) {
    char buf[256];                  /* User input buffer */
    char cmdbuf[MAX_MSG_LENGTH];    /* Command buffer */
    pid_t child = fork();           /* To prevent blocking reads from stdin */

    if (child == 0) {
        while (1) {
            read(STDIN_FILENO, buf, sizeof(buf));

            snprintf(cmdbuf, sizeof(cmdbuf), "PRIVMSG #%s :%s\r\n", channel, buf);
            send_command(cmdbuf);

            printf("\nMessage sent!\n");
        }
    }
}

int connect_to_server(char* ip) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error: Could not open socket.\n");
        return sockfd;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6667);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        printf("Error: Invalid address \"%s\"\n", ip);
        return 1;
    }

    config.client_sockfd = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (config.client_sockfd < 0) {
        printf("Error: Connection failed.\n");
        return 1;
    }

    return sockfd;
}

int send_initial_commands(char* nick, char* channel) {
    char cmdbuf[MAX_MSG_LENGTH];

    snprintf(cmdbuf, sizeof(cmdbuf), "NICK %s\r\n", nick);
    send_command(cmdbuf);

    snprintf(cmdbuf, sizeof(cmdbuf), "USER %s 0 * :%s\r\n", nick, nick);
    send_command(cmdbuf);

    snprintf(cmdbuf, sizeof(cmdbuf), "JOIN #%s\r\n", channel);
    send_command(cmdbuf);

    return 0;
}

int startup(char* server, char* nick, char* channel) {
    struct addrinfo hints;
    struct addrinfo* res;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(server, NULL, &hints, &res)) != 0) {
        printf("Error: Could not connect to %s\n", server);
        return status;
    }

    /* Find an IPv4 address */
    struct addrinfo* p;
    for (p = res; p != NULL; p = p->ai_next) {
        void* addr;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);

            inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
            break;
        }
    }

    freeaddrinfo(res);

    config.sockfd = connect_to_server(ipstr);
    if (config.sockfd < 0) {
        return 1;
    }

    send_initial_commands(nick, channel);
    handle_input(channel);

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

int main(int argc, char** argv) {
    if (parse_args(argc, argv, &config) != 0) {
        return 1;
    }

    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);

    startup(config.server, config.nick, config.channel);

    return 0;
}
