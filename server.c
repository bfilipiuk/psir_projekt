#include "tuple_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_TUPLES 10

// Definicja tablicy krotek
field_t tuples[MAX_TUPLES][2]; // Każda krotka składa się z 2 pól
int tupleCount = 0;

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_size;
    field_t fields[2]; // Przyjmujemy, że każda krotka ma 2 pola

    // Utworzenie gniazda UDP
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Błąd przy tworzeniu gniazda");
        exit(EXIT_FAILURE);
    }

    // Konfiguracja adresu serwera
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Przypisanie adresu do gniazda
    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Błąd przy przypisywaniu adresu do gniazda");
        exit(EXIT_FAILURE);
    }

    printf("Serwer UDP uruchomiony na porcie %d.\n", SERVER_PORT);

    while (1) {
        addr_size = sizeof(clientAddr);
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddr, &addr_size);
        if (recv_len > 0) {
            buffer[recv_len] = '\0';

            int command, num_fields;
            char tuple_name[32];
            deserializePacket(buffer, &command, tuple_name, fields, &num_fields);

            // Dodanie krotki do tablicy
            if (tupleCount < MAX_TUPLES) {
                tuples[tupleCount][0] = fields[0]; // Pierwsze pole
                tuples[tupleCount][1] = fields[1]; // Drugie pole
                tupleCount++;
            } else {
                printf("Brak miejsca dla nowych krotek.\n");
            }

            // Wyświetlanie krotek
            for (int i = 0; i < tupleCount; i++) {
                printf("Krotka %d: Zadanie %d, Liczba %d\n", i, tuples[i][0].data.int_field, tuples[i][1].data.int_field);
            }
        }
    }

    return 0;
}
