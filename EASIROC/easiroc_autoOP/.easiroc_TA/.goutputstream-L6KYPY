/*
*   decode.cc   (EASIROC-MODULE+FT decode-tool)
*/
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TTree.h>
#include <TString.h>

void decode(void){
  //std::cout << "Usage: dat2root(const char *filename, const char *outname)"
  // std::cout << "Usage: decode(char \*filename, char \*outname)"
  std::cout << "Usage: decode [input.dat] [output.root]"
	    << std::endl;
  exit(0);
}

void  decode(char *filename, char *outname){
 gROOT->Reset();
 gROOT->SetStyle("Modern");

 std::ifstream datfile(filename,std::ios::binary);
 if(!datfile.is_open()) decode();

 ////////// Initialize
 const int NumOfSeg1 = 32;//layer1
 const int NumOfSeg2 = 32;//layer2
 const int NumOfSeg  = 64;//all

 ////////// data set
 // adc1,2:  raw adc
 // madc1,2: max adc
 // p0, p1:  parameters for normalization; p0:ped, p1:gain
 // de1,2:   delta E (number of photon)
 // med1,2:  max delta E and next
 // seg1,2:  hit pattern of each layer
 // mseg1,2: hit pattern of each layer only max channel

 int  adc1[NumOfSeg1],  adc2[NumOfSeg];
 int madc1[NumOfSeg1], madc2[NumOfSeg];
 double p0[NumOfSeg] =//pedestal' value(input HERE!!)
   {   
     814.13 , 817.665, 810.914, 815.145, 819.983, 823.891, 825.663, 821.265,
     817.138, 817.185, 816.983, 812.087, 812.968, 822.839, 813.208, 817.945,
     816.306, 816.992, 817.824, 813.527, 817.357, 812.86 , 818.276, 823.108,
     816.71 , 819.709, 826.087, 813.643, 813.002, 813.83 , 819.163, 817.873,
     
     795.442, 794.293, 802.572, 798.565, 797.901, 791.884, 788.835, 804.612,
     797.462, 796.477, 797.746, 790.7  , 796.871, 792.186, 787.963, 796.612,
     795.298, 797.445, 792.847, 791.242, 789.422, 795.752, 796.307, 796.185,
     799.615, 796.291, 794.955, 797.667, 793.205, 802.05 , 793.188, 803.997
   };
 double p1[NumOfSeg] =//peak-interval(gain)' value(input HERE!!)
   {
     41.72, 40.26, 39.96, 40.93, 41.59, 41.65, 41.17, 41.76,
     42.82, 42.74, 42.79, 42.73, 43.04, 42.59, 43.18, 43.46,
     40.72, 39.06, 40.24, 39.86, 39.91, 41.10, 40.05, 41.28,
     39.12, 39.85, 39.44, 40.69, 40.57, 40.18, 40.45, 40.02,
     
     42.35, 38.80, 38.02, 38.70, 39.73, 38.64, 38.94, 40.20,
     39.77, 38.99, 39.64, 39.06, 40.67, 39.55, 40.83, 38.92,
     39.24, 40.11, 40.85, 40.03, 39.58, 39.13, 38.54, 39.51,
     39.31, 40.11, 41.88, 40.51, 42.40, 40.48, 42.07, 42.94
   };
 //double th[NumOfSeg] = p1[NumOfSeg]+3*(p1[NumOfSeg])/2;//threshold(2p.e)
 double de1[NumOfSeg1], de2[NumOfSeg], mde1[3], mde2[3];
 int seg1, seg2, mseg1, mseg2;

 TTree* tree = new TTree("tree", "Light yield from MPPC");
 tree->Branch("adc1",   adc1,  "adc1[32]/I");//X-side ADC
 tree->Branch("adc2",   adc2,  "adc2[64]/I");//Y-side ADC
 tree->Branch("de1",    de1,   "de1[32]/D");//X-side Detected Electron
 tree->Branch("de2",    de2,   "de2[64]/D");//Y-side Detected Electron
 //tree->Branch("madc1",  madc1, "madc1[3]/I");//X-side MAXADC
 //tree->Branch("madc2",  madc2, "madc2[3]/I");//Y-side MAXADC
 //tree->Branch("mde1",   mde1,  "mde1[3]/D");//
 // tree->Branch("mde2",   mde2,  "mde2[3]/D");//
 tree->Branch("mseg1", &mseg1, "mseg1/I");//X-side max adc' CH in event by event
 tree->Branch("mseg2", &mseg2, "mseg2/I");//Y-side max adc' CH in event by event


 
const int NbinAdc = 4096;
const double MinAdc =    0.;
const double MaxAdc = 4096.;

const int NbindE = 800;
const double MindE = -32.;
const double MaxdE = 128.;
 
 TH2D* h11 = new TH2D("h11","Adc1 % ch", NumOfSeg1, 0, NumOfSeg1, NbinAdc, MinAdc, MaxAdc);
 TH2D* h12 = new TH2D("h12","Adc2 % ch", NumOfSeg2, 32, 32+NumOfSeg2, NbinAdc, MinAdc, MaxAdc);
 TH2D* h21 = new TH2D("h21", "Pe1 % ch", NumOfSeg1, 0, NumOfSeg1, NbindE, MindE, MaxdE);
 TH2D* h22 = new TH2D("h22", "Pe2 % ch", NumOfSeg2, 32, 32+NumOfSeg2, NbindE, MindE, MaxdE);
 TH2D* h3 = new TH2D("h3","MaxPeCh2 % MaxPeCh1",32,0,32,32,32,64);

 /////////// analysis start
 while(!datfile.eof()){
   unsigned int val;
   datfile.read((char*)&val, sizeof(int));
   //    std::cout << std::hex << val << std::endl;
   if(val == 0xffffea0c){
     datfile.read((char*)&val, sizeof(int));

     /***** layer 1 *****/
     double mbuf1 = 0.;
     int imax = 0;
     for(int i=0; i<33; ++i){
	datfile.read((char*)&val, sizeof(int));
	if(i==0) continue;
	int buf1 = val & 0xffff;
	adc1[i-1] = buf1;
	de1[i-1]  = ((double)buf1-p0[i-1])/p1[i-1]; //adc->#p.e@L1
	h11->Fill(i-1,adc1[i-1]);
	h21->Fill(i-1,de1[i-1]);
	if(adc1[i-1]>mbuf1){
	  imax  = i-1;
	  mbuf1 = adc1[i-1];
	}	
     }
     //std::cout << "imax = " << imax << std::endl;
     mseg1 = imax;

     /***** layer 2 *****/
     double mbuf2 = 0.;
     int jmax  = 0;
     for(int j=33; j<65; ++j){
	datfile.read((char*)&val, sizeof(int));
	////if(j==16) continue;
	int buf2 = val & 0xffff;
	adc2[j-1] = buf2;
	de2[j-1]  = ((double)buf2-p0[j-1])/p1[j-1]; //adc->#p.e@L2
	h12->Fill(j-1,adc2[j-1]);
	h22->Fill(j-1,de2[j-1]);
	if(de2[j-1]>mbuf2){
	  jmax  = j-1;
	  mbuf2 = de2[j-1];
	}
     }

     madc2[jmax] = adc2[jmax];
     //mde2[jmax] = de2[jmax];
     mseg2 = jmax;
     tree->Fill();
     
     gStyle->SetPalette(1);
     h3->Fill(mseg1,mseg2);
   }   //if(val == 0xffffea0c){
 }//datfile.eof()){
 
 TFile *fout = new TFile(outname,"recreate");
 tree->Write();
 tree->Print();
 h11->Write();
 h12->Write();
 h21->Write();
 h22->Write();
 h3->Write();
 fout->Close();

 return;
}
