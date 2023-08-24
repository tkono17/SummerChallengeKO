void graph(){
  char filename[100];
  const int n = 59;
  TGraph *gr = new TGraph(n);
  std::cout<<"t"<<std::endl;
  for(int i=0; i<n; ++i){
    sprintf(filename, "ksc_029/ksc_029_%04d.root",i);
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
    gr->SetPoint(i , i, mean);
    f->Close();
    delete f;
  }
  gr->SetMarkerStyle(20);
  gr->Draw("ap");
}
