void spectrum() {
  Double_t a;
   Int_t i,nfound=0,bin;
   Int_t nbins = 10000;
   Int_t xmin = 0;
   Int_t xmax = nbins;
   Double_t * source = new Double_t[nbins];
   Double_t * dest = new Double_t[nbins];

TFile * f = new TFile ("../data/test20_10.root");
TH1F * h = ( TH1F *) f -> Get("ADC_HIGH_12");
TH1F *d = new TH1F("d","",nbins,xmin,xmax);
   for (i = 0; i < nbins;i++) source[i]=h->GetBinContent(i + 1);
//      TCanvas *c1 = gROOT->GetListOfCanvases()->FindObject("ADC_HIGH_12");
   TCanvas* c1 = new TCanvas("c1","c1",10,10,1000,700);
h->Draw();

Int_t maxpeaks = 10;
TSpectrum *s = new TSpectrum(maxpeaks);
//searching for candidate peaks positions
  nfound = s->SearchHighRes(source, dest, nbins, 2, 0.5, kFALSE, 3, kFALSE, 3);
  Bool_t *FixPos = new Bool_t[nfound];
  Bool_t *FixAmp = new Bool_t[nfound];
  for(i = 0; i< nfound ; i++){
     FixPos[i] = kFALSE;
     FixAmp[i] = kFALSE;
  }
  //filling in the initial estimates of the input parameters
  Double_t *PosX = new Double_t[nfound];
  Double_t *PosY = new Double_t[nfound];
  PosX = s->GetPositionX();
  for (i = 0; i < nfound; i++) {
     a=PosX[i];
     bin = 1 + Int_t(a + 0.5);
     PosY[i] = h->GetBinContent(bin);
  }
  TSpectrumFit *pfit = new TSpectrumFit(nfound);
  pfit->SetFitParameters(xmin, xmax-1, 1000, 0.1, pfit->kFitOptimChiCounts,
  pfit->kFitAlphaHalving, pfit->kFitPower2,
  pfit->kFitTaylorOrderFirst);
  pfit->SetPeakParameters(2, kFALSE, PosX, (Bool_t *) FixPos, PosY, (Bool_t *) FixAmp);
  pfit->FitStiefel(source);
  Double_t *CalcPositions = new Double_t[nfound];
  Double_t *CalcAmplitudes = new Double_t[nfound];
  CalcPositions=pfit->GetPositions();
  CalcAmplitudes=pfit->GetAmplitudes();
  for (i = 0; i < nbins; i++) d->SetBinContent(i + 1,source[i]);
  d->SetLineColor(kRed);
  d->Draw("SAMEL");
  for (i = 0; i < nfound; i++) {
     a=CalcPositions[i];
     bin = 1 + Int_t(a + 0.5);
     PosX[i] = d->GetBinCenter(bin);
     PosY[i] = d->GetBinContent(bin);
  }
  TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
  if (pm) {
     h->GetListOfFunctions()->Remove(pm);
     delete pm;
  }
  pm = new TPolyMarker(nfound, PosX, PosY);
  h->GetListOfFunctions()->Add(pm);
  pm->SetMarkerStyle(23);
  pm->SetMarkerColor(kRed);
  pm->SetMarkerSize(1);

}
