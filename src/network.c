#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "network.h"
#include "utils.h"

int connect_to_server(const char* ip) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        log_error("Failed to create socket.");
        return sockfd;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6667);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        log_error("Invalid address.");
        return 1;  // Should return -1 for consistency
    }
    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log_error("Failed to connect to server.");
        return 1;  // Should return -1 and close socket
    }

    return sockfd;
}
