/* Task 4: Multi-threaded TCP server with authentication (IPC via sockets) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024
#define MAX_CLIENTS 10

/* very simple credential store (demo). Real systems use hashed DB. */
const char *VALID_USER = "admin";
const char *VALID_PASS = "password123";

/* handle one client - runs in its own thread so many clients work at once */
void *handle_client(void *arg) {
    int sock = *(int *)arg;
    free(arg);

    char buffer[BUF_SIZE];
    int authenticated = 0;

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (bytes <= 0) {                     /* client disconnected */
            printf("[server] client on socket %d disconnected\n", sock);
            break;
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';   /* strip newline */
        printf("[server] received: %s\n", buffer);

        /* --- protocol handling --- */
        if (strncmp(buffer, "AUTH ", 5) == 0) {
            char user[64], pass[64];
            /* input validation: expect exactly two fields */
            if (sscanf(buffer + 5, "%63s %63s", user, pass) != 2) {
                send(sock, "ERROR invalid AUTH format\n", 26, 0);
                continue;
            }
            if (strcmp(user, VALID_USER) == 0 && strcmp(pass, VALID_PASS) == 0) {
                authenticated = 1;
                send(sock, "AUTH_OK\n", 8, 0);
            } else {
                send(sock, "AUTH_FAIL\n", 10, 0);
            }
        }
        else if (!authenticated) {
            /* must log in before any other command */
            send(sock, "ERROR not authenticated\n", 24, 0);
        }
        else if (strncmp(buffer, "ECHO ", 5) == 0) {
            char reply[BUF_SIZE];
            snprintf(reply, sizeof(reply), "MSG %s\n", buffer + 5);
            send(sock, reply, strlen(reply), 0);
        }
        else if (strcmp(buffer, "EXIT") == 0) {
            send(sock, "BYE\n", 4, 0);
            break;
        }
        else {
            send(sock, "ERROR unknown command\n", 22, 0);
        }
    }

    close(sock);
    return NULL;
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);   /* create socket */
    if (server_fd < 0) { perror("socket"); exit(1); }

    /* allow quick restart on the same port */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen"); exit(1);
    }

    printf("[server] listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int *sock = malloc(sizeof(int));
        *sock = accept(server_fd, (struct sockaddr *)&client, &len);
        if (*sock < 0) { perror("accept"); free(sock); continue; }

        printf("[server] new client connected from %s\n",
               inet_ntoa(client.sin_addr));

        /* one thread per client -> concurrent connections */
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, sock);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
