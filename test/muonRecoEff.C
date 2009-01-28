/*
Creates new histograms that are the ratio of two histograms named in the main efficiency() method which then calls the PlotEfficiency method.
Author: Adapted from efficiency.C written by F. Santanastasio

Usage: 
>root -l
root> .L muonRecoEff.C
root> efficiency()
root> .q

Should produce new root file with efficiency histograms.
*/

void PlotEfficiency(char* inputfile, char* namehisto1, char* namehisto2, char* plotName,char* xAxisTitle, float rebin=1, bool varBin, char* outputfile)
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

  histo1->Rebin(rebin);
  histo2->Rebin(rebin);

  if (varBin){
    Double_t xbin[51]={0,10,20,30,40,50,60,70,80,90,100,
    		       110,120,130,140,150,160,170,180,190,200,
		       210,220,230,240,250,260,270,280,290,300,
		       320,340,360,380,400,420,440,460,480,500,
		       550,600,650,700,750,800,850,900,950,1000};
    histo1->Rebin(50,"histo1_new",xbin);
    histo1=histo1_new;
    histo2->Rebin(50,"histo2_new",xbin);
    histo2=histo2_new;
  }

  histo1->Divide(histo2);
  histo1->SetStats(0); 

  for(int bin=1;bin<=histo1->GetNbinsX();bin++)
    {

      //cout << bin << endl;
      float eff = histo1->GetBinContent(bin);
      float one_minus_eff = 1- histo1->GetBinContent(bin);
      float Ntot =  float(histo2->GetBinContent(bin));

      if(Ntot>0)
	{
	  float err_eff = sqrt( (eff * one_minus_eff) / Ntot);
	  histo1->SetBinError(bin,err_eff);
	}
    }

  histo1->SetLineColor(kBlack);

  histo1->SetMarkerSize(1);
  histo1->SetMarkerStyle(20);
  histo1->SetMarkerColor(kBlack);

  histo1->GetYaxis()->SetRangeUser(0,1);
  histo1->GetYaxis()->SetTitle("efficiency");
  histo1->GetXaxis()->SetTitle(xAxisTitle);

  histo1->SetTitle("");
  histo1->SetName(plotName);

  histo1->Draw("histep");

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

  histo1->Write();
  histo2->Write();

  outfile.Close();

  infile.Close();

}


void efficiency()
{

  char* outputfile="muonRecoEff.root";

  TFile outfile(outputfile,"RECREATE");
  outfile.Close();


  PlotEfficiency("muons.root", 
		 "muon_Eta_Gen_matched", 
		 "muon_Eta_Gen",
		 "eff_Eta",
                 "\\eta",1, false,
		 outputfile);


  PlotEfficiency("muons.root", 
		 "muon_Pt_Gen_matched", 
		 "muon_Pt_Gen",
		 "eff_Pt",
                 "pT",1, true,
		 outputfile);



}
