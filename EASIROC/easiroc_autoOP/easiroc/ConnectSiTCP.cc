#include"ConnectSiTCP.h"
#include"test.hh"

#include<fstream>
#include<ios>
#include<iostream>
#include<time.h>

static const int  SiTCP_PORT       = 24;

extern unsigned int slowdata[sizeByte_SC];
extern unsigned int ReadSCdata[sizeByte_RSC];
extern unsigned int Probedata[sizeByte_PSC];
extern unsigned int Pindata[sizeByte_PIN];

int ForceStop = 0;
int EndADC = 0;

void ForceStop_FromCtrl_C(int signal);
int  receive(int sock, char* data_buf, unsigned int *ReadLength);
int  WriteData(int socket, unsigned int data);
int  ReadData(int socket, unsigned int signal, unsigned int data);
void ADCStop(int socket);

int DebugFPGA(int socket);
int TransmitSC(int socket);
int TransmitReadSC(int socket);
int TransmitProbe(int socket);
int ContinuousADC(int socket);
int ContinuousADC_ver2(int socket);
void ADCOneCycle(int socket, std::ofstream& file);
void ADCOneCycle_wHeader(int socket, std::ofstream& file);
int ADCOneCycle_wHeader_ver2(int socket, std::ofstream& file);
void MHTDC_debug(int socket);
int Debugging(int socket);
int chipselect();

int main(int argc, char* argv[]){
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "./easiroc [IP Address of SOY] [DAQ mode]" << std::e\
ndl;
    std::cout << "   DAQ mode : 1 MHTDC\n";
    std::cout << "            : 2 ADC\n";
    std::cout << "            : 3 ADC & MHTDC" << std::endl;
    exit(-1);
  }

  unsigned int daq_mode = (unsigned int)atoi(argv[2]);

  const char* SiTCP_MASTER_IP = argv[1];

  int sock = 0;
  struct sockaddr_in SiTCP_ADDR;
  const char*  IP;
  unsigned int port;

  //Initialize -------------------------------------------------------------
  ForceStop  = 0;
 (void) signal(SIGINT, ForceStop_FromCtrl_C);

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  IP                         = SiTCP_MASTER_IP;
  port                       = SiTCP_PORT;
  SiTCP_ADDR.sin_family      = AF_INET;
  SiTCP_ADDR.sin_port        = htons(port);
  SiTCP_ADDR.sin_addr.s_addr = inet_addr(IP);

  struct timeval tv;
  tv.tv_sec  = 3;
  tv.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

  int flag = 1;
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

  //Connection -------------------------------------------------------------
  if(0 > connect(sock, (struct sockaddr*)&SiTCP_ADDR, sizeof(SiTCP_ADDR))){
    std::cerr << "SiTCP Master :: Connection fail" << std::endl;
    close(sock);
    return -1;
  }
  
  std::cout << "SiTCP Master :: Connection Done" << std::endl;
  std::cout << std::endl;

  // Auto Initialize -------------------------------------------------------
  PrepareFPGA();
  DebugFPGA(sock);
  std::cout << "ASIS Initialize : Done" << std::endl;
  std::cout << std::endl;
  usleep(10000);

  WriteData(3, 37888);
  PrepareSC(1);
  TransmitSC(sock);
  std::cout << "Slow Control chip1 : Done" << std::endl;
  std::cout << std::endl;
  PrepareReadSC(1);
  TransmitReadSC(sock);
  std::cout << "Read Slow Control chip1 : Done" << std::endl;
  std::cout << std::endl;

  WriteData(3, 21504);
  PrepareSC(2);
  TransmitSC(sock);
  std::cout << "Slow Control chip2 : Done" << std::endl;
  std::cout << std::endl;
  PrepareReadSC(2);
  TransmitReadSC(sock);
  std::cout << "Read Slow Control chip2: Done" << std::endl;
  std::cout << std::endl;

  WriteData(3, 5120);
  {
    unsigned int signal = 31;
    signal = signal << 16;
    unsigned int data = daq_mode;
    data = data << 8;
    signal += data;
    if(-1 == WriteData(sock, signal)){
      exit(-1);
    }
    std::cout << "#D : DAQ mode is " << daq_mode << std::endl;
  }
  
  
  // User area -----------------------------------------------------------
  bool DispMenu = true;
  int CurrentCh = 0;
  while(DispMenu){
    for(int i = 0; i<3; ++i){
      std::cout << std::endl;
    }

    std::cout << "I'm : " << SiTCP_MASTER_IP  << std::endl;
    std::cout << "Please select" << std::endl;
    std::cout << " 1. Transmit SC" << std::endl;
    std::cout << " 2. Transmit Read SC" << std::endl;
    std::cout << " 3. ASIC Initialize" << std::endl;
    std::cout << " 4. Transmit Probe " << std::endl;
    std::cout << " 5. Connection Close" << std::endl;
    std::cout << " 6. Start DAQ" << std::endl;
    std::cout << " 7. Debug" << std::endl;
    std::cout << "input # ====> " << std::flush;
    char buf[10];
    fgets(buf, sizeof(buf),stdin);
    char num;
    sscanf(buf,"%c",&num);
  
    switch(num){
    case '1':
      WriteData(3, 37888);
      PrepareSC(1);
      TransmitSC(sock);
      WriteData(3, 21504);
      PrepareSC(2);
      TransmitSC(sock);
      WriteData(3, 5120);
      break;
      
    case '2':
      WriteData(3, 37888);
      PrepareReadSC(1);
      TransmitReadSC(sock);
      WriteData(3, 21504);
      PrepareReadSC(2);
      TransmitReadSC(sock);
      WriteData(3, 5120);
      break;
      
    case '3':
      PrepareFPGA();
      DebugFPGA(sock);
      break;

    case '4':
      std::cout << "Input channel number ====> " << std::endl;
      std::cin >> CurrentCh;
      fgets(&buf[0],sizeof(buf),stdin);           //Enter buffer clear
      if(CurrentCh < 32){
	WriteData(3, 37888);
      }      
      else if(CurrentCh > 31){
	WriteData(3, 21504);
	CurrentCh = CurrentCh - 32;
      }
      std::cout<<CurrentCh<<std::endl;
      PreparePSC(CurrentCh);
      TransmitProbe(sock);
      //WriteData(3, 5120);
      break;

    case '5':
      DispMenu = false;
      break;

    case '6':
      ContinuousADC_ver2(sock);
      break;

    case '7':
      Debugging(sock);
      break;
      
    case '8':
      MHTDC_debug(sock);
      break;

    case '9':
      chipselect();
      break;

    default:
      break;
    };
  }
  
  //End Connection ---------------------------------------------------------
  close(sock);
  std::cout << "All :: Connection Close" << std::endl;

  return 0;
}

void ForceStop_FromCtrl_C(int signal){
  std::cout << "Stop request from user" << std::endl;
  ForceStop = 1;
  EndADC = 1;
}

int receive(int sock, char* data_buf, unsigned int *ReadLength){
  unsigned int revd_size     = 0;
  int tmp_returnVal = 0;

  while(revd_size < *ReadLength){
    tmp_returnVal = recv(sock, data_buf +revd_size, *ReadLength -revd_size, 0);
    if(tmp_returnVal < 0){
      int errbuf = errno;
      perror("TCP receive");
      if(errbuf == EAGAIN){
	if(EndADC == 1){
	  std::cout << "No data" << std::endl;
	  break;
	}
	continue;
      }else if(errbuf == EINTR){
	continue;
      }else{
	exit(-1);
      }
    }
    revd_size += tmp_returnVal;
  }

    //    std::cout << "ret is " << std::dec << revd_size << std::endl;
  return revd_size;
}

int WriteData(int sock, unsigned int data){
  data += 128 << 24;
//   for(int i = 31; i>-1; --i){
//     unsigned int hoge = (data >> i) & 1;
//     if(hoge){
//       fprintf(stdout, "!");
//     }else{
//       fprintf(stdout, ".");
//     }
//     if(i%8 == 0){
//       fprintf(stdout, "\n");
//     }
//   }
//   fprintf(stdout, "\n");

//  std::cout << std::hex << data << std::endl;
  send(sock, (char*)&data, sizeof(int), 0);
  unsigned int buf = 0;
  unsigned int length = 4;
  receive(sock, (char*)&buf, &length);
  if(data - buf){
    std::cerr << "### Data Transmit Error ###" << std::endl;
    std::cerr << "Transmited data is " << std::hex << data << std::endl;
    std::cout << "Returned data is   " << std::hex << buf << std::endl;
    return -1;
  }

  return 0;
}

int ReadData(int sock, unsigned int signal, unsigned int *data){
  signal += 64 << 24;
  send(sock, (char*)&signal, sizeof(int), 0);
  unsigned int length = 4;
  receive(sock, (char*)data, &length);

//    for(int i = 31; i>-1; --i){
//      unsigned int hoge = (*data >> i) & 1;
//      if(hoge){
//        fprintf(stdout, "!");
//      }else{
//        fprintf(stdout, ".");
//      }
//      if(i%8 == 0){
//        fprintf(stdout, "\n");
//      }
//    }
//    fprintf(stdout, "\n");
  
  return 0;
}

int DebugFPGA(int socket){
  unsigned int buffer = 0;
  buffer += 0 << 16;
  buffer += (Pindata[0] & 255) << 8;
  if(-1 == WriteData(socket, buffer)){
    exit(-1);
  }

  for(int i = 1 ; i<5; ++i){
    buffer = 0;
    if(i == 4){
      buffer += 5 << 16;      
    }else{
      buffer += i << 16;
    }
    buffer += ((Pindata[1] >> (i-1)*8) & 255) << 8;

    if(-1 == WriteData(socket, buffer)){
      exit(-1);
    }
    usleep(1);
  }
  
  return 0;
}

int TransmitSC(int socket){
  unsigned int data = 0;
  //Set SC mode -----------------------------------------------------------
  data = 0;
  data += 1 << 16;
  data += 240 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  //SC start -------------------------------------------------------------
  data = 0;
  data += 10 << 16;
  data += (slowdata[0] & 255) << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  for(int i = 1; i<15; ++i){
    for(int shift = 0; shift<4; ++shift){
      data = 0;
      data += 10 << 16;
      data += ((slowdata[i] >> 8*shift) & 255) << 8;
      if(-1 == WriteData(socket, data)){
	exit(-1);
      }
      //std::cout<<"test"<<std::endl;
      usleep(1);
    }
  }

  // usleep(50000);

  //StartCycle -----------------------------------------------------------
  data = 0;
  data += 1 << 16;
  data += 242 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  data = 0;
  data += 1 << 16;
  data += 240 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  // usleep(50000);

  //Load SC --------------------------------------------------------------
  data = 0;
  data += 1 << 16;
  data += 241 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  data = 0;
  data += 1 << 16;
  data += 240 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  return 0;
}

int TransmitReadSC(int socket){
  //SCA read ---------------------------------------------------------------
  unsigned int data = 0;

  for(int i = 0; i<4; ++i){
    data = 0;
    data += 12 << 16;
    data += ((ReadSCdata[0] >> i*8) & 255) << 8;
    if(-1 == WriteData(socket, data)){
      exit(-1);
    }
    usleep(1);
  }

  //StartCycle ------------------------------------------------------------
  data = 0;
  data += 1 << 16;
  data += 242 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  data = 0;
  data += 1 << 16;
  data += 240 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }
  
  return 0;
}

int TransmitProbe(int socket){
  unsigned int data = 0;
  //Set Probe mode --------------------------------------------------------
  data += 1 << 16;
  data += 208 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  //Probe start ----------------------------------------------------------
  for(int i = 0; i<sizeByte_PSC; ++i){
    for(int shift = 0; shift<4; ++shift){
      data = 0;
      data += 10 << 16;
      data += ((Probedata[i] >> 8*shift) & 255) << 8;
      if(-1 == WriteData(socket, data)){
	exit(-1);
      }
      usleep(1);
    }
  }

  usleep(50000);

  //StartCycle ------------------------------------------------------------
  data = 0;
  data += 1 << 16;
  data += 210 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  data = 0;
  data += 1 << 16;
  data += 208 << 8;
  if(-1 == WriteData(socket, data)){
    exit(-1);
  }

  return 0;
}

int ContinuousADC(int socket){
  std::string file_num;
  std::cout << "Input output data file name : " << std::flush;
  std::cin >> file_num;
  std::string basename = "after_break_ext_";
  std::string file_name = basename + file_num + ".dat";
  std::ofstream file(file_name.c_str(),std::ios::binary);

  int MaxCycleNum = 0;
  std::cout << "Input total # of events" << std::endl;
  //  std::cout << "If you type 'N', preset # of events will be N*1000" << std::endl;
  std::cout << "======>" << std::flush;
  std::cin >> MaxCycleNum;
  MaxCycleNum = MaxCycleNum/1000;
  std::cout << "Preset value : " << MaxCycleNum*1000 << std::endl;

  for(int i = 0; i<MaxCycleNum; ++i){
    //    ADCOneCycle(socket, file);
    ADCOneCycle_wHeader(socket, file);
    std::cout << "Event # " << 1000*(i+1) << std::endl;
  }

  std::cout << "End ADC" << std::endl;
  file.close();

  return 0;
}

int ContinuousADC_ver2(int socket){
  std::string file_name;
  std::cout << "Input output data file name : " << std::flush;
  std::cin >> file_name;

  double step_width;
  std::cout<<"Input step width[mm]:"<<std::flush;
  std::cin>>step_width;
  //step_width[mm]
  
  double move_width;
  std::cout<<"Input move width[mm]:"<<std::flush;
  std::cin>>move_width;


  std::string directory_name;
  std::cout <<"Input directory name:"<<std::flush;
  std::cin >> directory_name;

  int MaxCycleNum = 0;
  std::cout << "Input total # of events" << std::endl;
  std::cout << "======>" << std::flush;
  std::cin >> MaxCycleNum;
  std::cout << "Preset value : " << std::dec << MaxCycleNum << std::endl;

  time_t t1,t2;

  t1 = time(NULL);
 
 int move_time;
 move_time = (int) move_width/step_width;
 system( "python /home/smcha2019/SummerChallengeKO/code_for_stage/stage.py -o -  ");

 sleep(10);
 
 int start_point = 30909 - 250 * move_width;
 std::stringstream move_width_order;
 std::string Move_Width_Order;
 move_width_order<<"python /home/smcha2019/SummerChallengeKO/code_for_stage/stage.py -mr "<<start_point;
 Move_Width_Order = move_width_order.str();

 system(Move_Width_Order.c_str() ); 

 usleep(5000000);

 std::stringstream directory_order;
 directory_order<<"mkdir "<<directory_name;
 std::string Directory_order =directory_order.str();
 system(Directory_order.c_str());

for(int move_count=0;move_count<move_time;move_count++){
  std::stringstream FileName;
  FileName<<"/home/smcha2019/EASIROC/easiroc_autoOP/easiroc/"<<directory_name<<"/"<<file_name<<move_count<<".dat";
  std::string result = FileName.str();
  std::ofstream file(result.c_str(),std::ios::binary);
  unsigned int signal = 0;
  int EventNum = 0;
  

  signal += 32 << 24;
  signal += 100 << 16;
  send(socket, (char*)&signal, sizeof(int), 0);

  while(EventNum < MaxCycleNum){

    ADCOneCycle_wHeader_ver2(socket, file);
    ++EventNum;
    if(0 == EventNum%1000){
      std::cout << "Event # " << std::dec << EventNum << std::endl;

      t2 = time(NULL);

      int time = t2-t1;

      //std::cout << std::endl;
      std::cout << "time" << " = " << time << "s"<< std::endl;
      std::cout << std::endl;

    }
    
    if(false 
       || EventNum  == MaxCycleNum
       || ForceStop == 1
       ){
      ADCStop(socket);
      
      EndADC = 1;
      int abort = 0;
      while(0 == ADCOneCycle_wHeader_ver2(socket, file)){
	usleep(10000);
	if(abort == 50){
	  ADCStop(socket);
	  abort = 0;
	}
	std::cout << "dummy data" << std::endl;
	++abort;
      }

      EndADC    = 0;
      ForceStop = 0;

      std::stringstream request;
      int step_width_count = (int) (step_width*500);
      request<<"python /home/smcha2019/SummerChallengeKO/code_for_stage/stage.py -mr "<<step_width_count;
      std::string Request = request.str();
      system(Request.c_str());
      usleep((int)step_width*50000);
      if(move_count==9){
	  
	      break;
	
	std::cout<<"hoge1";
      }
     
    }
  }
  file.close();
}
	//system("python /home/smcha2019/SummerChallengeKO/code_for_stage/stage.py -ml 35000");

  std::cout << "End ADC" << std::endl;

  t2 = time(NULL);

  int time = t2-t1;

  std::cout << std::endl;
  std::cout << "time" << " = " << time << "s"<< std::endl;

  return 0;
}

void ADCOneCycle(int socket, std::ofstream& file){
  static const int TotalEvent = 1000;
  unsigned int signal = 0;

  signal += 16 << 24;
  signal += 100 << 16;
  send(socket, (char*)&signal, sizeof(int), 0);
  
  unsigned int ADCData[16*TotalEvent];
  for(int i = 0; i<16*TotalEvent; ++i){
    ADCData[i] = 0;
  }
  static unsigned int ADCLength  = sizeof(ADCData);
  receive(socket, (char*)ADCData, &ADCLength);


  for(int Event = 0; Event<TotalEvent; ++Event ){
    int event = 0xffff;
    file.write((char*)&event, sizeof(short));
    //    fprintf(stdout, "Event # %d\n",Event);

    for(int i = 0; i<16; ++i){
      unsigned int buffer = ADCData[i+16*Event] & 4095;
      //      fprintf(stdout, "%d\n",buffer & 15);
      file.write((char*)&buffer, sizeof(short));
      buffer = (ADCData[i+16*Event] >> 16) & 4095;
      //      fprintf(stdout, "%d\n",buffer & 15);
      file.write((char*)&buffer, sizeof(short));
    }
    //    std::cout << std::endl;
    //    std::cout << std::endl;
  }

  signal = 0;
  signal += 100 << 16;
  if(-1 == WriteData(socket, signal)){
    exit(-1);
  }

}

void ADCOneCycle_wHeader(int socket, std::ofstream& file){
  static const int TotalEvent = 1000;
  unsigned int signal = 0;

  unsigned int DataBuffer[100*TotalEvent];
  memset(DataBuffer, 0, sizeof(DataBuffer));
  unsigned int *ptr_DataBuf = DataBuffer;
  unsigned int TotalRecvByte  = 0;
  unsigned int TotalRecvEvent = 0;

  static unsigned int sizeHeader = 2*sizeof(int);

  signal += 16 << 24;
  signal += 100 << 16;
  send(socket, (char*)&signal, sizeof(int), 0);

  for(int i = 0; i<TotalEvent; ++i){
    unsigned int Header[2] = {1, 1};
    receive(socket, (char*)Header, &sizeHeader);
    //    Header[0] = 0xFFFFEA0C;
    //    Header[1] = 0x00000100;
    if(Header[0] != 0xFFFFEA0C){
      std::cerr << "Fatal ADC ERROR : HEADER" << std::endl;
      std::cerr << "Header1 : " << std::hex << Header[0] << " Header2 : " << std::hex << Header[1] << std::endl;
      exit(-1);
    }else{
      //      std::cerr << "Header1 : " << std::hex << Header[0] << " Header2 : " << std::hex << Header[1] << std::endl;
      //      std::cout << "Num " << i << std::endl;
      //      std::cerr << "Header1 : " << std::hex << Header[0] << " Header2 : " << std::hex << Header[1] << std::endl;
      unsigned int sizeData = Header[1] & 0xffff;
      unsigned int OneData[sizeData/sizeof(int)];
//       for(unsigned int i = 0; i<sizeData/sizeof(int); ++i){
// 	OneData[i] = (900 +2*i) << 16;
// 	OneData[i] += 900 +2*i +1;
//       }
      receive(socket, (char*)OneData, &sizeData);
      //      std::cout << OneData[0] << std::endl;
      
      memcpy(ptr_DataBuf, Header, sizeHeader);
      ptr_DataBuf += 2;

      memcpy(ptr_DataBuf, OneData, sizeData);
      ptr_DataBuf += sizeData/sizeof(int);

      TotalRecvByte += sizeHeader + sizeData;
      ++TotalRecvEvent;
    }
  }


  for(unsigned int i = 0; i<TotalRecvByte/sizeof(int); ++i){
//    unsigned  int buffer = (DataBuffer[i] >> 16) & 0xffff;
//     //      fprintf(stdout, "%d\n",buffer & 15);
//     file.write((char*)&buffer, sizeof(short));
//     buffer = DataBuffer[i] & 0xffff;
//     //      fprintf(stdout, "%d\n",buffer & 15);
//     file.write((char*)&buffer, sizeof(short));
    unsigned  int buffer = DataBuffer[i];
    file.write((char*)&buffer, sizeof(int));
  }
  
  signal = 0;
  signal += 100 << 16;
  if(-1 == WriteData(socket, signal)){
    exit(-1);
  }

  return;
}

int ADCOneCycle_wHeader_ver2(int socket, std::ofstream& file){
  static const int NofHeader = 3;

  unsigned int DataBuffer[1000];
  memset(DataBuffer, 0, sizeof(DataBuffer));
  unsigned int *ptr_DataBuf = DataBuffer;
  unsigned int TotalRecvByte  = 0;
  unsigned int TotalRecvEvent = 0;

  static unsigned int sizeHeader = NofHeader*sizeof(int);
  
  unsigned int Header[NofHeader] = {1, 1, 1};
  int ret = 0;
  ret = receive(socket, (char*)Header, &sizeHeader);
  if(ret <= 0 && EndADC == 1){
    return -1;
  }

  //  std::cout << "Header1 : " << std::hex << Header[0] << " Header2 : " << std::hex << Header[1] << std::endl;
  //    Header[0] = 0xFFFFEA0C;
  //    Header[1] = 0x00000100;
  if(Header[0] != 0xFFFFEA0C){
    std::cerr << "Fatal ADC ERROR : HEADER" << std::endl;
    std::cerr << "Header1 : " << std::hex << Header[0] 
	      << " Header2 : " << std::hex << Header[1]
	      << " Header3 : " << std::hex << Header[2] << std::endl;
    exit(-1);
  }else{
    unsigned int sizeData = (sizeof(int)*Header[1] & 0xffff);
    unsigned int NofWord = (Header[1] & 0xffff);
    unsigned int OneData[NofWord];
    receive(socket, (char*)OneData, &sizeData);

//     for(int i = 0; i<NofWord; ++i){
//       std::cout << std::hex << OneData[i] << std::endl;
//     }

    memcpy(ptr_DataBuf, Header, sizeHeader);
    ptr_DataBuf += NofHeader;

    memcpy(ptr_DataBuf, OneData, sizeData);
    ptr_DataBuf += sizeData/sizeof(int);

    TotalRecvByte += sizeHeader + sizeData;
    ++TotalRecvEvent;
  }

  if(EndADC != 1){ for(unsigned int i = 0; i<TotalRecvByte/sizeof(int); ++i){
      unsigned int buffer = DataBuffer[i];
      file.write((char*)&buffer, sizeof(int));
    }
  }

  return 0;
}

// Debug ---------------------------------------------------------------------
int Debugging(int socket){
  enum debug{read, write, quit, sizedebug};
  bool flag_[sizedebug];
  for(int i = 0; i<sizedebug; ++i){
    flag_[i] = false;
  }

  std::cout << "Read/Write" << std::endl;
  std::cout << "1. Read" << std::endl;
  std::cout << "2. Write" << std::endl;
  std::cout << "3. Return" << std::endl;
  std::cout << "input # =====> " << std::flush;
  char buf[10];
  fgets(buf, sizeof(buf), stdin);
  char num;
  sscanf(buf, "%c", &num);
  
  switch(num){
  case '1':
    flag_[read] = true;
    break;
    
  case '2':
    flag_[write] = true;
    break;
    
  default:
    
    break;
  };

  if(flag_[read]){
    std::cout << "Debug Read" << std::endl;
    std::cout << "Input subaddress : " << std::flush;
    std::string strbuf;
    std::cin >> strbuf;
    std::istringstream valbuf(strbuf);
    unsigned int saddr = 0;
    valbuf >> saddr;
    saddr = saddr << 16;
    
    unsigned int data = 0;
    if(0 == ReadData(socket, saddr, &data)){
      std::cout << std::hex << data << std::endl;
      std::cout << "bit31 <<                    >> bit0" << std::endl;
      for(int i = 1; i<33; ++i){
	if(data >> (32 -i) & 1){
	  std::cout << "!" << std::flush;
	}else{
	  std::cout << "." << std::flush;
	}
	
	if(0 == i%8){
	  std::cout <<  " " << std::endl;
	}
      }
      std::cout << std::endl;
    }else{
      std::cerr << "Read error" << std::endl;
      exit(-1);
    }
  }

  if(flag_[write]){
    std::cout << "Debug Write" << std::endl;
    std::cout << "Input subaddress : " << std::flush;
    std::string strbuf;
    std::cin >> strbuf;
    std::istringstream valbuf(strbuf);
    unsigned int signal = 0;
    valbuf >> signal;
    signal = signal << 16;
    
    std::cout << "Input signal : " << std::flush;
    std::string strbuf2;
    std::cin >> strbuf2;
    std::istringstream databuf(strbuf2);
    unsigned int data = 0;
    databuf >> data;
    data = data << 8;
    signal += data;
    
    std::cout << std::hex << signal << std::flush;
    std::cout << std::dec << std::endl;
    std::cout << signal << std::endl;
    if(-1 == WriteData(socket, signal)){
      exit(-1);
    }
  }

  return 0;
}

void ADCStop(int socket){
  unsigned int signal = 0;
  std::cout << "ADC exit process" << std::endl;
  signal += 16 << 24;
  signal += 100 << 16;
  send(socket, (char*)&signal, sizeof(int), 0);
  sleep(1);
  
  signal = 0;
  signal += 100 << 16;
  send(socket, (char*)&signal, sizeof(int), 0);

  usleep(10000);

  return;
}

void MHTDC_debug(int sock){

  std::ofstream m_file("mhtdc.dat",std::ios::binary);

  unsigned int saddr = 50;
  unsigned int data = 0;
  saddr = saddr << 16;
  unsigned int signal = 4;
  signal = signal << 8;
  saddr += signal;
  WriteData(sock, saddr);

  saddr = 50;
  saddr = saddr << 16;
  WriteData(sock, saddr);
  usleep(100000);

  while(ForceStop == 0){
    //  for(int k = 0; k<3; ++k){
    saddr = 41;
    data = 0;
    saddr = saddr << 16;

    usleep(100000);
    if(0 == ReadData(sock, saddr, &data)){
      int size = (data >> 8) & 0xff;
      //      std::cout << "size : " << size << std::endl;
      std::cout << std::hex << "status : " << data << std::endl;
      
      int valid = data & 0x1;

      if(valid == 1){
	unsigned int f_size = 0xffff;
	f_size = (f_size << 16) + size;
	m_file.write((char*)&f_size, sizeof(unsigned int));
	for(int i = 0; i<size; ++i){
	  //	  usleep(100);
	  saddr = 42;
	  saddr = saddr << 16;
	  unsigned int head =0;
	  if(0 == ReadData(sock, saddr, &data)){
	    std::cout << std::hex << "header" << i << " : " << data << std::endl;
	    head = data & 0xffff;
	  }else{
	    std::cerr << "Read error" << std::endl;
	    exit(-1);
	  }

	  // Leading 
	  saddr = 40;
	  saddr = saddr << 16;
	  if(0 == ReadData(sock, saddr, &data)){
	    unsigned int time = (data & 0xffff);
	    unsigned int one_data = (head << 16) + time;
	    m_file.write((char*)&one_data, sizeof(unsigned int));
	    std::cout << std::dec << "time L" << i << " : " << time << " ns" << std::endl;
	  }else{
	    std::cerr << "Read error" << std::endl;
	    exit(-1);
	  }

	  // Trailing
	  saddr = 43;
	  saddr = saddr << 16;
	  if(0 == ReadData(sock, saddr, &data)){
	    unsigned int time = (data & 0xffff);
	    time = (1 << 15) + time;
	    unsigned int one_data = (head << 16) + time;
	    m_file.write((char*)&one_data, sizeof(unsigned int));
	    std::cout << std::dec << "time T" << i << " : " << (time & 0xfff) << " ns" << std::endl;
	  }else{
	    std::cerr << "Read error" << std::endl;
	    exit(-1);
	  }
	}
	saddr = 50;
	saddr = saddr << 16;
	signal = 4;
	signal = signal << 8;
	saddr += signal;
	WriteData(sock, saddr);

	saddr = 50;
	saddr = saddr << 16;
	WriteData(sock, saddr);

      }
    }else{
      std::cerr << "Read error" << std::endl;
      exit(-1);
    }
  }

  return;
}

int chipselect(){

  WriteData(3, 37888);
    // if(-1 == WriteData(3, 5120)){
    //   exit(-1);
    // }
  return 0;
}
