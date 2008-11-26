#include "Selection.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

using namespace std;

Selection::Selection(TChain *chain,TString *outputFileName,TString *eventProcess, float Kfactor, int NevNoCut)
{
  
  if (chain == 0) 
    {
      cout << "ERROR in Selection::Selection(TChain *chain,TString *outputFileName): chain not defined" << endl;
    }
  
  if (outputFileName == 0) 
    {
      cout << "ERROR in Selection::Selection(TChain *chain,TString *outputFileName): outputFileName not defined" << endl;
    }
  
  outputFileName_ = outputFileName;
  eventProcess_ = eventProcess;
  Kfactor_ = Kfactor;
  NevNoCut_ = NevNoCut;
   
  Init(chain);
}

Selection::~Selection()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Selection::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Selection::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Selection::Init(TChain *chain)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set branch addresses
   if (chain == 0) return;
   fChain = chain;
   fCurrent = -1;
   fChain->SetMakeClass(1);


   fChain->SetBranchAddress("event",&event);
   fChain->SetBranchAddress("run",&run);
   fChain->SetBranchAddress("cross_section",&cross_section);
   fChain->SetBranchAddress("auto_cross_section",&auto_cross_section);
   fChain->SetBranchAddress("processID",&processID);
   fChain->SetBranchAddress("ALPGENprocessID",&ALPGENprocessID);
   fChain->SetBranchAddress("filter_eff",&filter_eff);
   fChain->SetBranchAddress("pthat",&pthat);
   fChain->SetBranchAddress("weight",&weight);
   fChain->SetBranchAddress("GenParticleCount",&GenParticleCount);
   fChain->SetBranchAddress("GenParticleE",GenParticleE);
   fChain->SetBranchAddress("GenParticleP",GenParticleP);
   fChain->SetBranchAddress("GenParticlePt",GenParticlePt);
   fChain->SetBranchAddress("GenParticlePx",GenParticlePx);
   fChain->SetBranchAddress("GenParticlePy",GenParticlePy);
   fChain->SetBranchAddress("GenParticlePz",GenParticlePz);
   fChain->SetBranchAddress("GenParticlePdgId",GenParticlePdgId);
   fChain->SetBranchAddress("GenParticleEta",GenParticleEta);
   fChain->SetBranchAddress("GenParticlePhi",GenParticlePhi);
   fChain->SetBranchAddress("GenParticleVX",GenParticleVX);
   fChain->SetBranchAddress("GenParticleVY",GenParticleVY);
   fChain->SetBranchAddress("GenParticleVZ",GenParticleVZ);
   fChain->SetBranchAddress("GenParticleMotherIndex",GenParticleMotherIndex);
   fChain->SetBranchAddress("GenParticleNumDaught",GenParticleNumDaught);
   fChain->SetBranchAddress("hltCount",&hltCount);
   fChain->SetBranchAddress("hltNamesLen",&hltNamesLen);
   fChain->SetBranchAddress("HLTNames",HLTNames);
   fChain->SetBranchAddress("HLTResults",HLTResults);
   fChain->SetBranchAddress("eleCount",&eleCount);
   fChain->SetBranchAddress("eleEta",eleEta);
   fChain->SetBranchAddress("elePhi",elePhi);
   fChain->SetBranchAddress("elePt",elePt);
   fChain->SetBranchAddress("eleEnergy",eleEnergy);
   fChain->SetBranchAddress("eleCaloEnergy",eleCaloEnergy);
   fChain->SetBranchAddress("eleHoE",eleHoE);
   fChain->SetBranchAddress("eleSigmaEE",eleSigmaEE);
   fChain->SetBranchAddress("eleDeltaPhiTrkSC",eleDeltaPhiTrkSC);
   fChain->SetBranchAddress("eleDeltaEtaTrkSC",eleDeltaEtaTrkSC);
   fChain->SetBranchAddress("eleTrkIso",eleTrkIso);
   fChain->SetBranchAddress("eleNumTrkIso",eleNumTrkIso);
   fChain->SetBranchAddress("eleEcalIso",eleEcalIso);
   fChain->SetBranchAddress("genJetCount",&genJetCount);
   fChain->SetBranchAddress("genJetEta",genJetEta);
   fChain->SetBranchAddress("genJetPhi",genJetPhi);
   fChain->SetBranchAddress("genJetPt",genJetPt);
   fChain->SetBranchAddress("genJetEnergy",genJetEnergy);
   fChain->SetBranchAddress("genJetEMF",genJetEMF);
   fChain->SetBranchAddress("genJetHADF",genJetHADF);
   fChain->SetBranchAddress("caloJetIC5Count",&caloJetIC5Count);
   fChain->SetBranchAddress("caloJetIC5Eta",caloJetIC5Eta);
   fChain->SetBranchAddress("caloJetIC5Phi",caloJetIC5Phi);
   fChain->SetBranchAddress("caloJetIC5Pt",caloJetIC5Pt);
   fChain->SetBranchAddress("caloJetIC5Energy",caloJetIC5Energy);
   fChain->SetBranchAddress("caloJetIC5Pt_raw",caloJetIC5Pt_raw);
   fChain->SetBranchAddress("caloJetIC5Energy_raw",caloJetIC5Energy_raw);
   fChain->SetBranchAddress("caloJetIC5EMF",caloJetIC5EMF);
   fChain->SetBranchAddress("caloJetIC5HADF",caloJetIC5HADF);
   fChain->SetBranchAddress("caloJetIC5_L5corr",caloJetIC5_L5corr);
   fChain->SetBranchAddress("muonCount",&muonCount);
   fChain->SetBranchAddress("muonEta",muonEta);
   fChain->SetBranchAddress("muonPhi",muonPhi);
   fChain->SetBranchAddress("muonPt",muonPt);
   fChain->SetBranchAddress("muonEnergy",muonEnergy);
   fChain->SetBranchAddress("muonTrkHits",muonTrkHits);
   fChain->SetBranchAddress("muonTrkD0",muonTrkD0);
   fChain->SetBranchAddress("muonTrkDz",muonTrkDz);
   fChain->SetBranchAddress("muonEcalIso",muonEcalIso);
   fChain->SetBranchAddress("muonTrkIso",muonTrkIso);
   fChain->SetBranchAddress("muonHcalIso",muonHcalIso);
   fChain->SetBranchAddress("muonHOIso",muonHOIso);
   fChain->SetBranchAddress("genMET",&genMET);
   fChain->SetBranchAddress("MET",&MET);


   Notify();

}

Bool_t Selection::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.


   // Get branch pointers
   b_event = fChain->GetBranch("event");
   b_runnum = fChain->GetBranch("run");
   b_cross_section = fChain->GetBranch("cross_section");
   b_auto_cross_section = fChain->GetBranch("auto_cross_section");
   b_processID = fChain->GetBranch("processID");
   b_ALPGENprocessID = fChain->GetBranch("ALPGENprocessID");
   b_filter_eff = fChain->GetBranch("filter_eff");
   b_pthat = fChain->GetBranch("pthat");
   b_weight = fChain->GetBranch("weight");
   b_GenParticleCount = fChain->GetBranch("GenParticleCount");
   b_GenParticleE = fChain->GetBranch("GenParticleE");
   b_GenParticleP = fChain->GetBranch("GenParticleP");
   b_GenParticlePt = fChain->GetBranch("GenParticlePt");
   b_GenParticlePx = fChain->GetBranch("GenParticlePx");
   b_GenParticlePy = fChain->GetBranch("GenParticlePy");
   b_GenParticlePz = fChain->GetBranch("GenParticlePz");
   b_GenParticlePdgId = fChain->GetBranch("GenParticlePdgId");
   b_GenParticleEta = fChain->GetBranch("GenParticleEta");
   b_GenParticlePhi = fChain->GetBranch("GenParticlePhi");
   b_GenParticleVX = fChain->GetBranch("GenParticleVX");
   b_GenParticleVY = fChain->GetBranch("GenParticleVY");
   b_GenParticleVZ = fChain->GetBranch("GenParticleVZ");
   b_GenParticleMotherIndex = fChain->GetBranch("GenParticleMotherIndex");
   b_GenParticleNumDaught = fChain->GetBranch("GenParticleNumDaught");
   b_hltCount = fChain->GetBranch("hltCount");
   b_hltNamesLen = fChain->GetBranch("hltNamesLen");
   b_HLTNames = fChain->GetBranch("HLTNames");
   b_HLTResults = fChain->GetBranch("HLTResults");
   b_eleCount = fChain->GetBranch("eleCount");
   b_eleEta = fChain->GetBranch("eleEta");
   b_elePhi = fChain->GetBranch("elePhi");
   b_elePt = fChain->GetBranch("elePt");
   b_eleEnergy = fChain->GetBranch("eleEnergy");
   b_eleCaloEnergy = fChain->GetBranch("eleCaloEnergy");
   b_eleHoE = fChain->GetBranch("eleHoE");
   b_eleSigmaEE = fChain->GetBranch("eleSigmaEE");
   b_eleDeltaPhiTrkSC = fChain->GetBranch("eleDeltaPhiTrkSC");
   b_eleDeltaEtaTrkSC = fChain->GetBranch("eleDeltaEtaTrkSC");
   b_eleTrkIso = fChain->GetBranch("eleTrkIso");
   b_eleNumTrkIso = fChain->GetBranch("eleNumTrkIso");
   b_eleEcalIso = fChain->GetBranch("eleEcalIso");
   b_genJetCount = fChain->GetBranch("genJetCount");
   b_genJetEta = fChain->GetBranch("genJetEta");
   b_genJetPhi = fChain->GetBranch("genJetPhi");
   b_genJetPt = fChain->GetBranch("genJetPt");
   b_genJetEnergy = fChain->GetBranch("genJetEnergy");
   b_genJetEMF = fChain->GetBranch("genJetEMF");
   b_genJetHADF = fChain->GetBranch("genJetHADF");
   b_caloJetIC5Count = fChain->GetBranch("caloJetIC5Count");
   b_caloJetIC5Eta = fChain->GetBranch("caloJetIC5Eta");
   b_caloJetIC5Phi = fChain->GetBranch("caloJetIC5Phi");
   b_caloJetIC5Pt = fChain->GetBranch("caloJetIC5Pt");
   b_caloJetIC5Energy = fChain->GetBranch("caloJetIC5Energy");
   b_caloJetIC5Pt_raw = fChain->GetBranch("caloJetIC5Pt_raw");
   b_caloJetIC5Energy_raw = fChain->GetBranch("caloJetIC5Energy_raw");
   b_caloJetIC5EMF = fChain->GetBranch("caloJetIC5EMF");
   b_caloJetIC5HADF = fChain->GetBranch("caloJetIC5HADF");
   b_caloJetIC5_L5corr = fChain->GetBranch("caloJetIC5_L5corr");
   b_muonCount = fChain->GetBranch("muonCount");
   b_muonEta = fChain->GetBranch("muonEta");
   b_muonPhi = fChain->GetBranch("muonPhi");
   b_muonPt = fChain->GetBranch("muonPt");
   b_muonEnergy = fChain->GetBranch("muonEnergy");
   b_muonTrkHits = fChain->GetBranch("muonTrkHits");
   b_muonTrkD0 = fChain->GetBranch("muonTrkD0");
   b_muonTrkDz = fChain->GetBranch("muonTrkDz");
   b_muonEcalIso = fChain->GetBranch("muonEcalIso");
   b_muonTrkIso = fChain->GetBranch("muonTrkIso");
   b_muonHcalIso = fChain->GetBranch("muonHcalIso");
   b_muonHOIso = fChain->GetBranch("muonHOIso");
   b_genMET = fChain->GetBranch("genMET");
   b_MET = fChain->GetBranch("MET");

   return kTRUE;
}

void Selection::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Selection::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

// void Selection::SetCuts(float cut1, float cut2, float cut3, float cut4, float cut5, float cut6, float cut7, float cut8, float cut9, float cut10, float cut11)
// {
//   TrkIsoCut_=cut1;
//   HcalIsoCut_=cut2;
//   LoosePtCut_=cut3;
//   PT1Cut_=cut4;
//   PT2Cut_=cut5;
//   Eta1Cut_=cut6;
//   Eta2Cut_=cut7;
//   METCut_=cut8; 
//   Ext_weight_=cut9; 
//   Rescale_weight_=cut10; 
//   isSoup_=cut11; 
// }
