void chhist(char* filename = "hoge.dat"){
  gROOT->SetStyle("Plain");
  
  std::ifstream hoge(filename,std::ios::binary);
  if(!hoge.is_open()){
    std::cout << "no file" << std::endl;
    return;
  }

  TH2F* h2 = new TH2F("h2","",64,0-0.5,64-0.5,1500,500,2000);

  while(!hoge.eof()){
    int val;
    hoge.read((char*)&val, sizeof(int));
    //    std::cout << std::hex << val << std::endl;
    
    if(val == 0xffffea0c){
      hoge.read((char*)&val, sizeof(int));
      for(int i = 0; i<65; ++i){
 	hoge.read((char*)&val, sizeof(int));
	int buffer = val & 0xffff;
 	h2->Fill(i-1,buffer);
      }
    }
  }
  
  h2->Draw("colz");


  return;
}
