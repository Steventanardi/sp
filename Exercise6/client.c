#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
import sys

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed");
        return 1;
    }

    printf("Connected\n");

    // Communicate with server
    while (1) {
        printf("Enter message : ");
        fgets(message, BUFFER_SIZE, stdin);

        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return 1;
        }

        // Receive a reply from the server
        if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
            puts("recv failed");
            break;
        }

        puts("Server reply :");
        puts(server_reply);
    }

    close(sock);
    return 0;
}
