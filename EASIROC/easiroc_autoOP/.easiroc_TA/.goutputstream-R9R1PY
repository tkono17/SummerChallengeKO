#include <stdio.h>
#include "TH1F.h"
#include "TH1.h"
#include "TStyle.h"
#include "TLatex.h"
#include<fstream>
void myText2(Double_t x, Double_t y, Color_t color, char *text, Double_t size){

 //Double_t tsize=0.05;                                                                                                
 TLatex l; //l.SetTextAlign(12);                                                                                       
 l.SetTextSize(size);
 l.SetNDC();
 l.SetTextColor(color);
 l.DrawLatex(x,y,text);
}

Int_t hist(void)
{
  gROOT->Reset();
  gStyle->SetOptFit();
  x = new vector<Double_t>();
   
  Int_t cnt(0);
  Int_t bin;                     //number of bin
  
  TH1F *h1 = new TH1F("h1", "TH50", 1000, 0, 1000);  //data hisgram
 
 

  ifstream ifs("hist.dat");
  
  while(!ifs.eof()){
    if(ifs.fail()){
      break;
    }
    cnt++;
    Double_t xx;
    ifs >> xx;
    std::cout<<xx<<std::endl;
    x->push_back(xx);
    
  }
  
  ifs.close();
  
  for(UInt_t i=0;i<x->size();i++){
    h1->Fill(x->at(i));    //x bin に y倍にしてつめる
    
  }
  
  h1->Draw();
}
