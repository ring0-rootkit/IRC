#ifndef IRC_H
#define IRC_H

#include <pthread.h>

#define MAX_MSG_LENGTH 512
#define MAX_NICK_LENGTH 32
#define MAX_CHANNEL_LENGTH 64

typedef struct {
    char nick[MAX_NICK_LENGTH];
    char channel[MAX_CHANNEL_LENGTH];
    char server[INET6_ADDRSTRLEN];
    int sockfd;
    pthread_t input_thread;
} irc_config_t;

extern irc_config_t config;

void send_command(const char* cmd);
void handle_input(void* arg);
int send_initial_commands(const char* nick, const char* channel);
int startup(const char* server, const char* nick, const char* channel);
int parse_args(int argc, char** argv, irc_config_t* config);
void cleanup(int sig);

#endif // IRC_H
