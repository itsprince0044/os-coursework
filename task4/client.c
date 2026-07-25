/* Task 4: TCP client for the authentication server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);   /* connect to localhost */

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect"); exit(1);
    }
    printf("Connected to server. Commands: AUTH <user> <pass>, ECHO <msg>, EXIT\n");

    char buffer[BUF_SIZE];
    while (1) {
        printf("> ");
        if (!fgets(buffer, BUF_SIZE, stdin)) break;

        send(sock, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "EXIT", 4) == 0) {
            recv(sock, buffer, BUF_SIZE - 1, 0);   /* read BYE */
            printf("Server: %s", buffer);
            break;
        }

        memset(buffer, 0, BUF_SIZE);
        int bytes = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (bytes <= 0) { printf("Server closed connection\n"); break; }
        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}
