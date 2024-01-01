#include <Zsut.h>
#include <ZsutClient.h>
#include <ZsutDhcp.h>
#include <ZsutDns.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetClient.h>
#include <ZsutEthernetServer.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <ZsutIPAddress.h>
#include <ZsutUdp.h>

byte mac[] = {0x08, 0x00, 0x27, 0x13, 0x93, 0xEE};
ZsutIPAddress clientIP(192, 168, 10, 202);
ZsutIPAddress serverIP(192, 168, 10, 204);
unsigned int serverPort = 12345;      
unsigned int localPort = 54321;    

ZsutEthernetUDP Udp;

void waitForHello() {
  while (true) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

      if (strcmp(packetBuffer, "hello") == 0) {
        Serial.println("Otrzymano 'hello' od serwera.");
        sendResponseString("ACK: Hello from pracownik");
        Serial.println("Polaczono z serwerem");
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

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // if (ZsutEthernet.begin(mac) == 0) {
  //   Serial.println("Nie udało się skonfigurować Ethernetu za pomocą DHCP");
  //   for (;;)
  //     ;
  // }

  ZsutEthernet.begin(mac, clientIP);
  Udp.begin(localPort);

  Serial.println("Czekam na wiadomosc 'hello' od serwera...");
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

  // Odbiór taska
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Udp.read(taskMessage, UDP_TX_PACKET_MAX_SIZE);
    taskMessage[packetSize] = 0; // Dodanie znaku końca ciągu
    task = atoi(taskMessage);
    Serial.print("Otrzymano zadanie: ");
    Serial.println(task);

    // Oczekiwanie na liczbę
    delay(100);
    packetSize = Udp.parsePacket();
    if (packetSize) {
      Udp.read(numberMessage, UDP_TX_PACKET_MAX_SIZE);
      numberMessage[packetSize] = 0; // Dodanie znaku końca ciągu
      number = atoi(numberMessage);
      Serial.print("Otrzymano liczbe: ");
      Serial.println(number);

      // Wysłanie potwierdzenia odbioru
      char confirmMsg[] = "Zadanie odebrane";
      Udp.beginPacket(serverIP, serverPort);
      Udp.write(confirmMsg);
      Udp.endPacket();

      // Obliczenia
      switch (task) {
        case 1:
          result = mulitplyBy2(number);
          break;
        case 2:
          result = divideBy2(number);
          break;
        default:
          Serial.println("Nieznane zadanie");
          return;
      }

      // Odesłanie wyniku
      char resultStr[10];
      sprintf(resultStr, "%d", result);
      Udp.beginPacket(serverIP, serverPort);
      Udp.write(resultStr);
      Udp.endPacket();
      Serial.print("Wyslano wynik: ");
      Serial.println(resultStr);

    } else {
      Serial.println("Nie otrzymano liczby");
      return; // Przerwanie pętli, ponieważ nie otrzymano liczby
    }
  } else {
    Serial.println("Nie otrzymano zadania");
    return; // Przerwanie pętli, ponieważ nie otrzymano zadania
  }
  delay(3000);
}
