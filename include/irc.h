#ifndef IRC_H
#define IRC_H

#include <pthread.h>
#include <netinet/in.h>

#define MAX_MSG_LENGTH 512
#define MAX_NICK_LENGTH 32
#define MAX_CHANNEL_LENGTH 64
#define MAX_PORT_SIZE 6

typedef struct {
    char nick[MAX_NICK_LENGTH];
    char channel[MAX_CHANNEL_LENGTH];
    char server[INET6_ADDRSTRLEN];
    char port[MAX_PORT_SIZE];
    int sockfd;
    pthread_t input_thread;
    pthread_t server_thread;
} irc_config_t;

extern irc_config_t config;

void send_command(const char* cmd);
void* handle_input(void* arg);
void* handle_server_response(void* arg);
int send_initial_commands(const char* nick, const char* channel);
int startup(const char* server, const char* port, const char* nick, const char* channel);
int parse_args(int argc, char** argv, irc_config_t* config);
void cleanup(int sig);

#endif
