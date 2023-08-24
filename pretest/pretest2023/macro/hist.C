void hist(){
  char filename[100];
  TH1F *kansho = new TH1F("kansho","",120,0.,120.);
  std::cout<<"t"<<std::endl;
  for(int i=0; i<120; ++i){
    sprintf(filename, "ksc_030/ksc_030_%04d.root",i);
    auto f = TFile::Open(filename);
    auto hist = dynamic_cast<TH1*>(f->Get("ADC_HIGH_0"));
    if(hist==nullptr){
      std::cout<<"nullhist "<<i<<std::endl;
      f->Close();
      delete f;
      continue;
    }
    double mean = hist->GetMean();
    //hist->Fit("gaus","","",810,870);
    //auto func = hist->GetFunction("gaus");
    //double p0 = func->Integral(810,870)/10000.;
    //std::cout<<p0<<std::endl;
    //double value = -std::log(p0);
    kansho->SetBinContent(i+1, mean);
    f->Close();
    delete f;
  }
  kansho->Draw();
}
