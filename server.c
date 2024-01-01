#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define BUF_SIZE 1024
#define SERVER_PORT 12345
#define CLIENTS_COUNT 1
#define HELLO_INTERVAL 5

struct client_info {
    char ip[16];
    int port;
    bool connected;
};

int main() {
    int number;
    char numberStr[10];

    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t client_addr_size;

    struct client_info clients[CLIENTS_COUNT] = {
        {"192.168.10.202", 54321, false},
        // {"192.168.1.102", 54322, false},
        // {"192.168.1.103", 54323, false}
    };

    server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server is running on port %d\n", SERVER_PORT);

    time_t last_hello_time = 0;
    while (1) {
        if (time(NULL) - last_hello_time >= HELLO_INTERVAL) {
            for (int i = 0; i < CLIENTS_COUNT; i++) {
                if (!clients[i].connected) {
                    struct sockaddr_in temp_client_addr;
                    memset(&temp_client_addr, 0, sizeof(temp_client_addr));
                    temp_client_addr.sin_family = AF_INET;
                    temp_client_addr.sin_addr.s_addr = inet_addr(clients[i].ip);
                    temp_client_addr.sin_port = htons(clients[i].port);

                    sendto(server_socket, "hello", 5, 0, (struct sockaddr *)&temp_client_addr, sizeof(temp_client_addr));
                    printf("Sent 'hello' to %s:%d\n", clients[i].ip, clients[i].port);
                }
            }
            last_hello_time = time(NULL);
        }

        client_addr_size = sizeof(client_addr);
        ssize_t recv_len = recvfrom(server_socket, buffer, BUF_SIZE, 0,
                                    (struct sockaddr *)&client_addr, &client_addr_size);
        if (recv_len == -1) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_len] = '\0';
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);

        for (int i = 0; i < CLIENTS_COUNT; i++) {
            if (strcmp(client_ip, clients[i].ip) == 0 && client_port == clients[i].port && !clients[i].connected) {
                clients[i].connected = true;
                printf("Client %s:%d connected.\n", client_ip, client_port);
            }
        }

        bool all_connected = true;
        for (int i = 0; i < CLIENTS_COUNT; i++) {
            if (!clients[i].connected) {
                all_connected = false;
                break;
            }
        }

        if (all_connected) {
            printf("All clients connected. Proceeding to main communication loop.\n");
            break;
        }
    }

    // Główna pętla komunikacji
    while (1) {
        client_addr_size = sizeof(client_addr);
        ssize_t recv_len = recvfrom(server_socket, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);

        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            printf("Odebrano od klienta: %s\n", buffer);

            // Wysłanie zadania i liczby do klienta
            const char* task = "1";
            sendto(server_socket, task, strlen(task), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            const char* number = "8";
            sendto(server_socket, number, strlen(number), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

            // Oczekiwanie na wynik
            recv_len = recvfrom(server_socket, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);
            if (recv_len > 0) {
                buffer[recv_len] = '\0';
                printf("Wynik od klienta: %s\n", buffer);
            }
        }
        sleep(5000);
    }

    close(server_socket);
    return 0;
}
