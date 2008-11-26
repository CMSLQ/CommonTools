void PlotEfficiency(char* inputfile, char* namehisto1, char* namehisto2, char* plotName,char* xAxisTitle, float rebin=1, char* outputfile)
{
  gROOT->Reset();

  //**********************************************************************

  gROOT->SetStyle("Plain");
  

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);  // Show overflow, underflow + SumOfWeights 
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  //gStyle->SetErrorX(0);
  
  gStyle->SetTitleBorderSize(0); // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();

  TFile infile(inputfile);

  infile.cd();

  TCanvas c1;
  c1.Update();
  c1.Clear();

  TH1F *histo1 = ((TH1F*)gDirectory->Get(namehisto1)); //histo1->Sumw2();

  TH1F *histo2 = ((TH1F*)gDirectory->Get(namehisto2)); //histo2->Sumw2();

  TH1F *histo_eff = ((TH1F*)gDirectory->Get(namehisto1)); //histo_eff->Sumw2();

  histo1->Rebin(rebin);
  histo2->Rebin(rebin);

  histo_eff->Divide(histo1,histo2,1,1);
  histo_eff->SetStats(0); 

  for(int bin=1;bin<=histo_eff->GetNbinsX();bin++)
    {

      //cout << bin << endl;
      float eff = histo_eff->GetBinContent(bin);
      float one_minus_eff = 1- histo_eff->GetBinContent(bin);
      float Ntot =  float(histo2->GetBinContent(bin));

      if(Ntot>0)
	{
	  float err_eff = sqrt( (eff * one_minus_eff) / Ntot);
	  histo_eff->SetBinError(bin,err_eff);
	}
    }

  histo_eff->SetLineColor(kBlack);

  histo_eff->SetMarkerSize(1);
  histo_eff->SetMarkerStyle(20);
  histo_eff->SetMarkerColor(kBlack);

  histo_eff->GetYaxis()->SetRangeUser(0,1);
  histo_eff->GetYaxis()->SetTitle("efficiency");
  histo_eff->GetXaxis()->SetTitle(xAxisTitle);

  histo_eff->SetTitle("");
  histo_eff->SetName(plotName);

  histo_eff->Draw("histep");

  //total efficiency
  float eff_tot=float(histo1->GetEntries())/float(histo2->GetEntries());
  float Ntot = float(histo2->GetEntries());
  float err_eff_tot = sqrt( (eff_tot * (1 - eff_tot) ) / Ntot);

  cout << "******************************" << endl;
  cout << plotName << endl;
  cout << "eff +\- err_eff: " << eff_tot << " +/- " << err_eff_tot <<endl;
  cout << "******************************" << endl;

  TFile outfile(outputfile,"UPDATE");

  outfile.cd();

  histo_eff.Write();

  outfile.Close();

  infile.Close();

}


void efficiency()
{

  char* outputfile="efficiency.root";

  TFile outfile(outputfile,"RECREATE");
  outfile.Close();

//   PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
// 		 "myeleEffplots/h_pt_matched_Ele_IDcut", 
// 		 "myeleEffplots/h_pt_genEle", 
// 		 "eff_pt_M250_100pb_IDcut",
//                  "pT (GeV)",
// 		 outputfile);

//   PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
// 		 "myeleEffplots/h_pt_matched_Ele", 
// 		 "myeleEffplots/h_pt_genEle",
// 		 "eff_pt_M250_100pb",
//                  "pT (GeV)", 
// 		 outputfile);


  // Acceptance

  cout << "********** Acceptance eta *********" << endl;

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_genEle_FidVol", 
		 "myeleEffplots/h_eta_genEle",
		 "accept_eta_M250",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_genEle_FidVol", 
		 "myeleEffplots/h_eta_genEle",
		 "accept_eta_M400",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_genEle_FidVol", 
		 "myeleEffplots/h_eta_genEle",
		 "accept_eta_M650",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_genEle_FidVol", 
		 "myeleEffplots/h_eta_genEle",
		 "accept_eta_M1000",
                 "\\eta",1, 
		 outputfile);

  cout << "*******************************" << endl << endl;



  cout << "********** Reco efficiency eta in ECAL fiducial region *********" << endl;


  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_M250_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_M400_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_M650_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_M1000_100pb_fast",
                 "\\eta",1, 
		 outputfile);


  cout << "************************************************************" << endl << endl;


  cout << "********** Reco efficiency eta + ID cut in ECAL fiducial region *********" << endl;


  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_IDcut_M250_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_IDcut_M400_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_IDcut_M650_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle_FidVol",
		 "eff_eta_reco_FidVol_IDcut_M1000_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  cout << "*********************************************************************" << endl << endl;



  cout << "********** Acc x Reco efficiency eta + ID cut in ECAL fiducial region *********" << endl;


  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_X_accept_eta_reco_FidVol_IDcut_M250_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_X_accept_eta_reco_FidVol_IDcut_M400_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_X_accept_eta_reco_FidVol_IDcut_M650_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_FidVol_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_X_accept_eta_reco_FidVol_IDcut_M1000_100pb_fast",
                 "\\eta",1, 
		 outputfile);


  cout << "***************************************************************************" << endl << endl;


  cout << "********** Overall reco efficiency eta (no fid. region) *********" << endl;


  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_M250_100pb_fast",
                 "\\eta",1,  
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_M400_100pb_fast",
                 "\\eta",1,  
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_M650_100pb_fast",
                 "\\eta",1,  
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_M1000_100pb_fast",
                 "\\eta",1,  
		 outputfile);



  cout << "********** Overall reco efficiency eta + ID cut (no fid. region) *********" << endl;


  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_250_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M250_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_400_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M400_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M650_100pb_fast",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_1000_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M1000_100pb_fast",
                 "\\eta",1, 
		 outputfile);


  cout << "********** Overall reco efficiency pT/eta (no fid.region) - Fast vs Full *********" << endl;

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_pt_matched_Ele_IDcut", 
		 "myeleEffplots/h_pt_genEle",
		 "eff_pt_reco_IDcut_M650_100pb_fast",
                 "pT (GeV)",1, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fullSim167_650_100pbScenario_2500.root", 
		 "myeleEffplots/h_pt_matched_Ele_IDcut", 
		 "myeleEffplots/h_pt_genEle",
		 "eff_pt_reco_IDcut_M650_100pb_full",
                 "pT (GeV)",1, 
		 outputfile);

 PlotEfficiency("RecoElectrons_ElectronEfficiency_fastSim169_650_100pbScenario_10k.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M650_100pb_fast",
		"\\eta",2, 
		 outputfile);

  PlotEfficiency("RecoElectrons_ElectronEfficiency_fullSim167_650_100pbScenario_2500.root", 
		 "myeleEffplots/h_eta_matched_Ele_IDcut", 
		 "myeleEffplots/h_eta_genEle",
		 "eff_eta_reco_IDcut_M650_100pb_full",
                 "\\eta",2, 
		 outputfile);

  cout << "******************************************************************************" << endl;


}
