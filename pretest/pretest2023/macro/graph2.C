void graph2(const int n,const int fn){
  char filename[100];
  char grtitle[100];
  sprintf(grtitle,"data %03d" ,fn);
  TGraph *gr1 = new TGraph(n);
  gr1->SetTitle(grtitle);
  TGraph *gr2 = new TGraph(n);
  std::cout<<"t"<<std::endl;
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
    if(i%2==0){
      gr1->SetPoint(i , i, mean);
    }else{
      gr2->SetPoint(i , i, mean);
    }
    f->Close();
    delete f;
  }
  gr1->SetMarkerStyle(20);
  gr1->SetMarkerColor(2);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerColor(4);
  gr1->Draw("ap");
  gr2->Draw("p");
}
