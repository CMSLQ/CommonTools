/* File to compare histograms with same names in two different files.
Author: E. Twedt, Dec. 10, 2008

Usage:
>root -l
root>.L Compare2files.C
root>compare("file1.root","file2.root")
root>.q
>gv compare2files.ps

This macro opens the two files and looks for the histograms named in the drawcompare1 or drawcompare2 method called in the 
main compare method.  drawcompare1 plots the values from the histogram in the first file in blue hashes and the same 
histogram in the second file on the same canvas in black dots.  drawcompare2 does the same but puts two histograms side 
by side on the same canvas.  Both methods can be called multiple times in the main compare method.  Each canvas that is 
created is appended to the end of the ps file. 
Red rectanges are drawn on each canvas with the x min and max values set by b1 and b2 (e1 and e2 for the second 
pad in drawcompare2).
*/


void drawcompare2(char* namehisto1, char* namehisto2, char* namehisto3, char* namehisto4, TFile *file1, TFile *file2,TCanvas *canv, bool logY, bool norm, float b1, float b2,float e1, float e2 ){

  TPad fPads1("pad1", "", 0.00, 0.25, 0.50, 0.99);   fPads1.Draw();
  TPad fPads2("pad2", "", 0.50, 0.25, 0.99, 0.99);   fPads2.Draw();

  fPads1.cd();

  TLegend *legend1 = new TLegend(0.56,0.68,0.86,0.85);
  legend1->SetFillColor(kWhite);

  if(logY) 
    fPads1.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y1;
  ((TH1D*)gDirectory->Get(namehisto1))->Copy(y1); y1.Sumw2();
  y1.SetFillColor(kBlue);
  y1.SetFillStyle(3004);
  y1.Draw("hist");
  legend1->AddEntry("y1","LQ Elec - Barrel","f");
  double inter1 = y1.Integral();
  file2->cd();
  TH1F y2;
  ((TH1D*)gDirectory->Get(namehisto2))->Copy(y2);   y2.Sumw2();
  double inter2 = y2.Integral();
  if(norm)
    y2.SetNormFactor(inter1);
  y2.SetMarkerStyle(8);
  y2.SetMarkerSize(.5);
  y2.DrawCopy("pesame");

  float cut1=b1;
  float cut2=b2;
  float ymin=(y1.GetMinimum()<y2.GetMinimum())?y1.GetMinimum():y2.GetMinimum();
  float ymax=2*(y1.GetMaximum());
  //Draw the cut retangle
   Double_t x[5] = {b1,b2,b2,b1,b1};
   Double_t y[5] = {ymin,ymin,ymax,ymax,ymin};
   TPolyLine *pline = new TPolyLine(5,x,y);
   pline->SetFillStyle(3005);
   pline->SetLineColor(2);
   pline->SetLineWidth(4);
   pline->Draw("f");
   pline->Draw();

  legend1->AddEntry("y2","QCD Elec - Barrel","p");
  legend1->Draw();

  fPads2.cd();

  TLegend *legend2 = new TLegend(0.56,0.68,0.86,0.85);
  legend2->SetFillColor(kWhite);

  if(logY) 
    fPads2.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y3;
  ((TH1D*)gDirectory->Get(namehisto3))->Copy(y3); y3.Sumw2();
  y3.SetFillColor(kBlue);
  y3.SetFillStyle(3004);
  y3.Draw("hist");
  legend2->AddEntry("y3","LQ Elec - Endcap","f");
  double inter3 = y3.Integral();
  file2->cd();
  TH1F y4;
  ((TH1D*)gDirectory->Get(namehisto4))->Copy(y4);   y4.Sumw2();
  double inter4 = y4.Integral();
  if(norm)
    y4.SetNormFactor(inter3);
  y4.SetMarkerStyle(8);
  y4.SetMarkerSize(.5);
  y4.DrawCopy("pesame");

  float cut1=e1;
  float cut2=e2;
  float ymin=(y3.GetMinimum()<y4.GetMinimum())?y3.GetMinimum():y4.GetMinimum();
  float ymax=2*(y3.GetMaximum());
   Double_t x[5] = {e1,e2,e2,e1,e1};
   Double_t y[5] = {ymin,ymin,ymax,ymax,ymin};
   TPolyLine *pline = new TPolyLine(5,x,y);
   pline->SetFillStyle(3005);
   pline->SetLineColor(2);
   pline->SetLineWidth(3);
   pline->Draw("f");
   pline->Draw();

  legend2->AddEntry("y4","QCD Elec - Endcap","p");
  legend2->Draw();

//   cout << "inter 1 fast: " << inter1 << endl;
//   cout << "inter 2 full: " << inter2 << endl;
//   cout << "y1.Integral() fast: " << y1.Integral() << endl;
//   cout << "y2.Integral() full: " << y2.Integral() << endl;
//   cout << endl;


  canv->Print("compare2files.ps"); 
  //canv->Print("comparison.gif"); 
}

void drawcompare1(char* namehisto1, char* namehisto2, TFile *file1, TFile *file2, TCanvas *canv, bool logY, bool norm,float b1, float b2 ){

  TPad fPads1("pad1", "", 0.00, 0.25, 0.80, 0.99);   fPads1.Draw();

  fPads1.cd();

  TLegend *legend1 = new TLegend(0.56,0.68,0.86,0.85);
  legend1->SetFillColor(kWhite);

  if(logY) 
    fPads1.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y1;
  ((TH1D*)gDirectory->Get(namehisto1))->Copy(y1); y1.Sumw2();
  y1.SetFillColor(kBlue);
  y1.SetFillStyle(3004);
  y1.Draw("hist");
  legend1->AddEntry("y1","Nearest Reco Ele","f");
  double inter1 = y1.Integral();
  TH1F y2;
  ((TH1D*)gDirectory->Get(namehisto2))->Copy(y2);   y2.Sumw2();
  double inter2 = y2.Integral();
  if(norm)
    y2.SetNormFactor(inter1);
  y2.SetMarkerStyle(8);
  y2.SetMarkerSize(.5);
  y2.DrawCopy("pesame");
  //Draw the cut retangle
//   float cut1=b1;
//   float cut2=b2;
//   float ymin=(y1.GetMinimum()<y2.GetMinimum())?y1.GetMinimum():y2.GetMinimum();
//   float ymax=2*(y1.GetMaximum());
//    Double_t x[5] = {b1,b2,b2,b1,b1};
//    Double_t y[5] = {ymin,ymin,ymax,ymax,ymin};
//    TPolyLine *pline = new TPolyLine(5,x,y);
//    pline->SetFillStyle(3005);
//    pline->SetLineColor(2);
//    pline->SetLineWidth(4);
//    pline->Draw("f");
//    pline->Draw();


  legend1->AddEntry("y2","2nd Nearest Reco Ele","p");
  legend1->Draw();

  canv->Print("compare2files.ps"); 
  //canv->Print("comparison.gif"); 
}


void comparison(char *inputfile1,char *inputfile2){

  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);  // Show overflow, underflow + SumOfWeights 
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();

  TCanvas *c0 = new TCanvas("c0","--c0--",0,0,1200,900);
  c0->Clear();
  TFile input1(inputfile1);
  TFile input2(inputfile2);

  c0->Print("compare2files.ps["); 

  // rebinnings and reranging
  input1.cd(); 
  ((TH1D*)gDirectory->Get("DeltaR_Gen_Reco"))->SetTitle("Distance btwn Gen Ele and Reco Ele");
  ((TH1D*)gDirectory->Get("eleClassif_barrel_matched"))->SetTitle("Electron Classification Barrel");
  ((TH1D*)gDirectory->Get("eleClassif_endcap_matched"))->SetTitle("Electron Classification Endcap");
  ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_barrel_matched"))->SetTitle("Electron #Delta #eta Trk Barrel");
  ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_endcap_matched"))->SetTitle("Electron #Delta #eta Trk Endcap");
  ((TH1D*)gDirectory->Get("eleDeltaPhiTrkSC_barrel_matched"))->SetTitle("Electron #Delta #phi Trk Barrel");
  ((TH1D*)gDirectory->Get("eleDeltaPhiTrkSC_endcap_matched"))->SetTitle("Electron #Delta #phi Trk Endcap");
  ((TH1D*)gDirectory->Get("eleEcalRecHitIso_barrel_matched"))->SetTitle("Ecal Rec Hit Isolation Barrel");
  ((TH1D*)gDirectory->Get("eleEcalRecHitIso_endcap_matched"))->SetTitle("Ecal Rec Hit Isolation Endcap");
  ((TH1D*)gDirectory->Get("eleHoE_barrel_matched"))->SetTitle("Electron H over E Barrel");
  ((TH1D*)gDirectory->Get("eleHoE_endcap_matched"))->SetTitle("Electron H over E Endcap");
  ((TH1D*)gDirectory->Get("eleNumTrkIso_barrel_matched"))->SetTitle("Electron Track Number Isolation Barrel");
  ((TH1D*)gDirectory->Get("eleNumTrkIso_endcap_matched"))->SetTitle("Electron Track Number Isolation Endcap");
  ((TH1D*)gDirectory->Get("eleSigmaEE_barrel_matched"))->SetTitle("Electron #sigma #eta #eta Barrel");
  ((TH1D*)gDirectory->Get("eleSigmaEE_endcap_matched"))->SetTitle("Electron #sigma #eta #eta Endcap");
  ((TH1D*)gDirectory->Get("eleTrkIso_barrel_matched"))->SetTitle("Electron Track Isolation Barrel");
  ((TH1D*)gDirectory->Get("eleTrkIso_endcap_matched"))->SetTitle("Electron Track Isolation Endcap");

  ((TH1D*)gDirectory->Get("eleClassif_barrel_matched"))->Rebin(2);
  ((TH1D*)gDirectory->Get("eleClassif_endcap_matched"))->Rebin(2);
  ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_endcap_matched"))->Rebin(2);
  ((TH1D*)gDirectory->Get("eleDeltaPhiTrkSC_endcap_matched"))->Rebin(2);
//   ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_barrel_matched"))->SetTitle("Delta #eta Trk Barrel");
//   ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_barrel_matched"))->SetTitle("Delta #eta Trk Barrel");
//   ((TH1D*)gDirectory->Get("eleDeltaEtaTrkSC_barrel_matched"))->SetTitle("Delta #eta Trk Barrel");
////   ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel"))->SetAxisRange(.8,1.1);
//   ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap"))->SetTitle("Zee 1_6_12, gsf ele Ereco/Etrue endcap");
//   ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap"))->Rebin(4);
//   ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap"))->SetAxisRange(.8,1.1);

  ///////// Call the methods here for each histogram.  
  ///////// Note the histo names in each input file have to be the same.

//     drawcompare1("DeltaR_Gen_Reco","DeltaR_Gen_2ndReco",&input1,&input2, c0, true, true);
    drawcompare2("eleClassif_barrel_matched","eleClassif_barrel_unmatched","eleClassif_endcap_matched","eleClassif_endcap_unmatched",&input1,&input2, c0, true, true,40,40,40,40);
    drawcompare2("eleDeltaEtaTrkSC_barrel_matched","eleDeltaEtaTrkSC_barrel_unmatched","eleDeltaEtaTrkSC_endcap_matched","eleDeltaEtaTrkSC_endcap_unmatched",&input1,&input2, c0, true, true,-0.005,0.005,-0.007,0.007);
    drawcompare2("eleDeltaPhiTrkSC_barrel_matched","eleDeltaPhiTrkSC_barrel_unmatched","eleDeltaPhiTrkSC_endcap_matched","eleDeltaPhiTrkSC_endcap_unmatched",&input1,&input2, c0, true, true,-0.09,0.09,-0.09,0.09);
    drawcompare2("eleEcalRecHitIso_barrel_matched","eleEcalRecHitIso_barrel_unmatched","eleEcalRecHitIso_endcap_matched","eleEcalRecHitIso_endcap_unmatched",&input1,&input2, c0, true, true,-.5,6,-.5,6);
    drawcompare2("eleHoE_barrel_matched","eleHoE_barrel_unmatched","eleHoE_endcap_matched","eleHoE_endcap_unmatched",&input1,&input2, c0, true, true,0,0.05,0,0.1);
    drawcompare2("eleNumTrkIso_barrel_matched","eleNumTrkIso_barrel_unmatched","eleNumTrkIso_endcap_matched","eleNumTrkIso_endcap_unmatched",&input1,&input2, c0, true, true,-.5,5,-.5,5);
    drawcompare2("eleSigmaEE_barrel_matched","eleSigmaEE_barrel_unmatched","eleSigmaEE_endcap_matched","eleSigmaEE_endcap_unmatched",&input1,&input2, c0, true, true,0,0.011,0,0.0275);
    drawcompare2("eleTrkIso_barrel_matched","eleTrkIso_barrel_unmatched","eleTrkIso_endcap_matched","eleTrkIso_endcap_unmatched",&input1,&input2, c0, true, true,-.5,7.5,-.5,15);
//     drawcompare2("eleClassif_endcap_matched","eleClassif_endcap_unmatched",&input1, c0, true, true,1.0);
//     drawcompare2("eleDeltaEtaTrkSC_barrel_matched","eleDeltaEtaTrkSC_barrel_unmatched",&input1, c0, true, true,1.0);
//     drawcompare2("eleDeltaEtaTrkSC_barrel_matched","eleDeltaEtaTrkSC_barrel_unmatched",&input1, c0, true, true,1.0);
//     drawcompare2("eleDeltaEtaTrkSC_barrel_matched","eleDeltaEtaTrkSC_barrel_unmatched",&input1, c0, true, true,1.0);

    c0->Print("compare2files.ps]"); 
  
}
