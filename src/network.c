#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "network.h"
#include "utils.h"

int connect_to_server(const char* hostname_or_ip, const char* port) {
    int sockfd = -1;
    struct addrinfo hints, *result, *rp;
    if (strlen(port) == 0) {
	    port = "6667";
    }
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4 only for now
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    // Resolve hostname/IP to address info
    status = getaddrinfo(hostname_or_ip, port, &hints, &result);
    if (status != 0) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "getaddrinfo failed: %s", gai_strerror(status));
        log_error(error_msg);
        return -1;
    }

    // Try each address until successful connection
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
            continue;  // Try next address
        }

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;  // Success
        }

        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(result);

    if (sockfd == -1) {
        log_error("Failed to connect to any address.");
        return -1;
    }

    char ip_str[INET_ADDRSTRLEN];
// I HATE MACOS
#if defined(__linux__)
    printf("I love you fellow linux user <3\n");
    struct sockaddr_in* addr_in = (struct sockaddr_in*)rp->ai_addr;
    if(!inet_ntop(AF_INET, &addr_in->sin_addr, ip_str, INET_ADDRSTRLEN)) {
	printf("error occured when trying to convert address to display format\n");
	exit(1);
    };
#endif

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Connected to %s (%s:%s)", hostname_or_ip, ip_str, port);
    log_info(log_msg);

    return sockfd;
}
