void graphOcha(const int n, const int fn){
  TCanvas *c1 = new TCanvas("canvas","c1");
  char filename[100];
  char grtitle[100];
  char printname[100];
  sprintf(grtitle,"pretest %03d", fn);
  sprintf(printname,"../plot/pretest%03d.png", fn);
  TGraph *gr = new TGraph(n);
  gr->SetTitle(grtitle);
  std::cout<<"t"<<std::endl;
    std::cout<<gr->GetN()<<std::endl;
  for(int i=0; i<n; ++i){
    sprintf(filename, "test_%03d_%03d.root",fn,i);
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
    std::cout<<i<<" , "<<mean<<std::endl;
    f->Close();
    delete f;
  }
  auto x = gr->GetX();
  auto y = gr->GetY();
  for(int i = 0; i<n; ++i){
    std::cout<<i<<" "<<x[i]<<" "<<y[i]<<std::endl;
    //gr->Dump();
  }
  gr->SetMarkerStyle(20);
  gr->Draw("al");
  c1->Print(printname);
}
