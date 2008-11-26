void drawcompare2(char* namehisto1, char* namehisto2,TFile *file1, TFile *file2, TCanvas *canv, bool logY1, bool logY2, bool norm1, bool norm2){

  TPad fPads1("pad1", "", 0.00, 0.25, 0.50, 0.99);   fPads1.Draw();
  TPad fPads2("pad2", "", 0.00, 0.00, 0.50, 0.25);   fPads2.Draw();
  TPad fPads3("pad3", "", 0.50, 0.25, 0.99, 0.99);   fPads3.Draw();
  TPad fPads4("pad4", "", 0.50, 0.00, 0.99, 0.25);   fPads4.Draw();

  bool norm=false;

  fPads1.cd();
  if(logY1) 
    fPads1.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y1;
  ((TH1D*)gDirectory->Get(namehisto1))->Copy(y1); y1.Sumw2();
  y1.SetFillColor(kBlue);
  y1.SetFillStyle(3004);
  y1.Draw("hist");
  double inter1 = y1.Integral();
  file2->cd();
  TH1F y2;
  ((TH1D*)gDirectory->Get(namehisto1))->Copy(y2);   y2.Sumw2();
  double inter2 = y2.Integral();
  if(norm1)
    y2.SetNormFactor(inter1);
  y2.SetMarkerStyle(8);
  y2.SetMarkerSize(.5);
  y2.DrawCopy("pesame");

  cout << "inter 1 fast: " << inter1 << endl;
  cout << "inter 2 full: " << inter2 << endl;
  cout << "y1.Integral() fast: " << y1.Integral() << endl;
  cout << "y2.Integral() full: " << y2.Integral() << endl;
  cout << endl;

  fPads2.cd(); fPads2.SetBottomMargin(.2); fPads2.SetTopMargin(0);
  file1->cd();
  gPad->SetGridx(1);  gPad->SetGridy(1);
  TH1F *hratio = new TH1F(y1); hratio->SetName("hratio"); hratio->Reset();
  if(norm1)
    hratio->Divide(&y2, &y1,inter1,inter2);
  else
    hratio->Divide(&y2, &y1);
  hratio->SetMinimum(0.2); hratio->SetMaximum(1.8);
  hratio->SetMarkerStyle(8);
  hratio->SetMarkerSize(.5);
  hratio->SetNdivisions(504, "Y");
  hratio->SetLabelSize(0.13, "X");  hratio->SetLabelSize(0.17, "Y");
  hratio->SetStats(0);
  hratio->SetTitle();
  hratio->Draw("pe");
//   TLine line(xaxis->GetXmin(),1.,xaxis->GetXmax(),1.);
//   line.SetLineColor(kRed);
//   line.SetLineStyle(1);
//   //  line.SetLineWidth(1.);
//   line.Draw();

  fPads3.cd(); 
  if(logY2) 
    fPads3.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y3;
  ((TH1D*)gDirectory->Get(namehisto2))->Copy(y3); y3.Sumw2();
  y3.SetFillColor(kBlue);
  y3.SetFillStyle(3004);
  y3.Draw("hist");
  inter1 = y3.Integral();
  file2->cd();
  TH1F y4;
  ((TH1D*)gDirectory->Get(namehisto2))->Copy(y4); y4.Sumw2(); 
  inter2 = y4.Integral();
  if(norm2)
    y4.SetNormFactor(inter1);
  y4.SetMarkerStyle(8);
  y4.SetMarkerSize(.5);
  y4.DrawCopy("pesame");


  cout << "inter 3 fast: " << inter1 << endl;
  cout << "inter 4 full: " << inter2 << endl;
  cout << "y3.Integral() fast: " << y3.Integral() << endl;
  cout << "y4.Integral() full: " << y4.Integral() << endl;
  cout << endl;


  fPads4.cd(); fPads4.SetBottomMargin(.2); fPads4.SetTopMargin(0);
  file1->cd();
  gPad->SetGridx(1);  gPad->SetGridy(1);
  TH1F *hratio2 = new TH1F(y3); hratio2->SetName("hratio2"); hratio2->Reset();
  if(norm2)
    hratio2->Divide(&y4, &y3,inter1, inter2);
  else
    hratio2->Divide(&y4, &y3);
  hratio2->SetMinimum(0.2); hratio2->SetMaximum(1.8);
  hratio2->SetMarkerStyle(8);
  hratio2->SetMarkerSize(.5);
  hratio2->SetNdivisions(504, "Y");
  hratio2->SetLabelSize(0.13, "X");  hratio2->SetLabelSize(0.17, "Y");
  hratio2->SetStats(0);
  hratio2->SetTitle();
  hratio2->Draw("pe");

  canv->Print("comparison.ps"); 
  //canv->Print("comparison.gif"); 
}


void comparison(char *inputfile1, char *inputfile2){

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


  c0->Print("comparison.ps["); 

  // rebinnings and reranging
  input1.cd(); 
  ((TH1D*)gDirectory->Get("myeleplots/h_pT_recoEle"))->SetTitle("gsf ele pT (GeV)");
  ((TH1D*)gDirectory->Get("myeleplots/h_pT_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_energy_recoEle"))->SetTitle("gsf ele energy (GeV)");
  ((TH1D*)gDirectory->Get("myeleplots/h_energy_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_eta_recoEle"))->SetTitle("gsf ele \\eta");
  ((TH1D*)gDirectory->Get("myeleplots/h_eta_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle"))->SetTitle("number of gsf ele");
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle"))->SetAxisRange(0,10);
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle_pTcut"))->SetTitle("number of gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle_pTcut"))->SetAxisRange(0,10);
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->SetTitle("gsf ele Ereco/Etrue - \\DeltaR match - pTgen>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->Rebin(6);
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->SetAxisRange(.8,1.1);
  ((TH1D*)gDirectory->Get("myeleplots/h_DeltaR_genEle_recoEle"))->SetTitle("\\DeltaR gen ele - gsf ele");
  ((TH1D*)gDirectory->Get("myeleplots/h_DeltaR_genEle_recoEle"))->Rebin(3);
  ((TH1D*)gDirectory->Get("myeleplots/h_hOverE_recoEle_pTcut"))->SetTitle("H/E - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_hOverE_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->SetTitle("sigmaee - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->SetAxisRange(0.,0.1);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaPhiIn_recoEle_pTcut"))->SetTitle("\\Delta\\phi at Vtx - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaPhiIn_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->SetTitle("\\Delta\\eta at Vtx - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->SetAxisRange(0,0.04);


  input2.cd(); 
  ((TH1D*)gDirectory->Get("myeleplots/h_pT_recoEle"))->SetTitle("gsf ele pT (GeV)");
  ((TH1D*)gDirectory->Get("myeleplots/h_pT_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_energy_recoEle"))->SetTitle("gsf ele energy (GeV)");
  ((TH1D*)gDirectory->Get("myeleplots/h_energy_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_eta_recoEle"))->SetTitle("gsf ele \\eta");
  ((TH1D*)gDirectory->Get("myeleplots/h_eta_recoEle"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle"))->SetTitle("number of gsf ele");
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle"))->SetAxisRange(0,10);
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle_pTcut"))->SetTitle("number of gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_N_recoEle_pTcut"))->SetAxisRange(0,10);
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->SetTitle("gsf ele Ereco/Etrue - \\DeltaR match - pTgen>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->Rebin(6);
  ((TH1D*)gDirectory->Get("myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch"))->SetAxisRange(.8,1.1);
  ((TH1D*)gDirectory->Get("myeleplots/h_DeltaR_genEle_recoEle"))->SetTitle("\\DeltaR gen ele - gsf ele");
  ((TH1D*)gDirectory->Get("myeleplots/h_DeltaR_genEle_recoEle"))->Rebin(3);
  ((TH1D*)gDirectory->Get("myeleplots/h_hOverE_recoEle_pTcut"))->SetTitle("H/E - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_hOverE_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->SetTitle("sigmaee - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->Rebin(2);
  ((TH1D*)gDirectory->Get("myeleplots/h_sigmaee_recoEle_pTcut"))->SetAxisRange(0.,0.1);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaPhiIn_recoEle_pTcut"))->SetTitle("\\Delta\\phi at Vtx - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaPhiIn_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->SetTitle("\\Delta\\eta at Vtx - gsf ele - pTreco>50GeV");
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->Rebin(4);
  ((TH1D*)gDirectory->Get("myeleplots/h_deltaEtaIn_recoEle_pTcut"))->SetAxisRange(0,0.04);

  //drawcompare2("myeleplots/h_pT_recoEle","myeleplots/h_energy_recoEle",&input1 , &input2, c0, true, true, false, false);
  //  drawcompare2("myeleplots/h_eta_recoEle","myeleplots/h_E_recoEle_over_E_genEle_DeltaRmatch",&input1 , &input2, c0, true, false, false, true);
  //  drawcompare2("myeleplots/h_N_recoEle","myeleplots/h_N_recoEle_pTcut",&input1 , &input2, c0, true, true, false, false);
  //  drawcompare2("myeleplots/h_hOverE_recoEle_pTcut","myeleplots/h_sigmaee_recoEle_pTcut",&input1 , &input2, c0, true, true, true, true);
  // drawcompare2("myeleplots/h_deltaPhiIn_recoEle_pTcut","myeleplots/h_deltaEtaIn_recoEle_pTcut",&input1 , &input2, c0, true, true,true, true);
 
  c0->Print("comparison.ps]"); 
  
}
