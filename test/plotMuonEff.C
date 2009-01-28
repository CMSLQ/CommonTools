/*
Makes a nice plot from the output file from elecRecoEff.C
Usage:
Change the outfile to the file you want to run on (really 
the infile)
>root -l plotEleEff.C

*/

{

  TFile outfile("muonRecoEff.root");
  
  TCanvas c1;
  c1->Print("MuonEffPlots.ps[");
  c1->SetGridy();

  int Xmin=-3;
  int Xmax =3;

  eff_Eta->SetMarkerSize(0.5);
  eff_Eta->SetMarkerStyle(20);
  eff_Eta->SetMarkerColor(kBlack);
  eff_Eta->GetXaxis()->SetRangeUser(Xmin,Xmax);
  eff_Eta->Draw("histep");
  eff_Eta->GetYaxis()->SetTitle("efficiency");

  TLegend *legend = new TLegend(0.4,0.4,0.624372,0.618881);
  legend->SetFillColor(kWhite);
  legend->AddEntry(eff_Eta,"muons - no cut","p");
  legend->Draw();

  c1->Update();
  c1->Print("MuonEffPlots.ps");

  TCanvas c2;
  c2->SetGridy();
  Double_t xbin[11]={0,10,20,50,100,200,300,450,600,800,900};

  eff_Pt->SetMarkerSize(0.5);
  eff_Pt->SetMarkerStyle(20);
  eff_Pt->SetMarkerColor(kBlack);
  eff_Pt->Draw("histep");
  eff_Pt->GetYaxis()->SetTitle("efficiency");

  TLegend *legend = new TLegend(0.4,0.4,0.624372,0.618881);
  legend->SetFillColor(kWhite);
  legend->AddEntry(eff_Pt,"muon - no cut","p");
  legend->Draw();

  c2->Update();

  //// output to ps file /////

  c2->Print("MuonEffPlots.ps");
  c2->Print("MuonEffPlots.ps]");
  
}


