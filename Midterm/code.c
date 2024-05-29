#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024

void start_http_server(SOCKET server_socket);
void handle_client_request(SOCKET client_socket);

int main() {
    WSADATA wsa_data;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // Create a socket
    if((server_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket : %d" , WSAGetLastError());
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        closesocket(server_socket);
        return 1;
    }

    // Listen to incoming connections
    listen(server_socket, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while((client_socket = accept(server_socket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        handle_client_request(client_socket);
    }

    if (client_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d" , WSAGetLastError());
        return 1;
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

void handle_client_request(SOCKET client_socket) {
    char *message, client_reply[BUFFER_SIZE];
    int recv_size;

    message = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n\n<html><body><h1>Hello, World!</h1><p>This is me 林源茂 111110542 資工二<br>Thankyou for Visiting my HTTP Server Project !!</p></body></html>";
    send(client_socket, message, strlen(message), 0);

    if((recv_size = recv(client_socket, client_reply, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
        puts("recv failed");
    }

    puts("Reply sent\n");
    closesocket(client_socket);
}


