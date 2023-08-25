void graph(const int n, const int fn){
  TCanvas *c1 = new TCanvas("canvas","c1");
  char filename[100];
  char grtitle[100];
  char printname[100];
  sprintf(grtitle,"data %03d" ,fn);
  sprintf(printname,"~/sc/SummerChallengeKO/pretest/pretest2023/plot/ksc%03d.png", fn);
  TGraph *gr = new TGraph(n);
  gr->SetTitle(grtitle);
  std::cout<<"t"<<std::endl;
  std::cout<<gr->GetN()<<std::endl;
  for(int i=0; i<n; ++i){
    sprintf(filename, "ksc_%03d/ksc_%03d_%04d.root",fn,fn,i);
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
    gr->SetPoint(i, i, mean);
    std::cout<<i<<" , "<<mean<<std::endl;
    f->Close();
    delete f;
  }
  gr->SetMarkerStyle(20);
  gr->Draw("ap");
  c1->Print(printname);
}
