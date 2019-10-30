// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT        8080
#define MAXLINE     1024

// Driver code
int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 ){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        unsigned int len, n;
        char buffer[MAXLINE];
        printf("waiting message..\n");

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                     &len);
        buffer[n] = '\0';

        char *ip = inet_ntoa(cliaddr.sin_addr);

        printf("Client %s: %s\n", ip, buffer);
        sprintf(buffer, "%s from %s\n", buffer, ip);

        sendto(sockfd, (const char *)buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
               len);
    }
    return 0;
}


//Code: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
