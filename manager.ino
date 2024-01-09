#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include "tuple_protocol.h"

// Konfiguracja sieci
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
ZsutIPAddress serverIP(192, 168, 0, 47); // Adres IP serwera
unsigned int serverPort = 12345;    // Port serwera
ZsutIPAddress myIP(192, 168, 0, 241);   // Adres IP klienta

ZsutEthernetUDP udp;

void setup() {
  Serial.begin(9600);

  // Inicjalizacja karty sieciowej Ethernet
  ZsutEthernet.begin(mac, myIP);

  // Czekaj na inicjalizację karty Ethernet
  delay(1500);
  Serial.println("Połączono z siecią Ethernet");

  udp.begin(serverPort);
}

void loop() {
  field_t fields[2];
  fields[0].is_actual = TS_YES;
  fields[0].type = TS_INT;
  fields[0].data.int_field = 1; // Przykładowe zadanie
  fields[1].is_actual = TS_YES;
  fields[1].type = TS_INT;
  fields[1].data.int_field = random(100); // Przykładowa liczba

  char packet[1024];
  int packetSize = serializePacket(packet, TS_CMD_OUT, "example_tuple", fields, 2);
  
  udp.beginPacket(serverIP, serverPort);
  udp.write(packet, packetSize);
  udp.endPacket();

  delay(2000); // Czekaj 2 sekundy przed wysłaniem kolejnej krotki
}
