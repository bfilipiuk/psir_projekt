#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 1, 100); // Zmień na adres IP serwera
unsigned int localPort = 12345; // Zmień na port serwera
unsigned int remotePort = 54321; // Port, z którego będzie wysyłać klient

EthernetUDP Udp;

void setup() {
    Ethernet.begin(mac);
    Udp.begin(localPort);
    Serial.begin(9600);
}

void loop() {
    char buffer[10];
    int number = random(1, 100); // Losowanie liczby do sprawdzenia
    sprintf(buffer, "%d", number);

    Udp.beginPacket(server, remotePort);
    Udp.write(buffer);
    Udp.endPacket();

    delay(5000); // Oczekiwanie 5 sekund przed wysłaniem kolejnej liczby
}
