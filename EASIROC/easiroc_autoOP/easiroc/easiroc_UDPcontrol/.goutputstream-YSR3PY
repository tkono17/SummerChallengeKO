#pragma once
#include "SiTCP.h"


class Sender{

 public:
  Sender();
  ~Sender();

  void udp_send(SiTCP* sitcp, unsigned int address, int data);
  int udp_recv(SiTCP* sitcp, unsigned int address, int data);
  void RBCPpacket_send(SiTCP* sitcp, unsigned int address, unsigned char length, int* data);
  void RBCPpacket_recv(SiTCP* sitcp, unsigned int address, unsigned char length, int* data);
  void RBCP_multi_packet_send(SiTCP* sitcp, unsigned int address, unsigned int total_length, int* data);
  int read_madc(SiTCP* sitcp, int data);
  void clear_all(SiTCP* sitcp);
  void tcp_send(SiTCP* sitcp, char* data);

};
