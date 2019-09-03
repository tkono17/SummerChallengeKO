//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Dec 27 10:20:24 2016 by ROOT version 5.34/32
// from TTree tree/tree
// found on file: root2016/run0053.root
//////////////////////////////////////////////////////////

#ifndef hip_h
#define hip_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class hip {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           tdc[32][16];
   Int_t           tdc_n[32][16];
   Int_t           adc[32];

   // List of branches
   TBranch        *b_tdc;   //!
   TBranch        *b_tdc_n;   //!
   TBranch        *b_adc;   //!

   hip(TTree *tree=0);
   virtual ~hip();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef hip_cxx
hip::hip(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root2016/run0074_0104.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root2016/run0074_0104.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

hip::~hip()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t hip::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t hip::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void hip::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("tdc", tdc, &b_tdc);
   fChain->SetBranchAddress("tdc_n", tdc_n, &b_tdc_n);
   fChain->SetBranchAddress("adc", adc, &b_adc);
   Notify();
}

Bool_t hip::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void hip::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t hip::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef hip_cxx
