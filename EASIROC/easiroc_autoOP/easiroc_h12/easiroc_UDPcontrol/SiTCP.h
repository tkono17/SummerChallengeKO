#ifndef SiTCP_H
#define SiTCP_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<fstream>
#include "unistd.h"

struct bcp_header{
  unsigned char type;
  unsigned char command;
  unsigned char id;
  unsigned char length;
  unsigned int address;
};

class SiTCP{

  char* sitcpIpAddr;
  unsigned int tcpPort;
  unsigned int udpPort;
  int udpsock;
  int tcpsock;
  struct sockaddr_in tcpAddr;
  //  struct sockaddr_in udpAddr;

  fd_set rmask, wmask, readfds;
  struct timeval timeout;
  //struct bcp_header sndHeader;
  //unsigned char sndBuf[2048];


 public:
  struct bcp_header sndHeader;
  unsigned char sndBuf[2048];
  unsigned char sndData[256];
  struct sockaddr_in udpAddr;

  SiTCP();
  ~SiTCP();
  bool SetIPPort(char* IpAddr, unsigned int tcp, unsigned int udp);
  bool CreateUDPSock();
  bool CloseUDPSock();
  bool CreateTCPSock();
  bool CloseTCPSock();
  
  int GetTCPSock(){return tcpsock;}
  int GetUDPSock(){return udpsock;}

  void RBCPskeleton(unsigned char type, unsigned char command, unsigned char id, 
		    unsigned char length, unsigned int address);

  int rcvRBCP_ACK(int output);

  bcp_header GetsndHeader() const {return sndHeader;}

};
#endif
