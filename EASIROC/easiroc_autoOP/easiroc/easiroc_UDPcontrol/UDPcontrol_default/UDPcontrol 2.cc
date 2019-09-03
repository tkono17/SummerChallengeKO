//#pragma once
#include "sender.h"
#include "SiTCP.h"
#include <iostream>
#include <fstream>
#include <string>

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define IpAddr "192.168.10.16"
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
 
  int HVDAC(0);
  int MADC(0);
  int MUX(0);
  double HV(0);
  double rd_data(0);
  float DACdata[63];

  //std::cout<<"reset register"<<std::endl;
  //  sender->clear_all(sitcp);
  usleep(200);

  float HVC_1 = 412.6; //new
  float HVC_2 = 801.5;
  //float HVC_1 = 483.183;//old
  //float HVC_2 = 780;
  // HVDAC =HVC_1 * HV + HVC_2;            convert HV to DAC bit

  float ADC2HV = 0.00208;
  //  rd_data = ADC2HV * rd_data;          convert ADC bit to HV

  float ADC2uA = 0.034;
  //  rd_data = ADC2uA * rd_data;          convert ADC bit to HVcurrent

  float ADC2V = 0.0000685;
  //rd_data = ADC2V * rd_data;             convert ADC bit to inputDAC Voltage

  float ADC2K = 4500;
  //rd_data = (rd_data - 9620.24)/35.3094; //convert ADC bit to TEMP(C)
 
 
  while(1){

    menu = 0;  
    sender->udp_send(sitcp,0x00000012,248); //Set ADC rate to 50Hz
    sender->udp_send(sitcp,0x0000001f,0);
    std::cout << "" << std::endl;
    std::cout << "--------------------Select Menu--------------------" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "1.HV control" << std::endl;
    std::cout << "2.HV status" << std::endl;
    std::cout << "3.Input DAC scan" << std::endl;
    std::cout << "4.Read TEMP." << std::endl;
    std::cout << "5.Exit" << std::endl;
    std::cout << "6.Read & Write TEMP." << std::endl;
    std::cout << "8.MUX control" << std::endl;
    std::cout << "9.Read MADC" << std::endl;
    std::cin >> menu;
    

 
    if(menu == 1){
      
    	std::cout << "input 0.00~90.00V "<<std::endl;; //"input 0.00~90.00V "と表示
	std::cin >> HV; //　(1)HVの値を打って、読み込む
	//std::cout << HV << std::endl;

	//	while(1){
	
	HVDAC = HVC_1 * HV + HVC_2;        //change HV to DAC bit
	std::cout<<HVDAC<<std::endl;
	sender->udp_send(sitcp,0x00000010,HVDAC/256);//Set higher 8bit to FPGA reg
	sender->udp_send(sitcp,0x00000011,HVDAC%256);//lower 8bit
	sender->udp_send(sitcp,0x0000001e,1); //Start DAC control
	sleep(1);

	//std::cout << "Bias voltage" << std::endl;
	rd_data = sender->read_madc(sitcp,3);//(2)Read HV-ADC[real] data       
	//std::cout <<"MADC_data = "<< rd_data;
	//rd_data = ADC2HV * rd_data; //convert ADC bit to HV
	//printf("Bias voltage : %.2lfV\n\n",rd_data);
	//double Bias = rd_data;   
	//std::cout << "Bias" << std::endl; 

	
	// //if(rd_data < HV + 0.02  && rd_data > HV - 0.02){
	// if(rd_data < HV + 1.0  && rd_data > HV - 1.0){
	//   //break;      
	// }
	// else if(rd_data>HV){
	//    HV = HV - (rd_data - HV);      
	//    //rd_data = rd_data - (rd_data - HV)/2;    
	// }
	// else if(rd_data<HV){
	//    HV = HV + (HV - rd_data); 
	//    //rd_data = rd_data + (HV - rd_data)/2;       
	// }
	std::cout <<"Set V : "<< HV <<"   Real HV : " << rd_data << std::endl;      
	//	}	
    }
    
    else if(menu == 2){
      std::cout<<std::endl << "Bias voltage";
      rd_data = sender->read_madc(sitcp,3);//Read ADC data      
      //	std::cout <<"MADC_data = "<< rd_data;
      rd_data = ADC2HV * rd_data; //convert ADC bit to HV
      printf(" : %.2lfV\n",rd_data);
      
      rd_data = sender->read_madc(sitcp,4);//Read ADC data      
      //std::cout <<"MADC_data = "<< rd_data;
      rd_data = ADC2uA * rd_data;   //convert ADC bit to HVcurrent
      printf("Bias current : %.2lfuA",rd_data);
    }
    else if(menu == 4){
      
      rd_data = sender->read_madc(sitcp,5);//Read ADC data      
      ////std::cout <<"MADC_data = "<< rd_data;
      rd_data = (rd_data - 9620.24)/35.3094; //convert ADC bit to TEMP(C)
      printf("TEMP1 : %.2lfC\n",rd_data);
      
      rd_data = sender->read_madc(sitcp,0);//Read ADC data   
      ////	std::cout <<"MADC_data = "<< rd_data;
      //rd_data = (rd_data - 9620.24)/35.3094; //convert ADC bit to TEMP(C)
      printf("TEMP2raw : %.2lfC\n",rd_data);
      
      rd_data = sender->read_madc(sitcp,0);//Read ADC data   
      ////	std::cout <<"MADC_data = "<< rd_data;
      rd_data = (rd_data - 9620.24)/35.3094; //convert ADC bit to TEMP(C)
      printf("TEMP2 : %.2lfC",rd_data);

    }
    else if(menu == 6){
     
	//fileopen("temp.txt");
	FILE *outputfile;         // 出力ストリーム
	outputfile = fopen("temp.txt", "w");  // ファイルを書き込み用にオープン(開く)
	if (outputfile == NULL) {          // オープンに失敗した場合
	  printf("cannot open\n");         // エラーメッセージを出して
	  exit(1);                         // 異常終了
	}
	int i;
	//for(i = 0; i <= 1000000; i = i+1) { //1000000 + 1 =  1000001
	//for(i = 0; i <= 0; i = i+1) { //1kaibun
	for(i = 0; i <= 9999; i = i+1) { //
	//for(i = 0; i <= 999999; i = i+1) { //999999 + 1 =  1000000
	  rd_data = sender->read_madc(sitcp,0);//Read ADC data   
	  //	std::cout <<"MADC_data = "<< rd_data;
	  //rd_data = (rd_data - 9620.24)/35.3094; //convert ADC bit to TEMP(C)
	  printf("TEMP2 : %.2lfC\n",rd_data);
	  
	  fprintf(outputfile,"%f\n",rd_data); // ファイルに書く
	  //fprintf(outputfile, rd_data); // ファイルに書く

	}
      //もしくはread_temp,temp=(temp+temp_read)/i;
	fclose(outputfile);          // ファイルをクローズ(閉じる)
	return 0;

    }
    else if(menu == 9){
     

      std::cout << "" << std::endl;
      std::cout << "--------------------Select ADC ch--------------------" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "1.Bias_voltage" << std::endl;
      std::cout << "2.Bias_current" << std::endl;
      std::cout << "3.inputDAC_1" << std::endl;
      std::cout << "4.inputDAC_2" << std::endl;
      std::cout << "5.TEMP_1" << std::endl;
      std::cout << "6.TEMP_2" << std::endl;
      std::cin >> MADC;
      
      
      switch(MADC){
      case 1:
	std::cout << "Bias voltage read" << std::endl;

	rd_data = sender->read_madc(sitcp,3);//Read ADC data 
      	rd_data = ADC2HV * rd_data; //convert ADC bit to HV
	printf(" : %.2lfV",rd_data);

	break;

      case 2:
	std::cout << "Bias current read" << std::endl;

	rd_data = sender->read_madc(sitcp,4);//Read ADC data 
      	rd_data = ADC2uA * rd_data;   //convert ADC bit to HVcurrent
	printf(" : %.2lfuA",rd_data);

      break;

      case 3:
      case 4:
	std::cout << "Input DAC read" << std::endl;

	rd_data = sender->read_madc(sitcp,MADC-2);//Read ADC data 
	rd_data = ADC2V * rd_data; //convert ADC bit to V
	printf(" : %.2lfV",rd_data);

      break;

      case 5:
	std::cout << "TEMP read" << std::endl;

	rd_data = sender->read_madc(sitcp,5);//Read ADC data 
	rd_data = ADC2K * rd_data/65535/2.4; //convert ADC bit to TEMP(K)
	rd_data = rd_data - 273;     //convert K to C
	printf(" : %.2lfC",rd_data);


      break;

      case 6:
	std::cout << "TEMP read" << std::endl;

	rd_data = sender->read_madc(sitcp,0);//Read ADC data 
	rd_data = ADC2K * rd_data/65535/2.4; //convert ADC bit to TEMP(K)
	rd_data = rd_data - 273;     //convert K to C
	printf(" : %.2lfC",rd_data);

      break;

      default:
 	break;
      }          
    }

  
    else if(menu == 3){
     
      //std::cout << "Bias voltage" << std::endl;
      rd_data = sender->read_madc(sitcp,3);//Read ADC data       
      //std::cout <<"MADC_data = "<< rd_data;
      rd_data = ADC2HV * rd_data; //convert ADC bit to HV
      printf("Bias voltage : %.2lfV\n\n",rd_data);
      double Bias = rd_data;
      
      std::cout << "Input DAC scan" << std::endl <<std::endl;
      int i=0;
      for(i=0;i<33;i++){
	if(i%2 == 0 && i < 16){
	  MUX = 199 - i/2;      
	}
	else if(i%2 == 1 && i < 16){
	  MUX = 207 - i/2;      
	}
	else if(i%2 == 0 && i < 32 && i > 15){
	  MUX = 55 - (i -16)/2;      
	}
	else if(i%2 == 1 && i < 32 && i > 15){
	  MUX = 63 - (i -16)/2;      
	}
	else if(i == 32){
	  MUX = 0;
	}
	//std::cout << MUX;
	sender->udp_send(sitcp,0x00000013,MUX);
	usleep(2000);                          
	if(i<32){
	rd_data = sender->read_madc(sitcp,1);//Read ADC data      
	//std::cout <<"MADC_data = "<< rd_data;
	rd_data = ADC2V * rd_data; //convert ADC bit to V
	printf("ch%d = %.2lfV   ",i,rd_data);
	printf("%d = %.2lfV     ",i,Bias - rd_data);

	rd_data = sender->read_madc(sitcp,2);//Read ADC data        
	//std::cout <<"MADC_data = "<< rd_data;
	rd_data = ADC2V * rd_data; //convert ADC bit to V
	printf("ch%d = %.2lfV   ",i+32,rd_data);
	printf("%d = %.2lfV\n",i+32,Bias - rd_data);

	}
      }
    }

    else if(menu == 8){

      std::cout << "input DAC ch ? (0~32 : 32 = no connection) "<<std::endl;;
      std::cin >> MUX;
      std::cout << MUX << std::endl; 
      if(MUX%2 == 0 && MUX < 16){
	MUX = 199 - MUX/2;      
      }
      if(MUX%2 == 1 && MUX < 16){
	MUX = 207 - MUX/2;      
      }
      if(MUX%2 == 0 && MUX < 32 && MUX > 15){
	MUX = 55 - (MUX -16)/2;      
      }
      if(MUX%2 == 1 && MUX < 32 && MUX > 15){
	MUX = 63 - (MUX -16)/2;      
      }
      else if(MUX == 32){
	MUX = 0;
      }
      
      std::cout << MUX << std::endl;
      sender->udp_send(sitcp,0x00000013,MUX);
     
           
    }

    else if(menu == 7){
      
      std::ofstream out("data.dat");
      out.close();
      
      while(1){
	
	std::ofstream outf(("data.dat"),std::ios::app);
	if(!outf){
	  std::cout<<"Error Open File"<<std::endl;
	}
	
	outf<<11111111<<std::endl;
	
	
	rd_data = sender->read_madc(sitcp,3);//Read ADC data      
	//	std::cout <<"MADC_data = "<< rd_data;
	rd_data = ADC2HV * rd_data; //convert ADC bit to HV
	printf("Bias voltage : %.2lfV\n",rd_data);
	double Bias = rd_data;    
	
	outf<<rd_data<<std::endl;
        
	rd_data = sender->read_madc(sitcp,4);//Read ADC data      
	//std::cout <<"MADC_data = "<< rd_data;
	rd_data = ADC2uA * rd_data;   //convert ADC bit to HVcurrent
	printf("Bias current : %.2lfuA\n",rd_data);
	
	outf<<rd_data<<std::endl;
	
	//std::cout << "Input DAC scan" << std::endl <<std::endl;
	int i=0;
	for(i=0;i<33;i++){
	  if(i%2 == 0 && i < 16){
	    MUX = 199 - i/2;      
	  }
	  else if(i%2 == 1 && i < 16){
	    MUX = 207 - i/2;      
	  }
	  else if(i%2 == 0 && i < 32 && i > 15){
	    MUX = 55 - (i -16)/2;      
	  }
	  else if(i%2 == 1 && i < 32 && i > 15){
	    MUX = 63 - (i -16)/2;      
	  }
	  else if(i == 32){
	    MUX = 0;
	  }
	  //std::cout << MUX;
	  sender->udp_send(sitcp,0x00000013,MUX);
	  usleep(2000);                          
	  if(i<32){
	    rd_data = sender->read_madc(sitcp,1);//Read ADC data      
	    //std::cout <<"MADC_data = "<< rd_data;
	    rd_data = ADC2V * rd_data; //convert ADC bit to V
	    printf("ch%d = %.2lfV   ",i,rd_data);
	    printf("%d = %.2lfV     ",i,Bias - rd_data);
	    DACdata[i] = rd_data;
	    
	    rd_data = sender->read_madc(sitcp,2);//Read ADC data        
	    //std::cout <<"MADC_data = "<< rd_data;
	    rd_data = ADC2V * rd_data; //convert ADC bit to V
	    printf("ch%d = %.2lfV   ",i+32,rd_data);
	    printf("%d = %.2lfV\n",i+32,Bias - rd_data);
	    DACdata[i+32] = rd_data;
	  }
	}
	int k = 0;
	while(k != 64){
	  outf<<DACdata[k]<<std::endl;
	  k++;
	}
	outf.close();
	sleep(300);      
      }
    }     
    else if(menu == 5){
      break;

    }

      
    else{
      std::cout<< "" <<std::endl;
   
    }
 
  }
    
  /* ------------------Finalize------------------ */  
  
  sender->udp_send(sitcp,0x00000010,0);//Set higher 8bit to FPGA reg
  sender->udp_send(sitcp,0x00000011,0);//lower 8bit
  sender->udp_send(sitcp,0x0000001e,1); //Start DAC control
  
  sitcp->CloseUDPSock();
  
  delete sitcp;
  delete sender;
  
  return 0;
}
