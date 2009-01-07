/*
Makes a nice plot from the output file from elecRecoEff.C
Usage:
Change the outfile to the file you want to run on (really 
the infile)
>root -l plotEleEff.C

*/

{

  TFile outfile("elecRecoEff.root");
  
  TCanvas c1;

  c1->SetGridy();

  int Xmin=-3;
  int Xmax =3;

  eff_Eta->SetMarkerSize(0.5);
  eff_Eta->SetMarkerStyle(20);
  eff_Eta->SetMarkerColor(kBlack);
  eff_Eta->GetXaxis()->SetRangeUser(Xmin,Xmax);
  eff_Eta->Draw("histep");

  eff_Eta->GetYaxis()->SetTitle("efficiency");

  eff_ID_Eta->SetMarkerSize(0.5);
  eff_ID_Eta->SetMarkerStyle(2);
  eff_ID_Eta->SetMarkerColor(kBlue);
  eff_ID_Eta->Draw("samehistep");  

  eff_ID_ISO_Eta->SetMarkerSize(0.5);
  eff_ID_ISO_Eta->SetMarkerStyle(4);
  eff_ID_ISO_Eta->SetMarkerColor(kRed);
  eff_ID_ISO_Eta->Draw("samehistep");  
  TLegend *legend = new TLegend(0.614322,0.615385,0.874372,0.868881);
  TLegend *legend = new TLegend(0.75,0.75,0.874372,0.868881);
  legend->SetFillColor(kWhite);
  legend->AddEntry(eff_Eta,"gsf ele - no cut","p");
  legend->AddEntry(eff_ID_Eta,"gsf ele - ID cuts","p");
  legend->AddEntry(eff_ID_ISO_Eta,"gsf ele - ID+ISO cuts","p");
  legend->Draw();

  TCanvas c2;
  c2->SetGridy();

  eff_Pt->SetMarkerSize(0.5);
  eff_Pt->SetMarkerStyle(20);
  eff_Pt->SetMarkerColor(kBlack);
  eff_Pt->Draw("histep");

  eff_Pt->GetYaxis()->SetTitle("efficiency");

  eff_ID_Pt->SetMarkerSize(0.5);
  eff_ID_Pt->SetMarkerStyle(2);
  eff_ID_Pt->SetMarkerColor(kBlue);
  eff_ID_Pt->Draw("samehistep");  

  eff_ID_ISO_Pt->SetMarkerSize(0.5);
  eff_ID_ISO_Pt->SetMarkerStyle(4);
  eff_ID_ISO_Pt->SetMarkerColor(kRed);
  eff_ID_ISO_Pt->Draw("samehistep");  

  TLegend *legend = new TLegend(0.614322,0.615385,0.874372,0.868881);
  TLegend *legend = new TLegend(0.75,0.75,0.874372,0.868881);
  legend->SetFillColor(kWhite);
  legend->AddEntry(eff_Pt,"gsf ele - no cut","p");
  legend->AddEntry(eff_ID_Pt,"gsf ele - ID cuts","p");
  legend->AddEntry(eff_ID_ISO_Pt,"gsf ele - ID+ISO cuts","p");
  legend->Draw();



  //regions out of fiducial region

//   Int_t n = 5;
//   Double_t x[5] = {-3,-2,-2,-3,-3};
//   Double_t y[5] = {0,0,1,1,0};
//   TPolyLine * pl = new TPolyLine(n,x,y,"F");
//   pl->SetFillStyle(3005);   
//   pl->SetFillColor(kRed);   
//   pl->Draw();

//   Double_t x1[5] = {-1.6,-1.4,-1.4,-1.6,-1.6};
//   Double_t y1[5] = {0,0,1,1,0};
//   TPolyLine * pl1 = new TPolyLine(n,x1,y1,"F");
//   pl1->SetFillStyle(3005);   
//   pl1->SetFillColor(kRed);   
//   pl1->Draw();

//   Double_t x2[5] = {1.4,1.6,1.6,1.4,1.4};
//   Double_t y2[5] = {0,0,1,1,0};
//   TPolyLine * pl2 = new TPolyLine(n,x2,y2,"F");
//   pl2->SetFillStyle(3005);   
//   pl2->SetFillColor(kRed);   
//   pl2->Draw();

//   Double_t x3[5] = {2,3,3,2,2};
//   Double_t y3[5] = {0,0,1,1,0};
//   TPolyLine * pl3 = new TPolyLine(n,x3,y3,"F");
//   pl3->SetFillStyle(3005);   
//   pl3->SetFillColor(kRed);   
//   pl3->Draw();





  
}


