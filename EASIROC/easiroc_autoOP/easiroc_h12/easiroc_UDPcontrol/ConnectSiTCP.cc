#pragma once
#include "sender.h"
#include "SiTCP.h"
#include <iostream>
#include <fstream>
#include <string>

#include "stdio.h"
#include "stdlib.h"

#define IpAddr "192.168.10.102"
#define tcpport 24
#define udpport 4660

#define binary(bit) strtol(bit,NULL,2)

int main(int argc, char *argv[]){

  SiTCP* sitcp = new SiTCP();
  sitcp -> SetIPPort(IpAddr, tcpport, udpport);
  //  sitcp -> CreateTCPSock();
  sitcp -> CreateUDPSock();
  
  Sender* sender = new Sender();
   std::string filename;
  int menu(0);
  int event(0);
  int i(0);

  int HVDAC;
  int MADC;
   
  //std::cout<<"reset register"<<std::endl;
  //  sender->clear_all(sitcp);
  usleep(200);


 
  while(1){

    menu = 0;  

    std::cout << "" << std::endl;
    std::cout << "--------------------Select Menu--------------------" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "1.HV control" << std::endl;
    std::cout << "2.MADC control" << std::endl;
    std::cout << "3.quit" << std::endl;
    std::cin >> menu;
    
    
 
    if(menu == 1){

      std::cout << "number of DAC 0~65535 "<<std::endl;;
      std::cin >> HVDAC;
      std::cout << HVDAC << std::endl;
      
      
      sender->udp_send(sitcp,0x11111110,HVDAC/256);//higher 8bit
      sender->udp_send(sitcp,0x11111100,HVDAC%256);//lower 8bit
      // usleep(200);
      sender->udp_send(sitcp,0x11111111,1); 
      sender->udp_send(sitcp,0x1111111f,1); 
      break;
   
    }
    
    
    else if(menu == 2){
      std::cout << "input order bit "<<std::endl;;
      std::cin >> MADC;
      std::cout << MADC << std::endl;
      
      
      sender->udp_send(sitcp,0x11111000,MADC);//higher 8bit
      // usleep(200);
      // sender->udp_send(sitcp,0x1111111f,1); 
      break;
    }

  
    else if(menu == 3){
      int rd_data1;
      int rd_data2;
      double rd_data;
      int data;
      data = 0;
      
      rd_data1 = sender->udp_recv(sitcp,0x00000004,data);
      rd_data2 = sender->udp_recv(sitcp,0x00000005,data);
      
      rd_data = rd_data1*256 + rd_data2;
      std::cout <<"MADC_data = "<< rd_data;
      
      rd_data = 4.5 * rd_data/65535;
      printf(" : %.2lfV",rd_data);
      
      break;    
      
    }

    else if(menu == 4){

      
    }
    else if(menu == 5){
      break;

    }

      
    else{
      std::cout<< "" <<std::endl;
   
    }
  }
  
  /* ------------------Finalize------------------ */  
  sitcp->CloseTCPSock();
  sitcp->CloseUDPSock();
  
  delete sitcp;
  delete sender;
  
  return 0;
}
