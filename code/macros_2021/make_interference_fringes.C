#include "TFile.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TF1.h"

#include <vector>
#include <utility>

TH1D* get_hist(TString filename){
    TFile* file = TFile::Open(filename);
    auto hist = (TH1D*) file->Get("ADC_HIGH_12");
    return hist;
}


void make_interference_fringes() {
    std::vector<std::pair<TString, double>> filenames_point;
    std::vector<TH1D*> hists;
    std::vector<TF1*> funcs;

    auto graph = new TGraphErrors();
    graph->SetMarkerStyle(8);

    // make filelist below
    for (int i = 0; i < 20; i++){
        filenames_point.push_back(std::make_pair(
            Form("../data/test20_%d.root", i),
            (double)i
        ));
    }

    for(int i = 0; i < filenames_point.size(); i++){
        hists.push_back(get_hist(filenames_point.at(i).first));
        funcs.push_back(new TF1(Form("f_%d", i), "gaus", 0, 4096));
        hists.at(i)->Fit(funcs.at(i), "R");
        
        int i_point = graph->GetN();
        graph->SetPoint(i_point, filenames_point.at(i).second, funcs.at(i)->GetParameter(1));
        graph->SetPointError(i_point, 0, funcs.at(i)->GetParError(1));
    }

    graph->Draw("AP");

}

