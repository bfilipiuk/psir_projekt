#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int is_prime(int number) {
    if (number <= 1) return 0;
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_size;
    int num;

    // Utworzenie gniazda UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Błąd przy tworzeniu gniazda");
        exit(EXIT_FAILURE);
    }

    // Konfiguracja adresu serwera
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu do gniazda
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Błąd przy przypisywaniu adresu do gniazda");
        exit(EXIT_FAILURE);
    }

    printf("Serwer UDP uruchomiony na porcie %d\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        // Odbiór danych od klienta
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_size);

        num = atoi(buffer);
        printf("Otrzymano liczbę: %d\n", num);

        // Sprawdzenie, czy liczba jest pierwsza
        sprintf(buffer, "%d jest %s\n", num, is_prime(num) ? "pierwsza" : "nie pierwsza");

        // Wysłanie odpowiedzi do klienta
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_size);
    }

    close(sockfd);
    return 0;
}

