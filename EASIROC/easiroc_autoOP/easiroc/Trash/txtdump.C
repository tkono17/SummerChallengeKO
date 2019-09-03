/* ---------64channels read out system____y_d2r.cc(dat to root)---------- */
//このファイルで、root変換すると、ターミナル場で正しいchに反映される。

void txtdump(char *filename){
  gROOT->SetStyle("Plain");
  
  std::ifstream hoge(filename,std::ios::binary);
  if(!hoge.is_open()){
    cout << "no file" << endl;
    return;
  }

  while(!hoge.eof()){
    UInt_t val;
    hoge.read((char*)&val, sizeof(int));
    printf("%08x \n", val);
  }

  return;
}
