#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include "tuple_protocol.h"
#include "udp_manager.h"

field_t tuple[2];

// Konfiguracja sieci
// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// ZsutIPAddress serverIP(192, 168, 0, 47); // Adres IP serwera
// unsigned int serverPort = 12345;    // Port serwera
// ZsutIPAddress myIP(192, 168, 0, 241);   // Adres IP klienta

// ZsutEthernetUDP udp;

void setup() {
  Serial.begin(9600);

  udp_setup();
  // Inicjalizacja karty sieciowej Ethernet
  // ZsutEthernet.begin(mac, myIP);

  // Czekaj na inicjalizację karty Ethernet
  // delay(1500);
  // Serial.println("Połączono z siecią Ethernet");

  // udp.begin(serverPort);
}

void loop() {
  // field_t fields[2];
  // fields[0].is_actual = TS_YES;
  // fields[0].type = TS_INT;
  // fields[0].data.int_field = 1; // Przykładowe zadanie
  // fields[1].is_actual = TS_YES;
  // fields[1].type = TS_INT;
  // fields[1].data.int_field = random(100); // Przykładowa liczba

  // initializing tuple

  initializeTuple(tuple, 1, random(100));

  // sending tuple to server (tuple_space)

  int result = ts_out("example_tuple", tuple, 2);
    if (result == 1) {
      Serial.print("Krotka wyslana pomyslnie: [");
      Serial.print(tuple[0].data.int_field); // Zakładając, że field jest typu int
      Serial.print(", ");
      Serial.print(tuple[1].data.int_field); // Zakładając, że field jest typu int
      Serial.println("]");
    } else {
      Serial.println("Blad wysylania krotki");
    }

  // int result = ts_out(udp, serverIP, serverPort, "example_tuple", fields, 2);

  // if (result == TS_SUCCESS) {
  //     Serial.println("Krotka wysłana pomyślnie");
  // } else {
  //     Serial.println("Błąd wysyłania krotki");
  // }

  // char packet[1024];
  // int packetSize = serializePacket(packet, TS_CMD_OUT, "example_tuple", fields, 2);
  
  // udp.beginPacket(serverIP, serverPort);
  // udp.write(packet, packetSize);
  // udp.endPacket();

  delay(2000); // Czekaj 2 sekundy przed wysłaniem kolejnej krotki
}
