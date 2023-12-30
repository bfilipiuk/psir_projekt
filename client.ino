#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192, 168, 10, 204);
unsigned int serverPort = 12345;      
unsigned int localPort = 54321;    

EthernetUDP Udp;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Nie udało się skonfigurować Ethernetu za pomocą DHCP");
    for (;;)
      ;
  }
  Udp.begin(localPort);

  Serial.println("Czekam na wiadomość 'hello' od serwera...");
  waitForHello();
}

void loop() {
  char taskMessage[UDP_TX_PACKET_MAX_SIZE];
  char numberMessage[UDP_TX_PACKET_MAX_SIZE];

  int result;
  int task;
  int number;

  // Zapytanie o zadanie
  Udp.beginPacket(serverIP, serverPort);
  Udp.write("Czekam na zadanie");
  Udp.endPacket();

  // Odbiór ciągu znaków
  int packetSize = Udp.parsePacket();

  if (packetSize) {
    Udp.read(taskMessage, UDP_TX_PACKET_MAX_SIZE);
    taskMessage[packetSize] = 0; // Dodanie znaku końca ciągu
    task = atoi(taskMessage);
    Serial.print("Otrzymano zadanie: ");
    Serial.println(task);
  }

  // if (packetSize) {
  //   Udp.read(stringMessage, UDP_TX_PACKET_MAX_SIZE);
  //   stringMessage[packetSize] = 0; // Dodanie znaku końca ciągu
  //   Serial.print("Otrzymano ciąg znaków: ");
  //   Serial.println(stringMessage);
  // }

  // Oczekiwanie na liczbę
  packetSize = Udp.parsePacket();
  if (packetSize) {
    Udp.read(numberMessage, UDP_TX_PACKET_MAX_SIZE);
    numberMessage[packetSize] = 0; // Dodanie znaku końca ciągu
    number = atoi(numberMessage);
    Serial.print("Otrzymano liczbę: ");
    Serial.println(number);
  }

  switch (task) {
    case 1:
      result = mulitplyBy2(number);
      break;
    case 2:
      result = divideBy2(number);
      break;
  // Odesłanie wyników

  char numberStr[10];
  sprintf(numberStr, "%d", result);
  Udp.beginPacket(serverIP, serverPort);
  Udp.write(numberStr);
  Udp.endPacket();
}

void waitForHello() {
  while (true) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

      if (strcmp(packetBuffer, "hello") == 0) {
        Serial.println("Otrzymano 'hello' od serwera.");
        sendResponseString("ACK: Hello from pracownik");
        Serial.println("Połączono z serwerem");
        break;
      }
    }
  }
}

void sendResponseString(const char *message) {
    Udp.beginPacket(serverIP, serverPort);
    Udp.write(message);
    Udp.endPacket();
}

int mulitplyBy2(int x) {
  return x*2;
}

int divideBy2(int x) {
  return x/2;
}

int add2(int x) {
  return x+2;
}

int subtract2(int x) {
  return x-2;
}

bool checkIfEven(int x) {
  if (x%2 == 0) {
    return true;
  }

  else {
    return false;
  }
}

bool checkIfPierwsza(int x) {
  if(x<2)
		return false;
		
	for(int i=2;i*i<=x;i++)
		if(x%i==0)
			return false;

	return true;
}
