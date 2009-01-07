/*
Creates new histograms that are the ratio of two histograms named in the main efficiency() method which then calls the PlotEfficiency method.
Author: Adapted from efficiency.C written by F. Santanastasio

Usage: 
>root -l
root> .L eleRecoEff.C
root> efficiency()
root> .q

Should produce new root file with efficiency histograms.
*/

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

  histo_eff->Write();

  outfile.Close();

  infile.Close();

}


void efficiency()
{

  char* outputfile="elecRecoEff.root";

  TFile outfile(outputfile,"RECREATE");
  outfile.Close();


  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Eta_Gen_matched", 
		 "ele_Eta_Gen",
		 "eff_Eta",
                 "\\eta",1, 
		 outputfile);


  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Eta_Gen_matched_ID", 
		 "ele_Eta_Gen",
		 "eff_ID_Eta",
                 "\\eta",1, 
		 outputfile);


  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Eta_Gen_matched_ID_ISO", 
		 "ele_Eta_Gen",
		 "eff_ID_ISO_Eta",
                 "\\eta",1, 
		 outputfile);

  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Pt_Gen_matched", 
		 "ele_Pt_Gen",
		 "eff_Pt",
                 "pT",1, 
		 outputfile);


  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Pt_Gen_matched_ID", 
		 "ele_Pt_Gen",
		 "eff_ID_Pt",
                 "pT",1, 
		 outputfile);


  PlotEfficiency("HEEPv1_2cuts.root", 
		 "ele_Pt_Gen_matched_ID_ISO", 
		 "ele_Pt_Gen",
		 "eff_ID_ISO_Pt",
                 "pT",1, 
		 outputfile);

}
