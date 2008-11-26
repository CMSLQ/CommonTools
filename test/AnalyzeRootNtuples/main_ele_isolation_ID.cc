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
const int Nparam=4;   //--> NUMBER OF PARAMETERS 
//==================

float ECAL_barrel_limit = 1.479;

float ECAL_FR_1_cut = 1.4;
float ECAL_FR_2_cut = 1.6;
float ECAL_FR_3_cut = 2;

//     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
//     PSet robustEleIDCuts =
//     {
//         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
//         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}

float eleHoE_bar_cut=0.115; 
float eleSigmaEE_bar_cut=0.0140;
float eleDeltaPhiTrkSC_bar_cut=0.053;
float eleDeltaEtaTrkSC_bar_cut=0.0090;

float eleHoE_end_cut=0.150; 
float eleSigmaEE_end_cut=0.0275;
float eleDeltaPhiTrkSC_end_cut=0.092;
float eleDeltaEtaTrkSC_end_cut=0.0105;

int eleNumTrkIso_cut=5;
float eleTrkIso_cut=5.;
float eleEcalIso_cut=5.;

float elePt_cut=150.;

float ConeSizeJetCleaning_cut=0.5;
float pTJet_loose_cut=30.;

float ele1stPt_cut=85.;
float ele2ndPt_cut=20.;

float Mee_lowCut=80.;
float Mee_highCut=100.;

float St_Cut=400.;

int electron_PID=11;
int LQ_PID=42;
float ConeSizeMCmatch_cut=0.02;

int main(int argc, char* argv[])
{

  //=============== Check number of input parameters 
  if(argc!=Nparam+1) 
    
    {
      cout << "int main() >> ERROR : arcg = " << argc 
	   << " different from "              << Nparam+1 << endl;
      
      cout << "--- Correct Usage : exe eventProcess inputlist treename outputfilename" << endl;
      cout << "--- Correct Usage : eventProcess can be: all, ttbarjet, wjet, zjet, qcdjet, gammajet, leptoquark" << endl;
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
 
	  Selection analysis(chain,OutputFileName,eventProcess);

	  //analysis.SetCuts(TrkIsoCut,HcalIsoCut,LoosePtCut,PT1Cut,PT2Cut,Eta1Cut,Eta2Cut,METCut,Ext_weight,Rescale_weight,isSoup);
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

  //----------------------------------------------------------------------


  //h_DeltaR_ele_elegen
  TH1F *h_DeltaR_ele_elegen = new TH1F ("h_DeltaR_ele_elegen","",1000,0.,8.);
  h_DeltaR_ele_elegen->Sumw2();
  

  //** matchLQ **
 
  //h_Ereco_over_Etrue_matchLQ
  TH1F *h_Ereco_over_Etrue_matchLQ = new TH1F ("h_Ereco_over_Etrue_matchLQ","",100,0.8,1.2);
  h_Ereco_over_Etrue_matchLQ->Sumw2();
  
  //h_eleHoE_matchLQ
  TH1F *h_eleHoE_matchLQ = new TH1F ("h_eleHoE_matchLQ","",100,-0.02,0.2);
  h_eleHoE_matchLQ->Sumw2();
  
  //h_eleSigmaEE_matchLQ
  TH1F *h_eleSigmaEE_matchLQ = new TH1F ("h_eleSigmaEE_matchLQ","",100,0.,0.1);
  h_eleSigmaEE_matchLQ->Sumw2();

  //h_eleDeltaPhiTrkSC_matchLQ
  TH1F *h_eleDeltaPhiTrkSC_matchLQ = new TH1F ("h_eleDeltaPhiTrkSC_matchLQ","",100,-0.15,0.15);
  h_eleDeltaPhiTrkSC_matchLQ->Sumw2();

  //h_eleDeltaEtaTrkSC_matchLQ
  TH1F *h_eleDeltaEtaTrkSC_matchLQ = new TH1F ("h_eleDeltaEtaTrkSC_matchLQ","",100,-0.15,0.15);
  h_eleDeltaEtaTrkSC_matchLQ->Sumw2();

  //h_eleNumTrkIso_matchLQ
  TH1F *h_eleNumTrkIso_matchLQ = new TH1F ("h_eleNumTrkIso_matchLQ","",11,-0.5,10.5);
  h_eleNumTrkIso_matchLQ->Sumw2();

  //h_eleTrkIso_matchLQ
  TH1F *h_eleTrkIso_matchLQ = new TH1F ("h_eleTrkIso_matchLQ","",100,0.,1.);
  h_eleTrkIso_matchLQ->Sumw2();

  //h_eleEcalIso_matchLQ
  TH1F *h_eleEcalIso_matchLQ = new TH1F ("h_eleEcalIso_matchLQ","",100,0.,1.);
  h_eleEcalIso_matchLQ->Sumw2();

  //h_eleHcalIso_matchLQ
  TH1F *h_eleHcalIso_matchLQ = new TH1F ("h_eleHcalIso_matchLQ","",100,0.,1.);
  h_eleHcalIso_matchLQ->Sumw2();

  //h_eleTrkIsoAbs_matchLQ
  TH1F *h_eleTrkIsoAbs_matchLQ = new TH1F ("h_eleTrkIsoAbs_matchLQ","",100,0.,40.);
  h_eleTrkIsoAbs_matchLQ->Sumw2();

  //h_eleEcalIsoAbs_matchLQ
  TH1F *h_eleEcalIsoAbs_matchLQ = new TH1F ("h_eleEcalIsoAbs_matchLQ","",100,0.,40.);
  h_eleEcalIsoAbs_matchLQ->Sumw2();

  //h_eleHcalIsoAbs_matchLQ
  TH1F *h_eleHcalIsoAbs_matchLQ = new TH1F ("h_eleHcalIsoAbs_matchLQ","",100,0.,40.);
  h_eleHcalIsoAbs_matchLQ->Sumw2();


  //** NOmatchLQ **

  //h_eleHoE_NOmatchLQ
  TH1F *h_eleHoE_NOmatchLQ = new TH1F ("h_eleHoE_NOmatchLQ","",100,-0.02,0.2);
  h_eleHoE_NOmatchLQ->Sumw2();
  
  //h_eleSigmaEE_NOmatchLQ
  TH1F *h_eleSigmaEE_NOmatchLQ = new TH1F ("h_eleSigmaEE_NOmatchLQ","",100,0.,0.1);
  h_eleSigmaEE_NOmatchLQ->Sumw2();

  //h_eleDeltaPhiTrkSC_NOmatchLQ
  TH1F *h_eleDeltaPhiTrkSC_NOmatchLQ = new TH1F ("h_eleDeltaPhiTrkSC_NOmatchLQ","",100,-0.15,0.15);
  h_eleDeltaPhiTrkSC_NOmatchLQ->Sumw2();

  //h_eleDeltaEtaTrkSC_NOmatchLQ
  TH1F *h_eleDeltaEtaTrkSC_NOmatchLQ = new TH1F ("h_eleDeltaEtaTrkSC_NOmatchLQ","",100,-0.15,0.15);
  h_eleDeltaEtaTrkSC_NOmatchLQ->Sumw2();

  //h_eleNumTrkIso_NOmatchLQ
  TH1F *h_eleNumTrkIso_NOmatchLQ = new TH1F ("h_eleNumTrkIso_NOmatchLQ","",11,-0.5,10.5);
  h_eleNumTrkIso_NOmatchLQ->Sumw2();

  //h_eleTrkIso_NOmatchLQ
  TH1F *h_eleTrkIso_NOmatchLQ = new TH1F ("h_eleTrkIso_NOmatchLQ","",100,0.,1.);
  h_eleTrkIso_NOmatchLQ->Sumw2();

  //h_eleEcalIso_NOmatchLQ
  TH1F *h_eleEcalIso_NOmatchLQ = new TH1F ("h_eleEcalIso_NOmatchLQ","",100,0.,1.);
  h_eleEcalIso_NOmatchLQ->Sumw2();

  //h_eleHcalIso_NOmatchLQ
  TH1F *h_eleHcalIso_NOmatchLQ = new TH1F ("h_eleHcalIso_NOmatchLQ","",100,0.,1.);
  h_eleHcalIso_NOmatchLQ->Sumw2();

  //h_eleTrkIsoAbs_NOmatchLQ
  TH1F *h_eleTrkIsoAbs_NOmatchLQ = new TH1F ("h_eleTrkIsoAbs_NOmatchLQ","",100,0.,40.);
  h_eleTrkIsoAbs_NOmatchLQ->Sumw2();

  //h_eleEcalIsoAbs_NOmatchLQ
  TH1F *h_eleEcalIsoAbs_NOmatchLQ = new TH1F ("h_eleEcalIsoAbs_NOmatchLQ","",100,0.,40.);
  h_eleEcalIsoAbs_NOmatchLQ->Sumw2();

  //h_eleHcalIsoAbs_NOmatchLQ
  TH1F *h_eleHcalIsoAbs_NOmatchLQ = new TH1F ("h_eleHcalIsoAbs_NOmatchLQ","",100,0.,40.);
  h_eleHcalIsoAbs_NOmatchLQ->Sumw2();


  //----------------------------------------------------------------------


  // *** Inizialize variables ***

  //********** Loop over events *********
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  int myThousCounter=0;
  bool stdLoop=true;

  cout << "Analyzing process : " << eventProcess << " ......." << endl;
      
  //for (Long64_t jentry=0; jentry<30000;jentry++) {
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
	if(processID == -999 && ALPGENprocessID == -999)
	  passEventProcessSelected=true;
      }
    
    //skip event if not the desired process
    if(passEventProcessSelected==false)
      continue;

    // Skip signal events in gumbo (top secret)
    if (processID == 102 || processID == 123 || processID == 124 || processID == 141) 
      continue;

    // HLT selection
    bool pass_HLT=false;
    if(HLTResults[40]==true || HLTResults[41]==true) 
      pass_HLT=true;
    
    if(pass_HLT==false)
      continue;
      
    // -------------- Candidates -------------------


    float eleTrkIsoAbs[eleCount];
    float eleEcalIsoAbs[eleCount];
    float eleHcalIsoAbs[eleCount];
    
    //### Loop over electrons
    for(int iele=0;iele<eleCount;iele++)
      {

	bool pass_ECAL_FR = false;
	bool pass_elePt = false;
	bool EleIsMatched = false;

	TVector3 ele;
	ele.SetPtEtaPhi(elePt[iele],
			eleEta[iele],
			elePhi[iele]);	
	
	//ECAL fiducial region
	if(fabs(eleEta[iele]) < ECAL_FR_1_cut 
           || 
	   (fabs(eleEta[iele]) > ECAL_FR_2_cut 
	    && fabs(eleEta[iele]) < ECAL_FR_3_cut)
	   )
	  pass_ECAL_FR=true;
	
	//ele pT cut
	if(elePt[iele]>elePt_cut)
	  pass_elePt=true;

	//skip electrons out of fiducial region
	if(pass_ECAL_FR==false)
	  continue;

	//skip electrons with low pT
	if(pass_elePt==false)
	  continue;

	//Isolation with absolute variables
	float sinTheta=1/cosh(eleEta[iele]);
	eleTrkIsoAbs[iele]=eleTrkIso[iele]*elePt[iele];
	eleEcalIsoAbs[iele]=eleEcalIso[iele]*elePt[iele];
	eleHcalIsoAbs[iele]=eleHoE[iele]*eleEnergy[iele]*sinTheta;

	//MC Matching
	float DeltaR_ele_elegen = 9999.;

	for(int igen=0;igen<GenParticleCount;igen++)
	  {

	    //select electrons from LQ decay
	    if(GenParticlePdgId[igen]==abs(electron_PID)
	       && GenParticlePdgId[GenParticleMotherIndex[igen]]==abs(LQ_PID))
	      {
		
		TVector3 elegen;
		elegen.SetPtEtaPhi(GenParticlePt[igen],
				   GenParticleEta[igen],
				   GenParticlePhi[igen]);	
		
		float DeltaR_ele_elegen = elegen.DeltaR(ele);

		h_DeltaR_ele_elegen->Fill(DeltaR_ele_elegen,weight);
		
		if(DeltaR_ele_elegen<ConeSizeMCmatch_cut)
		  {

		    EleIsMatched = true;

		    h_Ereco_over_Etrue_matchLQ->Fill(eleEnergy[iele]/GenParticleE[igen]);

		    h_eleHoE_matchLQ->Fill(eleHoE[iele],weight);
		    h_eleSigmaEE_matchLQ->Fill(eleSigmaEE[iele],weight);
		    h_eleDeltaPhiTrkSC_matchLQ->Fill(eleDeltaPhiTrkSC[iele],weight);
		    h_eleDeltaEtaTrkSC_matchLQ->Fill(eleDeltaEtaTrkSC[iele],weight);

		    h_eleNumTrkIso_matchLQ->Fill(eleNumTrkIso[iele],weight);
		    h_eleTrkIso_matchLQ->Fill(eleTrkIso[iele],weight);
		    h_eleEcalIso_matchLQ->Fill(eleEcalIso[iele],weight);

		    h_eleTrkIsoAbs_matchLQ->Fill(eleTrkIsoAbs[iele],weight);
		    h_eleEcalIsoAbs_matchLQ->Fill(eleEcalIsoAbs[iele],weight);
		    h_eleHcalIsoAbs_matchLQ->Fill(eleHcalIsoAbs[iele],weight);

		  }// end electrons matched with genele from LQ decay

	      }// end select genele from LQ decay

	    //exit from gen particle loop if electron has been matched
	    // 	    if(EleIsMatched == true)
	    // 	      break;
	  }

	if(EleIsMatched == false)
	  {
	    h_eleHoE_NOmatchLQ->Fill(eleHoE[iele],weight);
	    h_eleSigmaEE_NOmatchLQ->Fill(eleSigmaEE[iele],weight);
	    h_eleDeltaPhiTrkSC_NOmatchLQ->Fill(eleDeltaPhiTrkSC[iele],weight);
	    h_eleDeltaEtaTrkSC_NOmatchLQ->Fill(eleDeltaEtaTrkSC[iele],weight);
	    
	    h_eleNumTrkIso_NOmatchLQ->Fill(eleNumTrkIso[iele],weight);
	    h_eleTrkIso_NOmatchLQ->Fill(eleTrkIso[iele],weight);
	    h_eleEcalIso_NOmatchLQ->Fill(eleEcalIso[iele],weight);

	    h_eleTrkIsoAbs_NOmatchLQ->Fill(eleTrkIsoAbs[iele],weight);
	    h_eleEcalIsoAbs_NOmatchLQ->Fill(eleEcalIsoAbs[iele],weight);
	    h_eleHcalIsoAbs_NOmatchLQ->Fill(eleHcalIsoAbs[iele],weight);

	  }
	
      }// loop over electrons


  }// end loop over events


  //Write histograms

  h_DeltaR_ele_elegen->Write();  

  h_Ereco_over_Etrue_matchLQ->Write();
  
  h_eleHoE_matchLQ->Write();
  h_eleSigmaEE_matchLQ->Write();
  h_eleDeltaPhiTrkSC_matchLQ->Write();
  h_eleDeltaEtaTrkSC_matchLQ->Write();
  
  h_eleNumTrkIso_matchLQ->Write();
  h_eleTrkIso_matchLQ->Write();
  h_eleEcalIso_matchLQ->Write();

  h_eleTrkIsoAbs_matchLQ->Write();
  h_eleEcalIsoAbs_matchLQ->Write();
  h_eleHcalIsoAbs_matchLQ->Write();
  
  h_eleHoE_NOmatchLQ->Write();
  h_eleSigmaEE_NOmatchLQ->Write();
  h_eleDeltaPhiTrkSC_NOmatchLQ->Write();
  h_eleDeltaEtaTrkSC_NOmatchLQ->Write();
  
  h_eleNumTrkIso_NOmatchLQ->Write();
  h_eleTrkIso_NOmatchLQ->Write();
  h_eleEcalIso_NOmatchLQ->Write();

  h_eleTrkIsoAbs_NOmatchLQ->Write();
  h_eleEcalIsoAbs_NOmatchLQ->Write();
  h_eleHcalIsoAbs_NOmatchLQ->Write();

}
