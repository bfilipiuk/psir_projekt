#ifndef UDP_MANAGER_H
#define UDP_MANAGER_H

#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>

extern ZsutEthernetUDP Udp;

void udp_setup();
int udp_send_packet(unsigned char *buffer, int length);
int udp_receive_packet(char *buffer, int length);

#endif