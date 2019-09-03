#define hip_cxx
#include "hip.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void hip::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L hip.C
//      Root > hip t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH1F* adcSUM_hist = new TH1F("adcSUM", "ADC SUM 0~15ch", 1100, -500.5, 4999.5);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
     
      float adcSUM = 0;

      adcSUM = adc[0]-802 
	+ adc[1]-815 
	+ adc[2]-810
	+ adc[3]-807 
	+ adc[4]-815
	+ adc[5]-810 
	+ adc[6]-815 
	+ adc[7]-820 
	+ adc[8]-815 
	+ adc[9]-807 
	+ adc[10]-806 
	+ adc[11]-817 
	+ adc[12]-810 
	+ adc[13]-813 
	+ adc[14]-823 
	+ adc[15]-810;

      /*for (int ich = 0; ich < 16; ich++) {
	adcSUM += adc[ich] - 820; //pedastal
	} */

      adcSUM_hist->Fill(adcSUM);


      // if (Cut(ientry) < 0) continue;
   }
}
