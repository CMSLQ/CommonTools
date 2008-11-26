//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun  5 17:08:28 2008 by ROOT version 5.10/00
// from TTree RootNtupleMaker/RootNtupleMaker
// found on file: /u1/santanas/Leptoquark/RootNtuples/CSA07Soups/CSA07Electron_CMSSW_1_6_7-CSA07-Chowder-A3-PDElectron-ReReco-100pb-Skims7-susyHLTElectronPhoton/output/rtuple_CSA07Electron_CMSSW_1_6_7-CSA07-Chowder-A3-PDElectron-ReReco-100pb-Skims7-susyHLTElectronPhoton_1.root
//////////////////////////////////////////////////////////

#ifndef Selection_h
#define Selection_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class Selection {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TString        *outputFileName_; //output file name (.root)
   TString        *eventProcess_; //event process
   float          Kfactor_;
   int            NevNoCut_;

   //Selection cuts
   //   Float_t TrkIsoCut_;
   //   Float_t HcalIsoCut_;
   //   Float_t LoosePtCut_;
   //   Float_t PT1Cut_;
   //   Float_t PT2Cut_;
   //   Float_t Eta1Cut_;
   //   Float_t Eta2Cut_;
   //   Float_t METCut_; 
   //   Float_t Kfactor_; 
   //   Float_t Rescale_weight_; 
   //   Float_t isSoup_; 


   // Declaration of leave types
   Int_t           event;
   Int_t           run;
   Float_t         cross_section;
   Float_t         auto_cross_section;
   Int_t           processID;
   Int_t           ALPGENprocessID;
   Float_t         filter_eff;
   Float_t         pthat;
   Float_t         weight;
   Int_t           GenParticleCount;
   Float_t         GenParticleE[51];   //[GenParticleCount]
   Float_t         GenParticleP[51];   //[GenParticleCount]
   Float_t         GenParticlePt[51];   //[GenParticleCount]
   Float_t         GenParticlePx[51];   //[GenParticleCount]
   Float_t         GenParticlePy[51];   //[GenParticleCount]
   Float_t         GenParticlePz[51];   //[GenParticleCount]
   Int_t           GenParticlePdgId[51];   //[GenParticleCount]
   Float_t         GenParticleEta[51];   //[GenParticleCount]
   Float_t         GenParticlePhi[51];   //[GenParticleCount]
   Float_t         GenParticleVX[51];   //[GenParticleCount]
   Float_t         GenParticleVY[51];   //[GenParticleCount]
   Float_t         GenParticleVZ[51];   //[GenParticleCount]
   Int_t           GenParticleMotherIndex[51];   //[GenParticleCount]
   Int_t           GenParticleNumDaught[51];   //[GenParticleCount]
   Int_t           hltCount;
   Int_t           hltNamesLen;
   Char_t          HLTNames[1359];   //[hltNamesLen]
   Bool_t          HLTResults[90];   //[hltCount]
   Int_t           eleCount;
   Float_t         eleEta[7];   //[eleCount]
   Float_t         elePhi[7];   //[eleCount]
   Float_t         elePt[7];   //[eleCount]
   Float_t         eleEnergy[7];   //[eleCount]
   Float_t         eleCaloEnergy[7];   //[eleCount]
   Float_t         eleHoE[7];   //[eleCount]
   Float_t         eleSigmaEE[7];   //[eleCount]
   Float_t         eleDeltaPhiTrkSC[7];   //[eleCount]
   Float_t         eleDeltaEtaTrkSC[7];   //[eleCount]
   Float_t         eleTrkIso[7];   //[eleCount]
   Float_t         eleNumTrkIso[7];   //[eleCount]
   Float_t         eleEcalIso[7];   //[eleCount]
   Int_t           genJetCount;
   Float_t         genJetEta[11];   //[genJetCount]
   Float_t         genJetPhi[11];   //[genJetCount]
   Float_t         genJetPt[11];   //[genJetCount]
   Float_t         genJetEnergy[11];   //[genJetCount]
   Float_t         genJetEMF[11];   //[genJetCount]
   Float_t         genJetHADF[11];   //[genJetCount]
   Int_t           caloJetIC5Count;
   Float_t         caloJetIC5Eta[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5Phi[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5Pt[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5Energy[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5Pt_raw[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5Energy_raw[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5EMF[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5HADF[11];   //[caloJetIC5Count]
   Float_t         caloJetIC5_L5corr[11];   //[caloJetIC5Count]
   Int_t           muonCount;
   Float_t         muonEta[3];   //[muonCount]
   Float_t         muonPhi[3];   //[muonCount]
   Float_t         muonPt[3];   //[muonCount]
   Float_t         muonEnergy[3];   //[muonCount]
   Float_t         muonTrkHits[3];   //[muonCount]
   Float_t         muonTrkD0[3];   //[muonCount]
   Float_t         muonTrkDz[3];   //[muonCount]
   Float_t         muonEcalIso[3];   //[muonCount]
   Float_t         muonTrkIso[3];   //[muonCount]
   Float_t         muonHcalIso[3];   //[muonCount]
   Float_t         muonHOIso[3];   //[muonCount]
   Float_t         genMET;
   Float_t         MET;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_runnum;   //!
   TBranch        *b_cross_section;   //!
   TBranch        *b_auto_cross_section;   //!
   TBranch        *b_processID;   //!
   TBranch        *b_ALPGENprocessID;   //!
   TBranch        *b_filter_eff;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_GenParticleCount;   //!
   TBranch        *b_GenParticleE;   //!
   TBranch        *b_GenParticleP;   //!
   TBranch        *b_GenParticlePt;   //!
   TBranch        *b_GenParticlePx;   //!
   TBranch        *b_GenParticlePy;   //!
   TBranch        *b_GenParticlePz;   //!
   TBranch        *b_GenParticlePdgId;   //!
   TBranch        *b_GenParticleEta;   //!
   TBranch        *b_GenParticlePhi;   //!
   TBranch        *b_GenParticleVX;   //!
   TBranch        *b_GenParticleVY;   //!
   TBranch        *b_GenParticleVZ;   //!
   TBranch        *b_GenParticleMotherIndex;   //!
   TBranch        *b_GenParticleNumDaught;   //!
   TBranch        *b_hltCount;   //!
   TBranch        *b_hltNamesLen;   //!
   TBranch        *b_HLTNames;   //!
   TBranch        *b_HLTResults;   //!
   TBranch        *b_eleCount;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_eleEnergy;   //!
   TBranch        *b_eleCaloEnergy;   //!
   TBranch        *b_eleHoE;   //!
   TBranch        *b_eleSigmaEE;   //!
   TBranch        *b_eleDeltaPhiTrkSC;   //!
   TBranch        *b_eleDeltaEtaTrkSC;   //!
   TBranch        *b_eleTrkIso;   //!
   TBranch        *b_eleNumTrkIso;   //!
   TBranch        *b_eleEcalIso;   //!
   TBranch        *b_genJetCount;   //!
   TBranch        *b_genJetEta;   //!
   TBranch        *b_genJetPhi;   //!
   TBranch        *b_genJetPt;   //!
   TBranch        *b_genJetEnergy;   //!
   TBranch        *b_genJetEMF;   //!
   TBranch        *b_genJetHADF;   //!
   TBranch        *b_caloJetIC5Count;   //!
   TBranch        *b_caloJetIC5Eta;   //!
   TBranch        *b_caloJetIC5Phi;   //!
   TBranch        *b_caloJetIC5Pt;   //!
   TBranch        *b_caloJetIC5Energy;   //!
   TBranch        *b_caloJetIC5Pt_raw;   //!
   TBranch        *b_caloJetIC5Energy_raw;   //!
   TBranch        *b_caloJetIC5EMF;   //!
   TBranch        *b_caloJetIC5HADF;   //!
   TBranch        *b_caloJetIC5_L5corr;   //!
   TBranch        *b_muonCount;   //!
   TBranch        *b_muonEta;   //!
   TBranch        *b_muonPhi;   //!
   TBranch        *b_muonPt;   //!
   TBranch        *b_muonEnergy;   //!
   TBranch        *b_muonTrkHits;   //!
   TBranch        *b_muonTrkD0;   //!
   TBranch        *b_muonTrkDz;   //!
   TBranch        *b_muonEcalIso;   //!
   TBranch        *b_muonTrkIso;   //!
   TBranch        *b_muonHcalIso;   //!
   TBranch        *b_muonHOIso;   //!
   TBranch        *b_genMET;   //!
   TBranch        *b_MET;   //!


   Selection(TChain *chain=0,TString *outputFileName=0,TString *eventProcess=0,float Kfactor=1., int NevNoCut=0);
   ~Selection();
   Int_t    Cut(Long64_t entry);
   Int_t    GetEntry(Long64_t entry);
   Long64_t LoadTree(Long64_t entry);
   void     Init(TChain *chain);
   void     Loop();
   Bool_t   Notify();
   void     Show(Long64_t entry = -1);
   //void     SetCuts(float cut1,float cut2, float cut3, float cut4, float cut5, float cut6, float cut7, float cut8, float cut9,float cut10,float cut11);

};

#endif

