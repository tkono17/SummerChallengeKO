#include <stdio.h>
#include "TH1F.h"
#include "TH1.h"
#include "TStyle.h"
#include "TLatex.h"
void myText2(Double_t x, Double_t y, Color_t color, char *text, Double_t size){

 //Double_t tsize=0.05;                                                                                                
 TLatex l; //l.SetTextAlign(12);                                                                                       
 l.SetTextSize(size);
 l.SetNDC();
 l.SetTextColor(color);
 l.DrawLatex(x,y,text);
}

Int_t fit(void)
{
  // TCanvas *c1 = new TCanvas("c1","c1",600,400);
  // gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1);

  Double_t par[6];
  Double_t a;
  Double_t b;
  Double_t n = 1.6;

  printf("input 2 fit peak\n");
  scanf("%lf %lf",&a,&b);

  par[0]=1000; par[1]=a; par[2]=5;
  par[3]=1000; par[4]=b; par[5]=5;

  printf("\npar1 %lf  par4 %lf  \n par2 %lf par5 %lf \n",par[1], par[4],par[2],par[5]);

  TF1 *f1 =new TF1("f1","[0]*exp(-0.5*((x-[1])/[2])^2)",par[1]-2*par[2],par[1]+2*par[2]);
  TF1 *f2 =new TF1("f2","[0]*exp(-0.5*((x-[1])/[2])^2)",par[4]-2*par[5],par[4]+2*par[5]);
 
  f1->SetParNames("peak event","mean1","#sigma1");
  f2->SetParNames("peak event","mean2","#sigma2");

  f1->SetParameters(&par[0]);
  f2->SetParameters(&par[3]);
  
  h1->Fit("f1","R");
  h1->Fit("f2","R+");
 
  f1->GetParameters(&par[0]);
  f2->GetParameters(&par[3]);

  printf("\npar1 %lf  par4 %lf  \n par2 %lf par5 %lf \n",par[1], par[4],par[2],par[5]); 

  f1->SetRange(par[1]-n*fabs(par[2]),par[1]+n*fabs(par[2]));
  f2->SetRange(par[4]-n*fabs(par[5]),par[4]+n*fabs(par[5]));

  h1->Fit("f1","R");
  h1->Fit("f2","R+");

  f1->GetParameters(&par[0]);
  f2->GetParameters(&par[3]);

  printf("\npar1 %lf  par4 %lf  \n par2 %lf par5 %lf \n",par[1], par[4],par[2],par[5]); 

  f1->SetRange(par[1]-n*fabs(par[2]),par[1]+n*fabs(par[2]));
  f2->SetRange(par[4]-n*fabs(par[5]),par[4]+n*fabs(par[5]));

  h1->Fit("f1","R");
  h1->Fit("f2","R+");

  f1->GetParameters(&par[0]);
  f2->GetParameters(&par[3]);

  //f1->GetChisquare()

  myText2(0.55,0.6,kBlack,Form("Mean1 = %5.2f \\ Mean2 = %5.2f",par[1], par[4]),0.04);

  myText2(0.55,0.55,kBlack,Form("sigma1 = %5.2f sigma2 = %5.2f" ,par[2], par[5]),0.04);

  myText2(0.7,0.6,kBlack,Form("Mean2 - Mean1 = %5.2f",par[4]-par[1]),0.04);

  return 0;

}









