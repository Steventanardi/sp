#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define WEB_ROOT "./webroot"

void handle_request(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Cannot open socket");
        return -1;
    }

    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Cannot bind");
        close(server_socket);
        return -2;
    }

    if (listen(server_socket, 10) == -1) {
        perror("Listen failed");
        close(server_socket);
        return -3;
    }

    printf("HTTP Server started on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_len);
        if (client_socket == -1) {
            perror("Cannot accept connection");
            continue;
        }

        handle_request(client_socket);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

void handle_request(int client_socket) {
    char buf[BUFFER_SIZE];
    int fd;
    int read_bytes;

    read_bytes = read(client_socket, buf, BUFFER_SIZE - 1);
    if (read_bytes > 0) {
        buf[read_bytes] = '\0';

        printf("Request:\n%s\n", buf);

        // Extract the first line with the request method and path
        char *method = strtok(buf, " ");
        char *path = strtok(NULL, " ");
        if (!method || !path) return;

        // Only handle GET requests
        if (strcmp(method, "GET") != 0) {
            dprintf(client_socket, "HTTP/1.1 405 Method Not Allowed\r\n\r\n");
            return;
        }

        // Process path to avoid accessing files outside of the web root
        char resolved_path[256];
        sprintf(resolved_path, "%s%s", WEB_ROOT, path);

        fd = open(resolved_path, O_RDONLY);
        if (fd == -1) {
            dprintf(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n");
            return;
        }

        dprintf(client_socket, "HTTP/1.1 200 OK\r\n\n");
        while ((read_bytes = read(fd, buf, BUFFER_SIZE)) > 0) {
            write(client_socket, buf, read_bytes);
        }

        close(fd);
    }
}
