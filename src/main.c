#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "irc.h"
#include "network.h"
#include "utils.h"

void cleanup(int sig) {
    (void)sig;
    
    log_info("Shutting down IRC client...");
    
    // Send QUIT command to server
    if (config.sockfd > 0) {
        send_command("QUIT :Client shutting down\r\n");
        
        // Cancel threads
        pthread_cancel(config.input_thread);
        pthread_cancel(config.server_thread);
        
        // Close socket
        close(config.sockfd);
    }
    
    exit(0);
}

int main(int argc, char** argv) {
    // Initialize config structure
    memset(&config, 0, sizeof(config));
    
    if (parse_args(argc, argv, &config) != 0) {
        return 1;
    }

    // Set up signal handlers for clean shutdown
    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);

    printf("Connecting to %s as %s, joining #%s\n", 
           config.server, config.nick, config.channel);

    if (startup(config.server, config.nick, config.channel) != 0) {
        log_error("Startup failed.");
        return 1;
    }

    // Wait for input thread to complete (happens when user types /quit)
    pthread_join(config.input_thread, NULL);
    
    // Cancel server thread and cleanup
    pthread_cancel(config.server_thread);
    close(config.sockfd);
    
    log_info("IRC client terminated.");
    return 0;
}
