void ehist64(char* filename = "hoge.dat"){
  gROOT->SetStyle("Plain");
  
  std::ifstream hoge(filename,std::ios::binary);
  if(!hoge.is_open()){
    cout << "no file" << endl;
    return;
  }
  TH1F *hist[64];
  TH2F* chhist = new TH2F("chhist","",64,0-0.5,64-0.5,1500,500,2000);
  for(int i = 0; i<64; ++i){
    std::ostringstream _name;
    _name << "ch" << i;
    std::string hist_name = _name.str();
    hist[i] = new TH1F(hist_name.c_str(), hist_name.c_str(), 4096, 0, 4096);
  }

  while(!hoge.eof()){
    int val;
    hoge.read((char*)&val, sizeof(int));
    //    std::cout << std::hex << val << std::endl;
    
    if(val == 0xffffea0c){
      hoge.read((char*)&val, sizeof(int));
      for(int i = 0; i<65; ++i){
 	hoge.read((char*)&val, sizeof(int));
	if(i>0){
	int buffer = val & 0xffff;
 	hist[i-1]->Fill(buffer);
	chhist->Fill(i-1,buffer);
	}
      }
    }
  }
 
  return;
}
