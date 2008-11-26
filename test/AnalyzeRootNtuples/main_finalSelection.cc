#include "Selection.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> //serve per sprintf 
#include <iomanip>

#include <TTree.h>
#include <TChain.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>

using namespace std;


//1)usare array di float ed int per dumpare il numero di eventi selezionato dopo
//  ogni taglio. 

//==================
const int Nparam=6;   //--> NUMBER OF PARAMETERS 
//==================

float Lint=100;//Lint (pb-1)

float ECAL_barrel_limit = 1.479;

float ECAL_FR_1_cut = 1.4;
float ECAL_FR_2_cut = 1.6;
float ECAL_FR_3_cut = 2;

//     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
//     PSet robustEleIDCuts =
//     {
//         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
//         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}

float eleHoE_bar_cut=0.05; 
float eleSigmaEE_bar_cut=0.011;
float eleDeltaEtaTrkSC_bar_cut=0.005;
float eleDeltaPhiTrkSC_bar_cut=0.09;

float eleHoE_end_cut=0.08; 
float eleSigmaEE_end_cut=0.0275;
float eleDeltaEtaTrkSC_end_cut=0.007;
float eleDeltaPhiTrkSC_end_cut=0.09;

float eleEcalIsoAbs_cut1=6;
float eleEcalIsoAbs_cut2=0.01;

float eleHcalIsoAbs_cut1=4;
float eleHcalIsoAbs_cut2=0.005;

float eleTrkIsoAbs_bar_cut=7.5;
float eleTrkIsoAbs_end_cut=15.;

int eleNumTrkIso_cut=4;

//** obsolete
// int eleNumTrkIso_cut=2;
// float eleTrkIso_cut=0.05;
// float eleEcalIso_cut=0.05;
//**

float elePt_cut=30.;
float muonPt_cut=30.;

float ConeSizeJetCleaning_cut=0.5;
float jetPt_cut=50.;
float jetEta_cut=3.;

float ele1stPt_cut=85.;
float ele2ndPt_cut=30.;
float muon1stPt_cut=30.;

float Mee_lowCut=80.;
float Mee_highCut=100.;

float St_Cut=400.;

int main(int argc, char* argv[])
{

  //=============== Check number of input parameters 
  if(argc!=Nparam+1) 
    
    {
      cout << "int main() >> ERROR : arcg = " << argc 
	   << " different from "              << Nparam+1 << endl;
      
      cout << "--- Correct Usage : exe eventProcess inputlist treename outputfilename Kfactor NevNoCut" << endl;
      cout << "--- Correct Usage : eventProcess can be: all, ttbarjet, wjet, zjet, qcdjet, gammajet, leptoquark" 
	   << endl;
      cout << "--- Correct Usage : Kfactor: if = 1 use the cross section in the rootples" << endl;
      cout << "--- Correct Usage : NevNoCut: number of events generated (only used for LQ --> in the other cases you can use 0)" << endl;
    }
  
  else 
    
    {
      //       cout << endl;
      //       cout << "***********************************" << endl;
      //       cout << "******* Launching the main  *******" << endl;
      //       cout << "***********************************" << endl;
      //       cout << endl;      

      //================ Parameters 

      // Event Number List
      TString *eventProcess= new TString(argv[1]);

      // Input list
      char listName[500];
      sprintf(listName,argv[2]); 
      
      // Name of input tree objects in (.root) files 
      char treeName[100];
      sprintf(treeName,argv[3]);
      
      // Output filename (.root)  
      TString *OutputFileName= new TString(argv[4]);

      // External weight 
      float Kfactor= atof(argv[5]);

      //Number of events (used only for LQ)
      int NevNoCut= atoi(argv[6]);

      //================ Creating chain 

      // A TChain is a collection of TFile objects.a
      // the first parameter "name" is the name of the TTree object
      // in the files added with Add.
      // Use TChain::Add to add a new element to this chain.
      
      //       cout << "Loading Data ............" << endl;
      //       cout << "Creating TChain ........." << endl;
      
      TChain *chain = new TChain(treeName);
      char pName[500];
      ifstream is(listName);
      if(is.good()) 
	{
	  cout << "Reading list : " << listName << " ......." << endl;
	  
	  while( is.getline(pName, 500, '\n') )
	    {
	      if (pName[0] == '#') continue;
	      //cout << "   Add: " << pName << endl;
	      chain->Add(pName);
	    }
	  
	  cout <<  "List read " << endl;

	  //================ Run analysis
 
	  Selection analysis(chain,OutputFileName,eventProcess,Kfactor,NevNoCut);

	  //analysis.SetCuts(TrkIsoCut,HcalIsoCut,LoosePtCut,PT1Cut,PT2Cut,Eta1Cut,Eta2Cut,METCut,Kfactor,Rescale_weight,isSoup);
	  analysis.Loop();
	 
	} // end check file is good
      
      else cout << "int main() >> ERROR : file " << listName << " not read" << endl;
      
      is.close();
      
      //cout << ".......... end of program" << endl;
    }// end check argc
}


void Selection::Loop()
{

  char eventProcess[200];
  sprintf(eventProcess,"%s",&string(*eventProcess_)[0]);

  char output_root_title[200];
  sprintf(output_root_title,"%s%s",&string(*outputFileName_)[0],".root");

  // Create output file
  TFile *output_root = new TFile(&output_root_title[0],"RECREATE");
  
  // *** Book Histograms ***
  //TH1F(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup)

  //weight
  TH1F *h_weight = new TH1F ("h_weight","",100,0,100);

  //pThat
  TH1F *h_pThat_unweight = new TH1F ("h_pThat_unweight","",100,0,1000);
  h_pThat_unweight->Sumw2();

  TH1F *h_pThat_weight = new TH1F ("h_pThat_weight","",100,0,1000);
  h_pThat_weight->Sumw2();

  TH1F *h_pThat_sel = new TH1F ("h_pThat_sel","",100,0,1000);
  h_pThat_sel->Sumw2();

  TH1F *h_pThat_sel_MeecutInside = new TH1F ("h_pThat_sel_MeecutInside","",100,0,1000);
  h_pThat_sel_MeecutInside->Sumw2();


  //----------------------------------------------------------------------

  //N electron
  TH1F *h_nEleID = new TH1F ("h_nEleID","",11,-0.5,10.5);
  h_nEleID->Sumw2();

  TH1F *h_nEleFinal_noIso = new TH1F ("h_nEleFinal_noIso","",11,-0.5,10.5);
  h_nEleFinal_noIso->Sumw2();

  TH1F *h_nEleFinal = new TH1F ("h_nEleFinal","",11,-0.5,10.5);
  h_nEleFinal->Sumw2();

  //pT 1st ele
  TH1F *h_pT1stEle = new TH1F ("h_pT1stEle","",100,0,1000);
  h_pT1stEle->Sumw2();
  
  //pT 2nd ele
  TH1F *h_pT2ndEle = new TH1F ("h_pT2ndEle","",100,0,1000);
  h_pT2ndEle->Sumw2();
  
  //eta 1st ele
  TH1F *h_Eta1stEle = new TH1F ("h_Eta1stEle","",100,-3,3);
  h_Eta1stEle->Sumw2();
  
  //eta 2nd ele
  TH1F *h_Eta2ndEle = new TH1F ("h_Eta2ndEle","",100,-3,3);
  h_Eta2ndEle->Sumw2();

  //----------------------------------------------------------------------

  //N jets
  TH1F *h_nJetFinal = new TH1F ("h_nJetFinal","",11,-0.5,10.5);
  h_nJetFinal->Sumw2();

  //   TH1F *h_nJetFinal_pTcut = new TH1F ("h_nJetFinal_pTcut","",11,-0.5,10.5);
  //   h_nJetFinal_pTcut->Sumw2();

  //pT 1st jet
  TH1F *h_pT1stJet = new TH1F ("h_pT1stJet","",100,0,1000);
  h_pT1stJet->Sumw2();
  
  //pT 2nd jet
  TH1F *h_pT2ndJet = new TH1F ("h_pT2ndJet","",100,0,1000);
  h_pT2ndJet->Sumw2();
  
  //eta 1st jet
  TH1F *h_Eta1stJet = new TH1F ("h_Eta1stJet","",100,-5,5);
  h_Eta1stJet->Sumw2();
  
  //eta 2nd jet
  TH1F *h_Eta2ndJet = new TH1F ("h_Eta2ndJet","",100,-5,5);
  h_Eta2ndJet->Sumw2();

  //----------------------------------------------------------------------

  //S_t
  TH1F *h_St = new TH1F ("h_St","",200,0.,2000.);
  h_St->Sumw2();
  
  //----------------------------------------------------------------------

  //Mee
  TH1F *h_Mee = new TH1F ("h_Mee","",200,0.,1000.);
  h_Mee->Sumw2();

  //Mej (all Comb)
  TH1F *h_Mej_allComb = new TH1F ("h_Mej_allComb","",150,0.,1500.);
  h_Mej_allComb->Sumw2();

  TH1F *h_Mej_allComb_Meecut = new TH1F ("h_Mej_allComb_Meecut","",150,0.,1500.);
  h_Mej_allComb_Meecut->Sumw2();

  TH1F *h_Mej_allComb_Meecut_Stcut = new TH1F ("h_Mej_allComb_Meecut_Stcut","",150,0.,1500.);
  h_Mej_allComb_Meecut_Stcut->Sumw2();

  //Mej (best Comb)
  TH1F *h_Mej_best = new TH1F ("h_Mej_best","",150,0.,1500.);
  h_Mej_best->Sumw2();

  TH1F *h_Mej_best_Meecut = new TH1F ("h_Mej_best_Meecut","",150,0.,1500.);
  h_Mej_best_Meecut->Sumw2();

  TH1F *h_Mej_best_Meecut_Stcut = new TH1F ("h_Mej_best_Meecut_Stcut","",150,0.,1500.);
  h_Mej_best_Meecut_Stcut->Sumw2();

  //Mej (wrong Comb)
  TH1F *h_Mej_wrong = new TH1F ("h_Mej_wrong","",150,0.,1500.);
  h_Mej_wrong->Sumw2();

  TH2F *h_2d_Mej_best_vs_wrong = new TH2F ("h_2d_Mej_best_vs_wrong","",150,0.,1500.,150,0.,1500.);
  h_2d_Mej_best_vs_wrong->Sumw2();

  //St vs Mej (Meecut)
  TH2F *h_2d_Mej_allComb_Meecut_vs_St = new TH2F ("h_2d_Mej_allComb_Meecut_vs_St","",200,0.,2000.,150,0.,1500.);
  h_2d_Mej_allComb_Meecut_vs_St->Sumw2();

  TH2F *h_2d_Mej_best_Meecut_vs_St = new TH2F ("h_2d_Mej_best_Meecut_vs_St","",200,0.,2000.,150,0.,1500.);
  h_2d_Mej_best_Meecut_vs_St->Sumw2();


  //Mej (inside M(Z) window)
  TH1F *h_Mej_allComb_MeecutInside_Stcut = new TH1F ("h_Mej_allComb_MeecutInside_Stcut","",150,0.,1500.);
  h_Mej_allComb_MeecutInside_Stcut->Sumw2();

  TH1F *h_Mej_best_MeecutInside_Stcut = new TH1F ("h_Mej_best_MeecutInside_Stcut","",150,0.,1500.);
  h_Mej_best_MeecutInside_Stcut->Sumw2();


  //Mej/Mmuj
  TH1F *h_Memuj_allComb_Memucut_Stcut = new TH1F ("h_Memuj_allComb_Memucut_Stcut","",150,0.,1500.);
  h_Memuj_allComb_Memucut_Stcut->Sumw2();

  TH1F *h_Memuj_best_Memucut_Stcut = new TH1F ("h_Memuj_best_Memucut_Stcut","",150,0.,1500.);
  h_Memuj_best_Memucut_Stcut->Sumw2();


  //----------------------------------------------------------------------

  // *** Inizialize variables ***

  //number of events (absolute)
  float N_abs_nocut=0;               //all events
  float N_abs_skim=0;                //+ LQ skim
  float N_abs_hlt=0;                 //+ hlt
  float N_abs_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float N_abs_2ele=0;                //+ isolation criteria
  float N_abs_2ele_2jets=0;          //+ 2 jets
  float N_abs_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float N_abs_Mee=0;                 //+ Mee cut
  float N_abs_St=0;                  //+ ST cut

  //number of events (in Lint)
  float N_Lint_nocut=0;               //all events
  float N_Lint_skim=0;                //+ LQ skim
  float N_Lint_hlt=0;                 //+ hlt
  float N_Lint_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float N_Lint_2ele=0;                //+ isolation criteria
  float N_Lint_2ele_2jets=0;          //+ 2 jets
  float N_Lint_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float N_Lint_Mee=0;                 //+ Mee cut
  float N_Lint_St=0;                  //+ ST cut

  //error on number of events (absolute)
  float e_N_abs_nocut=0;               //all events
  float e_N_abs_skim=0;                //+ LQ skim
  float e_N_abs_hlt=0;                 //+ hlt
  float e_N_abs_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float e_N_abs_2ele=0;                //+ isolation criteria
  float e_N_abs_2ele_2jets=0;          //+ 2 jets
  float e_N_abs_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float e_N_abs_Mee=0;                 //+ Mee cut
  float e_N_abs_St=0;                  //+ ST cut

  //error on number of events (in Lint)
  float e_N_Lint_nocut=0;               //all events
  float e_N_Lint_skim=0;                //+ LQ skim
  float e_N_Lint_hlt=0;                 //+ hlt
  float e_N_Lint_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float e_N_Lint_2ele=0;                //+ isolation criteria
  float e_N_Lint_2ele_2jets=0;          //+ 2 jets
  float e_N_Lint_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float e_N_Lint_Mee=0;                 //+ Mee cut
  float e_N_Lint_St=0;                  //+ ST cut

  //relative error on number of events (in Lint)
  float e_rel_N_Lint_nocut=0;               //all events
  float e_rel_N_Lint_skim=0;                //+ LQ skim
  float e_rel_N_Lint_hlt=0;                 //+ hlt
  float e_rel_N_Lint_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float e_rel_N_Lint_2ele=0;                //+ isolation criteria
  float e_rel_N_Lint_2ele_2jets=0;          //+ 2 jets
  float e_rel_N_Lint_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float e_rel_N_Lint_Mee=0;                 //+ Mee cut
  float e_rel_N_Lint_St=0;                  //+ ST cut

  //efficiency
  float eff_N_Lint_nocut=0;               //all events
  float eff_N_Lint_skim=0;                //+ LQ skim
  float eff_N_Lint_hlt=0;                 //+ hlt
  float eff_N_Lint_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float eff_N_Lint_2ele=0;                //+ isolation criteria
  float eff_N_Lint_2ele_2jets=0;          //+ 2 jets
  float eff_N_Lint_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float eff_N_Lint_Mee=0;                 //+ Mee cut
  float eff_N_Lint_St=0;                  //+ ST cut
  float eff_N_Lint_final=0;               //final

  //error on efficiency
  float e_eff_N_Lint_nocut=0;               //all events
  float e_eff_N_Lint_skim=0;                //+ LQ skim
  float e_eff_N_Lint_hlt=0;                 //+ hlt
  float e_eff_N_Lint_2ele_noiso=0;          //+ at least 2 electrons (with pT>LoosePtCut) without isolation
  float e_eff_N_Lint_2ele=0;                //+ isolation criteria
  float e_eff_N_Lint_2ele_2jets=0;          //+ 2 jets
  float e_eff_N_Lint_eleCuts_jetCuts=0;     //+ kinematic cuts on electron and jets
  float e_eff_N_Lint_Mee=0;                 //+ Mee cut
  float e_eff_N_Lint_St=0;                  //+ ST cut
  float e_eff_N_Lint_final=0;               //final

  //********** Loop over events *********
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  int myThousCounter=0;
  bool stdLoop=true;

  cout << "Analyzing process : " << eventProcess << " ......." << endl;
      
  //for (Long64_t jentry=0; jentry<2000;jentry++) {
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    //###### Standard loop over all the events #####
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    // Event counter
    if(myThousCounter==1000)
      {
	cout << "event "<< jentry << endl;
	myThousCounter=0;
      }
    myThousCounter++;
    
    //**************************************
    //** Write here your code inside loop **
    //**************************************

    //Include K factor
    weight=weight*Kfactor_;
    //cout << weight << endl;
        
    //Select event process
    bool passEventProcessSelected=false;
    
    if( strcmp(eventProcess, "all")==0 )
      {    
	passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "wjet")==0 ) 
      {
	if(ALPGENprocessID>=1000 && ALPGENprocessID<=1999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "zjet")==0 ) 
      {
	if(ALPGENprocessID>=2000 && ALPGENprocessID<=2999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "ttbarjet")==0 ) 
      {
	if(ALPGENprocessID>=3000 && ALPGENprocessID<=3999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "qcdjet")==0 ) 
      {
	if(processID == 11 || processID == 12 || processID == 13 || processID == 28 || processID == 68 || processID == 53)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "gammajet")==0 ) 
      {
	if(processID == 14 || processID == 18 || processID == 29)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "leptoquark")==0 ) 
      {
	//temporary... to be changed after new FastSim production
	if( (processID == -999 && ALPGENprocessID == -999)
            || processID == 163 || processID == 164 )
	  passEventProcessSelected=true;
      }
    
    //skip event if not the desired process
    if(passEventProcessSelected==false)
      continue;

    // Skip signal events in gumbo (top secret)
    if (processID == 102 || processID == 123 || processID == 124 || processID == 141) 
      continue;

    
    // -------------- Candidates -------------------

    
    //### Electron definition
    vector<int> v_idx_ele_ID;
    vector<int> v_idx_ele_final_noIso;
    vector<int> v_idx_ele_final;

    float eleTrkIsoAbs[eleCount];
    float eleEcalIsoAbs[eleCount];
    float eleHcalIsoAbs[eleCount];

    for(int iele=0;iele<eleCount;iele++)
      {

	bool pass_ECAL_FR=false;
	bool pass_eleID=false;
	bool pass_eleIso=false;
	bool pass_elePt=false;
	
	//ECAL fiducial region
	if(fabs(eleEta[iele]) < ECAL_FR_1_cut 
           || 
	   (fabs(eleEta[iele]) > ECAL_FR_2_cut 
	    && fabs(eleEta[iele]) < ECAL_FR_3_cut)
	   )
	  pass_ECAL_FR=true;


	///Electron ID
	if(fabs(eleEta[iele]) < ECAL_barrel_limit
	   && eleHoE[iele] < eleHoE_bar_cut 
	   && eleSigmaEE[iele] < eleSigmaEE_bar_cut
	   && eleDeltaPhiTrkSC[iele] < eleDeltaPhiTrkSC_bar_cut
	   && eleDeltaEtaTrkSC[iele] < eleDeltaEtaTrkSC_bar_cut
	   )
	  pass_eleID=true;

	if(fabs(eleEta[iele]) > ECAL_barrel_limit
	   && eleHoE[iele] < eleHoE_end_cut 
	   && eleSigmaEE[iele] < eleSigmaEE_end_cut
	   && eleDeltaPhiTrkSC[iele] < eleDeltaPhiTrkSC_end_cut
	   && eleDeltaEtaTrkSC[iele] < eleDeltaEtaTrkSC_end_cut
	   )
	  pass_eleID=true;

	//** obsolete
	// 	if(eleNumTrkIso[iele] <= eleNumTrkIso_cut
	// 	   && eleTrkIso[iele] < eleTrkIso_cut
	// 	   && eleEcalIso[iele] < eleEcalIso_cut
	// 	   )
	// 	  pass_eleIso=true;
	//** 

	//Electron Isolation

	float sinTheta=1/cosh(eleEta[iele]);
	eleTrkIsoAbs[iele]=eleTrkIso[iele]*elePt[iele];
	eleEcalIsoAbs[iele]=eleEcalIso[iele]*elePt[iele];
	eleHcalIsoAbs[iele]=eleHoE[iele]*eleEnergy[iele]*sinTheta;


	if(fabs(eleEta[iele]) < ECAL_barrel_limit
	   && eleTrkIsoAbs[iele] < eleTrkIsoAbs_bar_cut
	   && eleEcalIsoAbs[iele] < (eleEcalIsoAbs_cut1+eleEcalIsoAbs_cut2*elePt[iele])
	   && eleHcalIsoAbs[iele] < 
	   (eleHcalIsoAbs_cut1+eleHcalIsoAbs_cut2*elePt[iele])
	   && eleNumTrkIso[iele] < eleNumTrkIso_cut
	   )
	  pass_eleIso=true;


	if(fabs(eleEta[iele]) > ECAL_barrel_limit
	   && eleTrkIsoAbs[iele] < eleTrkIsoAbs_end_cut
	   && eleEcalIsoAbs[iele] < (eleEcalIsoAbs_cut1+eleEcalIsoAbs_cut2*elePt[iele])
	   && eleHcalIsoAbs[iele] < 
	   (eleHcalIsoAbs_cut1+eleHcalIsoAbs_cut2*elePt[iele])
	   && eleNumTrkIso[iele] < eleNumTrkIso_cut
	   )
	  pass_eleIso=true;


	//pT cut
	if(elePt[iele] > elePt_cut)
	  {
	    pass_elePt=true;
	  }

	//only ID	
	if(pass_eleID)
	  v_idx_ele_ID.push_back(iele);	  

	//full selection except isolation
	if(pass_ECAL_FR && pass_eleID && pass_elePt)
	  v_idx_ele_final_noIso.push_back(iele);	  

	//full selection
	if(pass_ECAL_FR && pass_eleID && pass_eleIso && pass_elePt)
	  v_idx_ele_final.push_back(iele);	  

      }


    //### Jet definition
    vector<int> v_idx_jet_final;
    //vector<int> v_idx_jet_final_pTcut;

    for(int ijet=0;ijet<caloJetIC5Count;ijet++)
      {
	bool skipJet=false;

	TVector3 calojet;
	calojet.SetPtEtaPhi(caloJetIC5Pt[ijet],caloJetIC5Eta[ijet],caloJetIC5Phi[ijet]);	

	for(int iele=0;iele<min(int(v_idx_ele_ID.size()),2);iele++)
	  {

	    TVector3 ele;
	    ele.SetPtEtaPhi(elePt[v_idx_ele_ID[iele]],
			    eleEta[v_idx_ele_ID[iele]],
			    elePhi[v_idx_ele_ID[iele]]);	

	    float DeltaR_ele_jet = calojet.DeltaR(ele);

	    if(DeltaR_ele_jet<ConeSizeJetCleaning_cut)
	      {
		skipJet=true;
		break;
	      }

	  }//end ele loop


	if(skipJet==true)
	  continue;


	if(caloJetIC5Pt[ijet] > jetPt_cut 
	   && fabs(caloJetIC5Eta[ijet]) < jetEta_cut)
	  v_idx_jet_final.push_back(ijet);	  

      }


    //### Muon definition
    vector<int> v_idx_muon_ID;
    vector<int> v_idx_muon_final_noIso;
    vector<int> v_idx_muon_final;

    for(int imuon=0;imuon<muonCount;imuon++)
      {

	bool pass_MUON_FR=false;
	bool pass_muonID=false;
	bool pass_muonIso=false;
	bool pass_muonPt=false;
	
	//MUON fiducial region matching ECAL one
	if(fabs(muonEta[imuon]) < ECAL_FR_1_cut 
           || 
	   (fabs(muonEta[imuon]) > ECAL_FR_2_cut 
	    && fabs(muonEta[imuon]) < ECAL_FR_3_cut)
	   )
	  pass_MUON_FR=true;

	//????????????? ADD isolations ???????????????

	//pT cut
	if(muonPt[imuon] > muonPt_cut)
	  {
	    pass_muonPt=true;
	  }

	//remove fake muons coming from jets
	bool skipMuon=false;

	TVector3 muon;
	muon.SetPtEtaPhi(muonPt[imuon],
			 muonEta[imuon],
			 muonPhi[imuon]);	
	
	for(int ijet=0;ijet<min(int(v_idx_jet_final.size()),2);ijet++)
	  {

	    TVector3 calojet;
	    calojet.SetPtEtaPhi(caloJetIC5Pt[v_idx_jet_final[ijet]],
				caloJetIC5Eta[v_idx_jet_final[ijet]],
				caloJetIC5Phi[v_idx_jet_final[ijet]]);	

	    float DeltaR_muon_jet = muon.DeltaR(calojet);

	    if(DeltaR_muon_jet<ConeSizeJetCleaning_cut)
	      {
		skipMuon=true;
		break;
	      }

	  }//end muon loop

	if(skipMuon==true)
	  continue;

	//full selection
	if(pass_MUON_FR && pass_muonPt)
	  v_idx_muon_final.push_back(imuon);	  

      }



    // -------------  Selection ------------------------

    //## Booleans
    bool pass_HLT=false;

    bool pass_twoEle=false;
    bool pass_twoEle_noIso=false;
    bool pass_twoJet=false;

    bool pass_1stElePtCut=false;
    bool pass_2ndElePtCut=false;

//     bool pass_1stJetEtaCut=false;
//     bool pass_2ndJetEtaCut=false;

    bool pass_Mee=false;
    bool pass_Mee_at_Z=false;
   
    bool pass_St=false;

    //for muon control sample
    bool pass_1Ele1Muon=false;
    bool pass_1stMuonPtCut=false;
    bool pass_Memu=false;
    bool pass_St_mu=false;

    //## Selection criteria

    //## HLT
    if(HLTResults[40]==true || HLTResults[41]==true 
       || 
       (strcmp(eventProcess, "leptoquark")==0 && processID == -999 && ALPGENprocessID == -999)
       ) 
      pass_HLT=true;

    //## 2 ele non Iso
    if(v_idx_ele_final_noIso.size()>=2)
      pass_twoEle_noIso=true;

    //## 2 ele Iso
    if(v_idx_ele_final.size()>=2)
      pass_twoEle=true;

    //## 1 ele Iso + 1 muon Iso
    if(v_idx_muon_final.size()>=1 && v_idx_ele_final.size()>=1)
      pass_1Ele1Muon=true;

    //## 2 jets
    if(v_idx_jet_final.size()>=2)
      pass_twoJet=true;

    //## pT cut 1st ele
    if(v_idx_ele_final.size()>=1)
      {
	if(elePt[v_idx_ele_final[0]] > ele1stPt_cut)
	  pass_1stElePtCut=true;
      }

    //## pT cut 2nd ele
    if(pass_twoEle==true)
      {
	if(elePt[v_idx_ele_final[1]] > ele2ndPt_cut)
	  pass_2ndElePtCut=true;
      }

    //## pT cut 1st muon
    if(v_idx_muon_final.size()>=1)
      {
	if(muonPt[v_idx_muon_final[0]] > muon1stPt_cut)
	  pass_1stMuonPtCut=true;
      }

//      //## eta cut 1st jet
//     if(v_idx_jet_final.size()>=1)
//       {
//  	if( fabs(caloJetIC5Eta[v_idx_jet_final[0]]) < jetEta_cut)
//  	  pass_1stJetEtaCut=true;
//       }
    
//     //## eta cut 2nd jet
//     if(pass_twoJet==true)
//       {
//  	if( fabs(caloJetIC5Eta[v_idx_jet_final[1]]) < jetEta_cut)
//  	  pass_2ndJetEtaCut=true;
//       }
    

    //## M(ele-ele) ##
    TLorentzVector v_ee;

    float Mee = -999.;

    if(pass_twoEle==true)
      {
	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector ele2;
	ele2.SetPtEtaPhiM(elePt[v_idx_ele_final[1]],eleEta[v_idx_ele_final[1]],elePhi[v_idx_ele_final[1]],0);	

	v_ee = ele1 + ele2;

	Mee = v_ee.M();
      }

    //## M(ele-muon) ##
    TLorentzVector v_emu;

    float Memu = -999.;

    if(pass_1Ele1Muon==true)
      {
	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector muon1;
	muon1.SetPtEtaPhiM(muonPt[v_idx_muon_final[0]],muonEta[v_idx_muon_final[0]],muonPhi[v_idx_muon_final[0]],0);	

	v_emu = ele1 + muon1;

	Memu = v_emu.M();
      }


    //## Remove real Zs
    if(pass_twoEle==true && Mee>Mee_highCut)
      pass_Mee=true;

    //simulate the cut above using emu events
    if(pass_1Ele1Muon==true && Memu>Mee_highCut)
      pass_Memu=true;

    //Mee around MZ
    if(pass_twoEle==true && Mee<Mee_highCut && Mee>Mee_lowCut)
      pass_Mee_at_Z=true;

    //## St cut
    float St=-999.;

    if(pass_twoEle==true && pass_twoJet==true)
      {
	St = 
	  elePt[v_idx_ele_final[0]] 
	  + elePt[v_idx_ele_final[1]] 
	  + caloJetIC5Pt[v_idx_jet_final[0]]
	  + caloJetIC5Pt[v_idx_jet_final[1]];
	
	//~~
	
	if(St>St_Cut)
	  pass_St=true;
      }

    //## St cut (mu)
    float St_mu=-999.;

    if(pass_1Ele1Muon==true && pass_twoJet==true)
      {
	St_mu = 
	  elePt[v_idx_ele_final[0]] 
	  + muonPt[v_idx_muon_final[0]] 
	  + caloJetIC5Pt[v_idx_jet_final[0]]
	  + caloJetIC5Pt[v_idx_jet_final[1]];
	
	//~~
	
	if(St_mu>St_Cut)
	  pass_St_mu=true;
      }


    //## M(jet-ele) ##
    TLorentzVector LQ11;
    TLorentzVector LQ12;
    TLorentzVector LQ21;
    TLorentzVector LQ22;

    float bestLQMass1=0.;
    float bestLQMass2=0.;
    float wrongLQMass1=0.;
    float wrongLQMass2=0.;

    if(pass_twoEle==true && pass_twoJet==true)
      { 

	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector ele2;
	ele2.SetPtEtaPhiM(elePt[v_idx_ele_final[1]],eleEta[v_idx_ele_final[1]],elePhi[v_idx_ele_final[1]],0);	

	TLorentzVector jet1;
	jet1.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[0]],caloJetIC5Eta[v_idx_jet_final[0]],caloJetIC5Phi[v_idx_jet_final[0]],0);	
	TLorentzVector jet2;
	jet2.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[1]],caloJetIC5Eta[v_idx_jet_final[1]],caloJetIC5Phi[v_idx_jet_final[1]],0);	

	//comb1	
	LQ11 = ele1 + jet1;
	LQ22 = ele2 + jet2;

	//comb2
	LQ12 = ele1 + jet2;
	LQ21 = ele2 + jet1;

	//Best LQ mass algorithm
	float DeltaM_comb1=fabs(LQ11.M()-LQ22.M());
	float DeltaM_comb2=fabs(LQ21.M()-LQ12.M());

	if(DeltaM_comb1<DeltaM_comb2)
	  {
	    bestLQMass1=LQ11.M();
	    bestLQMass2=LQ22.M();
	    wrongLQMass1=LQ12.M();
	    wrongLQMass2=LQ21.M();
	  }
	else
	  {
	    bestLQMass1=LQ12.M();
	    bestLQMass2=LQ21.M();
	    wrongLQMass1=LQ11.M();
	    wrongLQMass2=LQ22.M();
	  }

      }



    //## M(jet-ele/mu) ##
    TLorentzVector LQ11_mu;
    TLorentzVector LQ12_mu;
    TLorentzVector LQ21_mu;
    TLorentzVector LQ22_mu;

    float bestLQMass1_mu=0.;
    float bestLQMass2_mu=0.;
    float wrongLQMass1_mu=0.;
    float wrongLQMass2_mu=0.;

    if(pass_1Ele1Muon==true && pass_twoJet==true)
      { 

	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector muon1;
	muon1.SetPtEtaPhiM(muonPt[v_idx_muon_final[0]],muonEta[v_idx_muon_final[0]],muonPhi[v_idx_muon_final[0]],0);	

	TLorentzVector jet1;
	jet1.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[0]],caloJetIC5Eta[v_idx_jet_final[0]],caloJetIC5Phi[v_idx_jet_final[0]],0);	
	TLorentzVector jet2;
	jet2.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[1]],caloJetIC5Eta[v_idx_jet_final[1]],caloJetIC5Phi[v_idx_jet_final[1]],0);	

	//comb1	
	LQ11_mu = ele1 + jet1;
	LQ22_mu = muon1 + jet2;

	//comb2
	LQ12_mu = ele1 + jet2;
	LQ21_mu = muon1 + jet1;

	//Best LQ mass algorithm
	float DeltaM_comb1_mu=fabs(LQ11_mu.M()-LQ22_mu.M());
	float DeltaM_comb2_mu=fabs(LQ21_mu.M()-LQ12_mu.M());

	if(DeltaM_comb1_mu<DeltaM_comb2_mu)
	  {
	    bestLQMass1_mu=LQ11_mu.M();
	    bestLQMass2_mu=LQ22_mu.M();
	    wrongLQMass1_mu=LQ12_mu.M();
	    wrongLQMass2_mu=LQ21_mu.M();
	  }
	else
	  {
	    bestLQMass1_mu=LQ12_mu.M();
	    bestLQMass2_mu=LQ21_mu.M();
	    wrongLQMass1_mu=LQ11_mu.M();
	    wrongLQMass2_mu=LQ22_mu.M();
	  }

      }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //## Plots
    //## number of selected events 
    //## efficiencies

    h_pThat_unweight->Fill(pthat);
    h_pThat_weight->Fill(pthat,weight);

    h_nEleID->Fill(v_idx_ele_ID.size(),weight);
    h_nEleFinal_noIso->Fill(v_idx_ele_final_noIso.size(),weight);
    h_nEleFinal->Fill(v_idx_ele_final.size(),weight);

    h_nJetFinal->Fill(v_idx_jet_final.size(),weight);
    //h_nJetFinal_pTcut->Fill(v_idx_jet_final_pTcut.size(),weight);

    //~~~

    //     N_abs_nocut++;
    //     N_Lint_nocut+=weight;
    //     e_N_Lint_nocut+=weight*weight;

    //~~~

    N_abs_skim++;
    N_Lint_skim+=weight;
    e_N_Lint_skim+=weight*weight;

    //~~~

    if(pass_HLT==true)
      {
	N_abs_hlt++;
	N_Lint_hlt+=weight;
	e_N_Lint_hlt+=weight*weight;
      }

    //~~~

    if(pass_HLT==true && pass_twoEle_noIso==true)
      {
	N_abs_2ele_noiso++;
	N_Lint_2ele_noiso+=weight;
	e_N_Lint_2ele_noiso+=weight*weight;
      }      

    //~~~

    if(pass_HLT==true && pass_twoEle==true)
      {
	N_abs_2ele++;
	N_Lint_2ele+=weight;
	e_N_Lint_2ele+=weight*weight;
      }      

    //~~~

    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true)
      {
	N_abs_2ele_2jets++;
	N_Lint_2ele_2jets+=weight;
	e_N_Lint_2ele_2jets+=weight*weight;
      }

    //~~~


    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true && pass_Mee==true)
      {
	
	h_pT1stEle->Fill(elePt[v_idx_ele_final[0]],weight);
	h_pT2ndEle->Fill(elePt[v_idx_ele_final[1]],weight);
	
	h_Eta1stEle->Fill(eleEta[v_idx_ele_final[0]],weight);
	h_Eta2ndEle->Fill(eleEta[v_idx_ele_final[1]],weight);
	
	h_pT1stJet->Fill(caloJetIC5Pt[v_idx_jet_final[0]],weight);
	h_pT2ndJet->Fill(caloJetIC5Pt[v_idx_jet_final[1]],weight);
	
	h_Eta1stJet->Fill(caloJetIC5Eta[v_idx_jet_final[0]],weight);
	h_Eta2ndJet->Fill(caloJetIC5Eta[v_idx_jet_final[1]],weight);

      }


    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true 
       && pass_1stElePtCut==true) 
      { 

	//~~~
	
	N_abs_eleCuts_jetCuts++;
	N_Lint_eleCuts_jetCuts+=weight;
	e_N_Lint_eleCuts_jetCuts+=weight*weight;

	//~~~
	

	h_Mej_allComb->Fill(LQ11.M(),weight);
	h_Mej_allComb->Fill(LQ12.M(),weight);
	h_Mej_allComb->Fill(LQ21.M(),weight);
	h_Mej_allComb->Fill(LQ22.M(),weight);

	h_Mej_best->Fill(bestLQMass1,weight);
	h_Mej_best->Fill(bestLQMass2,weight);

	h_Mej_wrong->Fill(wrongLQMass1,weight);
	h_Mej_wrong->Fill(wrongLQMass2,weight);

	h_2d_Mej_best_vs_wrong->Fill(bestLQMass1,wrongLQMass1);
	h_2d_Mej_best_vs_wrong->Fill(bestLQMass2,wrongLQMass2);

      }


    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true
       && pass_1stElePtCut==true 
       && pass_Mee==true)
      { 
	
	//~~~
	
	N_abs_Mee++;
	N_Lint_Mee+=weight;
	e_N_Lint_Mee+=weight*weight;
	
	//~~~
	
	h_Mej_allComb_Meecut->Fill(LQ11.M(),weight);
	h_Mej_allComb_Meecut->Fill(LQ12.M(),weight);
	h_Mej_allComb_Meecut->Fill(LQ21.M(),weight);
	h_Mej_allComb_Meecut->Fill(LQ22.M(),weight);

	h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ11.M());
	h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ12.M());
	h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ21.M());
	h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ22.M());
	
	h_Mej_best_Meecut->Fill(bestLQMass1,weight);
	h_Mej_best_Meecut->Fill(bestLQMass2,weight);
	
	h_2d_Mej_best_Meecut_vs_St->Fill(St,bestLQMass1);
	h_2d_Mej_best_Meecut_vs_St->Fill(St,bestLQMass2);

	h_St->Fill(St,weight);
	
      }


    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true 
       && pass_1stElePtCut==true 
       && pass_St==true)
      { 

	h_Mee->Fill(Mee,weight);
	
      }


    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true 
       && pass_1stElePtCut==true 
       && pass_Mee==true
       && pass_St==true)
      { 

	//~~~
	
	N_abs_St++;
	N_Lint_St+=weight;
	e_N_Lint_St+=weight*weight;
	
	//~~~
	
	h_Mej_allComb_Meecut_Stcut->Fill(LQ11.M(),weight);
	h_Mej_allComb_Meecut_Stcut->Fill(LQ12.M(),weight);
	h_Mej_allComb_Meecut_Stcut->Fill(LQ21.M(),weight);
	h_Mej_allComb_Meecut_Stcut->Fill(LQ22.M(),weight);

	h_Mej_best_Meecut_Stcut->Fill(bestLQMass1,weight);
	h_Mej_best_Meecut_Stcut->Fill(bestLQMass2,weight);

	h_pThat_sel->Fill(pthat,weight);
	
      }



    if(pass_HLT==true && pass_twoEle==true && pass_twoJet==true
       && pass_1stElePtCut==true  
       && pass_Mee==false
       && pass_Mee_at_Z==true
       && pass_St==true)
      { 

// 	//~~~
	
// 	N_abs_St++;
// 	N_Lint_St+=weight;
// 	e_N_Lint_St+=weight*weight;
	
// 	//~~~

	h_Mej_allComb_MeecutInside_Stcut->Fill(LQ11.M(),weight);
	h_Mej_allComb_MeecutInside_Stcut->Fill(LQ12.M(),weight);
	h_Mej_allComb_MeecutInside_Stcut->Fill(LQ21.M(),weight);
	h_Mej_allComb_MeecutInside_Stcut->Fill(LQ22.M(),weight);

	h_Mej_best_MeecutInside_Stcut->Fill(bestLQMass1,weight);
	h_Mej_best_MeecutInside_Stcut->Fill(bestLQMass2,weight);

	h_pThat_sel_MeecutInside->Fill(pthat,weight);

      }


    if(pass_HLT==true && pass_1Ele1Muon==true && pass_twoJet==true 
       && pass_1stElePtCut==true && pass_1stMuonPtCut==true
       && pass_Memu==true
       && pass_St_mu==true)
      { 

// 	//~~~
	
// 	N_abs_St_mu++;
// 	N_Lint_St_mu+=weight;
// 	e_N_Lint_St_mu+=weight*weight;
	
// 	//~~~
	
	h_Memuj_allComb_Memucut_Stcut->Fill(LQ11_mu.M(),weight);
	h_Memuj_allComb_Memucut_Stcut->Fill(LQ12_mu.M(),weight);
	h_Memuj_allComb_Memucut_Stcut->Fill(LQ21_mu.M(),weight);
	h_Memuj_allComb_Memucut_Stcut->Fill(LQ22_mu.M(),weight);

	h_Memuj_best_Memucut_Stcut->Fill(bestLQMass1_mu,weight);
	h_Memuj_best_Memucut_Stcut->Fill(bestLQMass2_mu,weight);

      }



  }// end loop over events


  // ONLY for LQ (for the moment) 
  // calculate number of events after no cut 
  // using weight, Lint and cross section

  //  - correct if running on all the events of the 
  //    RootNtuple for that sample
  if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
    { 
      //N_abs_nocut=(cross_section*Lint)/weight;
      N_abs_nocut=NevNoCut_;
      e_N_abs_nocut=sqrt(N_abs_nocut);       
      //N_Lint_nocut=cross_section*Lint;
      N_Lint_nocut=N_abs_nocut*weight;
      e_N_Lint_nocut+=(e_N_abs_nocut/N_abs_nocut)*N_Lint_nocut;
    }

  //## errors on number of selected events ##

  //~~~

  //## errors on number of events
  //e_N_abs_nocut=sqrt(N_abs_nocut);       
  //e_N_abs_skim=sqrt(N_abs_skim);        
  e_N_abs_hlt=sqrt(N_abs_hlt);        
  e_N_abs_2ele_noiso=sqrt(N_abs_2ele_noiso);  
  e_N_abs_2ele=sqrt(N_abs_2ele);        
  e_N_abs_2ele_2jets=sqrt(N_abs_2ele_2jets);  
  e_N_abs_eleCuts_jetCuts=sqrt(N_abs_eleCuts_jetCuts);
  e_N_abs_Mee=sqrt(N_abs_Mee);            
  e_N_abs_St=sqrt(N_abs_St);             


  //~~~


  //## error on number of events (in Lint)
  //e_N_Lint_nocut=sqrt(e_N_Lint_nocut);           
  //e_N_Lint_skim=sqrt(e_N_Lint_skim);               
  e_N_Lint_hlt=sqrt(e_N_Lint_hlt);               
  e_N_Lint_2ele_noiso=sqrt(e_N_Lint_2ele_noiso);         
  e_N_Lint_2ele=sqrt(e_N_Lint_2ele);               
  e_N_Lint_2ele_2jets=sqrt(e_N_Lint_2ele_2jets);         
  e_N_Lint_eleCuts_jetCuts=sqrt(e_N_Lint_eleCuts_jetCuts);    
  e_N_Lint_Mee=sqrt(e_N_Lint_Mee);                
  e_N_Lint_St=sqrt(e_N_Lint_St);                 

  //~~~

  //## relative error on number of events (in Lint)
  //e_rel_N_Lint_nocut=e_N_Lint_nocut/N_Lint_nocut;
  //e_rel_N_Lint_skim=e_N_Lint_skim/N_Lint_skim; 
  e_rel_N_Lint_hlt=e_N_Lint_hlt/N_Lint_hlt; 
  e_rel_N_Lint_2ele_noiso=e_N_Lint_2ele_noiso/N_Lint_2ele_noiso;
  e_rel_N_Lint_2ele=e_N_Lint_2ele/N_Lint_2ele;      
  e_rel_N_Lint_2ele_2jets=e_N_Lint_2ele_2jets/N_Lint_2ele_2jets;
  e_rel_N_Lint_eleCuts_jetCuts=e_N_Lint_eleCuts_jetCuts/N_Lint_eleCuts_jetCuts;
  e_rel_N_Lint_Mee=e_N_Lint_Mee/N_Lint_Mee;            
  e_rel_N_Lint_St=e_N_Lint_St/N_Lint_St;             

  //~~~

  //## efficiency
  //eff_N_Lint_nocut=N_Lint_nocut/N_Lint_nocut; 
  //eff_N_Lint_skim=N_Lint_skim/N_Lint_nocut;  

  if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
    { 
      eff_N_Lint_hlt=N_Lint_hlt/N_Lint_nocut;  
    }

  eff_N_Lint_2ele_noiso=N_Lint_2ele_noiso/N_Lint_hlt;
  eff_N_Lint_2ele=N_Lint_2ele/N_Lint_2ele_noiso;      
  eff_N_Lint_2ele_2jets=N_Lint_2ele_2jets/N_Lint_2ele;
  eff_N_Lint_eleCuts_jetCuts=N_Lint_eleCuts_jetCuts/N_Lint_2ele_2jets;
  eff_N_Lint_Mee=N_Lint_Mee/N_Lint_eleCuts_jetCuts;            
  eff_N_Lint_St=N_Lint_St/N_Lint_Mee;   

  if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
    { 
      eff_N_Lint_final=N_Lint_St/N_Lint_nocut;             
    }

  //~~~

  //## error on efficiency
  //e_eff_N_Lint_nocut=eff_N_Lint_nocut * sqrt(e_rel_N_Lint_nocut*e_rel_N_Lint_nocut + e_rel_N_Lint_nocut*e_rel_N_Lint_nocut);

  if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
    { 
      e_eff_N_Lint_hlt = sqrt( eff_N_Lint_hlt*(1 - eff_N_Lint_hlt) / N_abs_nocut);
    }

  //   e_eff_N_Lint_skim = eff_N_Lint_skim*sqrt(e_rel_N_Lint_skim*e_rel_N_Lint_skim 
  // 					 + e_rel_N_Lint_skim*e_rel_N_Lint_skim);

  //   e_eff_N_Lint_hlt = eff_N_Lint_hlt*sqrt(e_rel_N_Lint_hlt*e_rel_N_Lint_hlt  
  // 					 + e_rel_N_Lint_skim*e_rel_N_Lint_skim);

  e_eff_N_Lint_2ele_noiso = eff_N_Lint_2ele_noiso*sqrt(e_rel_N_Lint_2ele_noiso*e_rel_N_Lint_2ele_noiso  
						       + e_rel_N_Lint_hlt*e_rel_N_Lint_hlt);
  
  e_eff_N_Lint_2ele = eff_N_Lint_2ele*sqrt(e_rel_N_Lint_2ele*e_rel_N_Lint_2ele
					   + e_rel_N_Lint_2ele_noiso*e_rel_N_Lint_2ele_noiso);
  
  e_eff_N_Lint_2ele_2jets = eff_N_Lint_2ele_2jets*sqrt(e_rel_N_Lint_2ele_2jets*e_rel_N_Lint_2ele_2jets
						       + e_rel_N_Lint_2ele*e_rel_N_Lint_2ele);
  
  e_eff_N_Lint_eleCuts_jetCuts 
    = eff_N_Lint_eleCuts_jetCuts*sqrt(e_rel_N_Lint_eleCuts_jetCuts*e_rel_N_Lint_eleCuts_jetCuts
				      + e_rel_N_Lint_2ele_2jets*e_rel_N_Lint_2ele_2jets);    
  
  e_eff_N_Lint_Mee = eff_N_Lint_Mee*sqrt(e_rel_N_Lint_Mee*e_rel_N_Lint_Mee
					 + e_rel_N_Lint_eleCuts_jetCuts*e_rel_N_Lint_eleCuts_jetCuts);
  
  e_eff_N_Lint_St = eff_N_Lint_St*sqrt(e_rel_N_Lint_St*e_rel_N_Lint_St
				       + e_rel_N_Lint_Mee*e_rel_N_Lint_Mee); 

  if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
    { 
      e_eff_N_Lint_final = sqrt( eff_N_Lint_final*(1 - eff_N_Lint_final) / N_abs_nocut);
    }  

  //~~~


  //Write histograms
  h_pThat_unweight->Write();
  h_pThat_weight->Write();
  h_pThat_sel->Write();

  h_nEleID->Write();
  h_nEleFinal_noIso->Write();
  h_nEleFinal->Write();

  h_pT1stEle->Write();
  h_pT2ndEle->Write();
  h_Eta1stEle->Write();
  h_Eta2ndEle->Write();

  h_nJetFinal->Write();
  //h_nJetFinal_pTcut->Write();

  h_pT1stJet->Write();
  h_pT2ndJet->Write();
  h_Eta1stJet->Write();
  h_Eta2ndJet->Write();

  h_St->Write();

  h_Mee->Write();
  h_Mej_allComb->Write();
  h_Mej_allComb_Meecut->Write();
  h_Mej_allComb_Meecut_Stcut->Write();
  h_Mej_best->Write();
  h_Mej_best_Meecut->Write();
  h_Mej_best_Meecut_Stcut->Write();
  h_Mej_wrong->Write();
  h_2d_Mej_best_vs_wrong->Write();

  h_2d_Mej_allComb_Meecut_vs_St->Write();
  h_2d_Mej_best_Meecut_vs_St->Write();

  h_Mej_allComb_MeecutInside_Stcut->Write();
  h_Mej_best_MeecutInside_Stcut->Write();
  h_pThat_sel_MeecutInside->Write();

  h_Memuj_allComb_Memucut_Stcut->Write();
  h_Memuj_best_Memucut_Stcut->Write();

  output_root->Close();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  

  //Latex table
  char output_txt_title[200];
  sprintf(output_txt_title,"%s%s",&string(*outputFileName_)[0],"_latex.tex");
  
  ofstream output_txt;  //dichiarazione ofstream
  output_txt.open(&output_txt_title[0]/*,ofstream::app*/);
  
  if(output_txt)
    {   
      output_txt << "\\documentclass[a4paper,12pt]{report}" << endl; 
      output_txt << "\\usepackage[latin1]{inputenc}" << endl; 
      output_txt << "\\usepackage[english]{babel}" << endl; 
      output_txt << "\\usepackage{graphicx}" << endl; 
      output_txt << "\\usepackage[normal]{subfigure}" << endl; 
      output_txt << "\\usepackage{lscape}" << endl; 
      output_txt << "%\\usepackage{axodraw}" << endl; 
      output_txt << "\\RequirePackage{xspace}" << endl; 
      output_txt << "\\begin{document}" << endl; 
      output_txt << "\\begin{table}[htbp]" << endl; 
      output_txt << "\\begin{center}" << endl; 
      output_txt << "\\begin{tabular}{|c|c|c|c|}" << endl; 
      output_txt << "\\hline" << endl; 
      output_txt << "\\hline" << endl; 
      output_txt << " & $N_{ev}$ $100pb^{-1}$ & $N_{ev}$ & $\\varepsilon$ \\\\" << endl; 
      output_txt << "\\hline" << endl; 
      output_txt << "\\hline" << endl; 


      //output_txt << scientific << setprecision(2) << endl;
      output_txt << fixed << setprecision(2) << endl;

      if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
	{ 
	  output_txt <<  "no cut " << "&"  
		     << N_Lint_nocut <<" $\\pm$ "<< e_N_Lint_nocut << "& " 
		     << N_abs_nocut <<" & - \\\\" << endl;
	}      

      //       output_txt << "+ LQ skim " << "&"
      // 		 << N_Lint_skim <<" $\\pm$ "<< e_N_Lint_skim << "& " << N_abs_skim <<" & "
      // 		 << scientific << setprecision(2)  
      // 		 << eff_N_Lint_skim << " $\\pm$ " << e_eff_N_Lint_skim  
      // 		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ hlt (HE or VHE) " << "&"
		 << N_Lint_hlt <<" $\\pm$ "<< e_N_Lint_hlt << "& " << N_abs_hlt <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_hlt << " $\\pm$ " << e_eff_N_Lint_hlt  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ 2 ele (ID) $P_{T} >" << elePt_cut << "$ GeV " << "&" 
		 << N_Lint_2ele_noiso <<" $\\pm$ "<< e_N_Lint_2ele_noiso << "& " << N_abs_2ele_noiso <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_2ele_noiso << " $\\pm$ " << e_eff_N_Lint_2ele_noiso  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ 2 ele (ID+Iso) $P_{T} >" << elePt_cut << "$ GeV " << "&" 
		 << N_Lint_2ele <<" $\\pm$ "<< e_N_Lint_2ele << "& " << N_abs_2ele <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_2ele << " $\\pm$ " << e_eff_N_Lint_2ele  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ 2e, 2j " 
		 << "$P_{T}^{ele}$ ($P_{T}^{jet}$) $>$" 
		 << elePt_cut << "(" << jetPt_cut <<")"<< " GeV " << "&"   
		 << N_Lint_2ele_2jets <<" $\\pm$ "<< e_N_Lint_2ele_2jets << "& " << N_abs_2ele_2jets <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_2ele_2jets << " $\\pm$ " << e_eff_N_Lint_2ele_2jets  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      //       output_txt << "+ ele, jet kinematics cuts " << "&"
      //  		 << N_Lint_eleCuts_jetCuts <<" $\\pm$ "<< e_N_Lint_eleCuts_jetCuts << "& " 
      //  		 << N_abs_eleCuts_jetCuts <<" & "
      //  		 << scientific << setprecision(2)  
      //  		 << eff_N_Lint_eleCuts_jetCuts << " $\\pm$ " << e_eff_N_Lint_eleCuts_jetCuts  
      //  		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ $P_{T}$ $1^{st}$ ele $>$ " << ele1stPt_cut << " GeV " << "&"  
		 << N_Lint_eleCuts_jetCuts <<" $\\pm$ "<< e_N_Lint_eleCuts_jetCuts << "& " 
  		 << N_abs_eleCuts_jetCuts <<" & "
  		 << scientific << setprecision(2)  
  		 << eff_N_Lint_eleCuts_jetCuts << " $\\pm$ " << e_eff_N_Lint_eleCuts_jetCuts  
  		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ $M_{ee} >" << Mee_highCut << "$ GeV" << "&" 
		 << N_Lint_Mee <<" $\\pm$ "<< e_N_Lint_Mee << "& " << N_abs_Mee <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_Mee << " $\\pm$ " << e_eff_N_Lint_Mee  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      output_txt << "+ $S_{T} >" << St_Cut << "$ GeV " << "&" 
		 << N_Lint_St <<" $\\pm$ "<< e_N_Lint_St << "& " << N_abs_St <<" & "
		 << scientific << setprecision(2)  
		 << eff_N_Lint_St << " $\\pm$ " << e_eff_N_Lint_St  
		 << "\\\\" << fixed << setprecision(2) << endl; 

      if( strcmp(eventProcess, "leptoquark")==0 && cross_section>-999.)
	{ 
	  
	  output_txt << "\\hline" << endl;
	  
	  output_txt << scientific << setprecision(2) << endl;
	  
	  output_txt << "full selection efficiency" << "&" 
		     << " & " << " & "
		     << eff_N_Lint_final << " $\\pm$ " << e_eff_N_Lint_final  << "\\\\" << endl;
	}
      
      output_txt << "\\hline" << endl; 
      
      output_txt << "\\end{tabular}" << endl; 
      output_txt << "\\end{center}" << endl; 
      output_txt << "\\caption{" << eventProcess << " sample: the first column lists the selection sequence, $N_{ev}$ $100pb^{-1}$ is the number of selected events in $100pb^{-1}$, $N_{ev}$ is the absolute number of selected events (unweighted for the cross section), $\\varepsilon$ is the relative selection efficiency with respect to the previous cut. The full selection efficiency is shown in the last line.}" << endl; 
      output_txt << "\\end{table}" << endl; 
      output_txt << "\\end{document}" << endl; 
    }
  
  output_txt.close();

}
