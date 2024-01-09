#ifndef UDP_MANAGER_H
#define UDP_MANAGER_H

#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>

extern ZsutEthernetUDP Udp;

// this function sets up udp connection
void udp_setup();

// this function sends UDP message
int udp_send_packet(unsigned char *buffer, int length);

// this function receives UDP message
int udp_receive_packet(char *buffer, int length);

#endif