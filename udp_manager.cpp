#include "udp_manager.h"

#define SERVER_PORT 12345
#define LOCAL_PORT 5001

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

unsigned int localPort = LOCAL_PORT;
unsigned int serverPort = SERVER_PORT;
ZsutIPAddress localIP (192, 168, 0, 241);
ZsutIPAddress serverIP(192, 168, 0, 47);

ZsutEthernetUDP udp;

void udp_setup() {
    ZsutEthernet.begin(mac, localIP);
    delay(1500);
    Serial.println("Połączono z siecią Ethernet");
    udp.begin(localPort);
}

int udp_send_packet(unsigned char *buffer, int length) {
    udp.beginPacket(serverIP, serverPort);
    udp.write(buffer, length);
    udp.endPacket();

    return 1;
}
