
{
  gROOT->Reset();
  
  double x1;
  /*double x2;
  double x3;
  double x4;
  double x5;
  double x6;
  double x7;
  double x8;
  double x9;
  double x10;
  double x11;
  double x12;
  double x13;
  double x14;
  double x15;*/
  
  TCanvas *c1 = new TCanvas("name","title",10,20,500,400);
  
 
  TH1F *h1 = new TH1F("h1","h1",10000,0,10000);
  /*TH1F *h2 = new TH1F("h2","h2",10000,0,10000);
  TH1F *h3 = new TH1F("h3","h3",10000,0,10000);
  TH1F *h4 = new TH1F("h4","h4",10000,0,10000);
  TH1F *h5 = new TH1F("h5","h5",10000,0,10000);
  TH1F *h6 = new TH1F("h6","h6",10000,0,10000);
  TH1F *h7 = new TH1F("h7","h7",10000,0,10000);
  TH1F *h8 = new TH1F("h8","h8",10000,0,10000);
  TH1F *h9 = new TH1F("h9","h9",10000,0,10000);
  TH1F *h10 = new TH1F("h10","h10",10000,0,10000);
  TH1F *h11 = new TH1F("h11","h11",10000,0,10000);
  TH1F *h12 = new TH1F("h12","h12",10000,0,10000);
  TH1F *h13 = new TH1F("h13","h13",10000,0,10000);
  TH1F *h14 = new TH1F("h14","h14",10000,0,10000);
  TH1F *h15 = new TH1F("h15","h15",10000,0,10000);*/
  
  ifstream ifs1("root2016/run0023_120611221.root"); 
  /*ifstream ifs1("data2016/run0023_12061221.dat");
  ifstream ifs2("data2016/run0024_12061235.dat");
  ifstream ifs3("data2016/run0025_12061247.dat");
  ifstream ifs4("data2016/run0026_12061254.dat");
  ifstream ifs5("data2016/run0027_12061301.dat");
  ifstream ifs6("data2016/run0028_12061309.dat");
  ifstream ifs7("data2016/run0029_12061315.dat");
  ifstream ifs8("data2016/run0030_12061319.dat");
  ifstream ifs9("data2016/run0031_12061322.dat");
  ifstream ifs10("data2016/run0032_12061325.dat");
  ifstream ifs11("data2016/run0033_12061330.dat");
  ifstream ifs12("data2016/run0034_12061340.dat");
  ifstream ifs13("data2016/run0035_12061344.dat");
  ifstream ifs14("data2016/run0036_12061348.dat");
  ifstream ifs15("data2016/run0037_12061350.dat");*/
  
  int i1=0;
  /*int i2=0;
  int i3=0;
  int i4=0;
  int i5=0;
  int i6=0;
  int i7=0;
  int i8=0;
  int i9=0;
  int i10=0;
  int i11=0;
  int i12=0;
  int i13=0;
  int i14=0;
  int i15=0;*/
  
  while(ifs1>>x1){
    h1->Fill(i1,x1);
    i1++;
  }
  
  /*while(ifs2>>x2){
    h2->Fill(i2,x2);
    i2++;
  }
  
  while(ifs3>>x3){
    h3->Fill(i3,x3);
    i3++;
  }
  
  while(ifs4>>x4){
    h4->Fill(i4,x4);
    i4++;
  }
  
  while(ifs5>>x5){
    h5->Fill(i5,x5);
    i5++;
  }
  
  while(ifs6>>x6){
    h6->Fill(i6,x6);
    i6++;
  }
  
  while(ifs7>>x7){
    h7->Fill(i7,x7);
    i7++;
  }
  
  while(ifs8>>x8){
    h8->Fill(i8,x8);
    i8++;
  }
  
  while(ifs9>>x9){
    h9->Fill(i9,x9);
    i9++;
  }
  
  while(ifs10>>x10){
    h10->Fill(i10,x10);
    i10++;
  }
  
  while(ifs11>>x11){
    h11->Fill(i11,x11);
    i11++;
  }
  
  while(ifs12>>x12){
    h12->Fill(i12,x12);
    i12++;
  }
  
  while(ifs13>>x13){
    h13->Fill(i13,x13);
    i13++;
  }
  
  while(ifs14>>x14){
    h14->Fill(i14,x14);
    i14++;
  }
  
  while(ifs15>>x15){
    h15->Fill(i15,x15);
    i15++;
    }*/
  
  
  h1->SetLineColor(kRed);
  /*h2->SetLineColor(kBlue);
  h3->SetLineColor(kGreen);
  h4->SetLineColor(kYellow);
  h5->SetLineColor(kMagenta);
  h6->SetLineColor(kOrange);
  h7->SetLineColor(kPink);
  h8->SetLineColor(kSpring);
  h9->SetLineColor(kViolet);
  h10->SetLineColor(kAzure);
  h11->SetLineColor(kTeal);
  h12->SetLineColor(kCyan);
  h13->SetLineColor(kBlack);
  h14->SetLineColor(kGray);
  h15->SetLineColor(kWhite);*/
  
  h1->Draw("C");  
  /*h2->Draw("same");
  h3->Draw("same");
  h4->Draw("same");
  h5->Draw("same");
  h6->Draw("same");
  h7->Draw("same");
  h8->Draw("same");
  h9->Draw("same");
  h10->Draw("same");
  h11->Draw("same");
  h12->Draw("same");
  h13->Draw("same");
  h14->Draw("same");
  h15->Draw("same");*/



}
