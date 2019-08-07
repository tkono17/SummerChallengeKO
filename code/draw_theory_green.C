#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "math.h"

void draw_theory_green(float a, float b, float L) {
  const float lam = 0.560e-3;
  std::vector<float> theta;
  std::vector<float> range;
  std::vector<float> Inte_1;
  std::vector<float> Inte_2;

  for (int theta_i=0; theta_i<100000; theta_i++) {
    float th = (theta_i*1.0-50000)/800000;

    float k = 2*M_PI/lam;
    float s = sin(th);
    float alpha = k*a*s/2;
    float beta = k*b*s/2;    
    float z = L*s;

    float I1 = 4*(sin(beta)*sin(beta)/(beta*beta))*cos(alpha)*cos(alpha);
    float I2 = 4*(sin(beta)*sin(beta)/(beta*beta));

    theta.push_back(th); 
    range.push_back(z);
    Inte_1.push_back(I1);
    Inte_2.push_back(I2);
  }
  
  float m_a = lam*L/(2*a);
  float m_b = lam*L/b;
  
  std::cout << "Interval by alpha : " << m_a << std::endl;
  std::cout << "Interval by beta  : " << m_b << std::endl;

  TCanvas *c1 = new TCanvas("name","title",800, 800);
  TGraph* g1 = new TGraph(Inte_1.size(), &range[0], &Inte_1[0]);
  TGraph* g2 = new TGraph(Inte_1.size(), &range[0], &Inte_2[0]);
  g1->SetTitle("interfence; z[mm]; Intensity");
  g1->SetMarkerColor(8);
  g2->SetMarkerColor(9);

  g1->Draw("AP");
  g2->Draw("P");
  c1->SaveAs(Form("./picture/Draw_Theory_Green_%g_%g_%g.png", a, b, L));
}
