#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TSpectrum.h"
#include "TF1.h"


TString getMultiGaus(Int_t gaus_num){
    // create gaus + gaus + ...
    TString multi_gaus = "";
    for (Int_t i=0; i<gaus_num; i++){ multi_gaus += "+gaus(" + std::to_string(3*i) + ")"; }
    multi_gaus = multi_gaus(1, multi_gaus.Length()-1);
    return multi_gaus;
}

void example01() {
    // open root file
    TFile* file = TFile::Open("../data/led3018.root");
    // get ttree from root file
    TTree* tree = (TTree*)file->Get("tree");
    // set branch address
    Int_t adc_high_gain[64];
    tree->SetBranchAddress("VadcHigh", adc_high_gain);

    // initialize histogram
    const TString HIST_NAME = "hist";
    const TString HIST_TITLE = "hist_title;x_axis_title;y_axis_title";
    const Int_t N_BINS = 4096;
    const Double_t X_MIN = 0;
    const Double_t X_MAX = 4096;
    TH1D* hist = new TH1D(HIST_NAME, HIST_TITLE, N_BINS, X_MIN, X_MAX);
    
    // fill tree data into hist
    const Int_t N_ENTRIES = tree->GetEntries();
    for (Int_t i = 0; i < N_ENTRIES; i++){
        tree->GetEntry(i);
        hist->Fill((Double_t)adc_high_gain[26]);
    }

    // initialize TSpectrum
    const Int_t MAX_SEARCH_PEAKS = 20;
    const Double_t SEARCH_SIGMA = 10;
    TSpectrum* spectrum = new TSpectrum(MAX_SEARCH_PEAKS);

    // search peaks in histogram
    spectrum->Search(hist, SEARCH_SIGMA, "new");
    Int_t n_peaks = spectrum->GetNPeaks();
    Double_t* x_peaks = spectrum->GetPositionX();
    Double_t* y_peaks = spectrum->GetPositionY();

    // make fitting function
    const Double_t FIT_RANGE_MIN = 800;
    const Double_t FIT_RANGE_MAX = 1260;
    const TString MULTI_GAUS_STRING = getMultiGaus(n_peaks);
    const TString FIITING_FUNC_NAME = "multi_gaussian";
    TF1* f = new TF1(FIITING_FUNC_NAME, MULTI_GAUS_STRING, FIT_RANGE_MIN, FIT_RANGE_MAX);

    // set parameter names
    for (Int_t i=0; i<n_peaks; i++) {
        f->SetParName(3*i + 0, (TString)std::to_string(i) + (TString)"th Const");
        f->SetParName(3*i + 1, (TString)std::to_string(i) + (TString)"th Mean"); 
        f->SetParName(3*i + 2, (TString)std::to_string(i) + (TString)"th Sigma"); 
    }

    // set fitting initial parameters
    const Double_t INIT_SIGMA = 20;
    for (Int_t i=0; i<n_peaks; i++) {
        f->SetParameter(3*i + 0, y_peaks[i]);
        f->SetParameter(3*i + 1, x_peaks[i]);
        f->SetParameter(3*i + 2, INIT_SIGMA);
        f->SetParLimits(3*i + 2, 0, 100);
    }

    // execute fitting
    hist->Fit(f, "R");

    // set showing range
    hist->GetXaxis()->SetRangeUser(750, 1350);

    // show
    hist->Draw();
}