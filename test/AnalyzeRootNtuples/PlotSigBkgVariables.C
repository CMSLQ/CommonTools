PlotSigBkgVariable(char *histoname_, char* outputfilename_, char* xtitle_, char* ytitle_, float Ymin_=0.1, float Ymax_=1000, float Xmin_=0., float Xmax_=1000., int rebin_, int logY_){
  
  gROOT->Reset();
  gStyle->SetOptStat(0);

  //Which histogram you want to plot?
  char histoname[200];
  sprintf(histoname,"%s",&histoname_[0]);

  //Which sig/bkg files you want to consider?
  //TFile L250("lq1stgen_M250_fast169_100pb.root");
  TFile L250("lq1stgen_M250_full167_100pb.root");
  TFile L400("lq1stgen_M400_full167_100pb.root");
  TFile L650("lq1stgen_M650_full167_100pb.root");
  TFile L1000("lq1stgen_M1000_full167_100pb.root");

  TFile Z("zjet.root");
  TFile W("wjet.root");
  TFile ttbar("ttbar.root");
  TFile QCD("qcdjet.root");
  TFile gj("gammajet.root");
  TFile ww("ww.root");
  TFile wz("wz.root");
  TFile zz("zz.root");

  TLegend *legend = new TLegend(0.56,0.62,0.86,0.85);
  legend->SetFillColor(kWhite);

  int rebin=rebin_;

  TCanvas c1;
  if(logY_==1)
    c1.SetLogy();

  THStack h_BgkStack;
  TH1F allBkg;

  //~~~~~~~~~~~~~~~~~~~~~~
  Z.cd();
  TH1F h1;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h1); 
  h1.GetXaxis()->SetTitle(xtitle_);
  h1.GetYaxis()->SetTitle(ytitle_);

  h1.SetMinimum(Ymin_);

  if(Ymax_>0)
    {
      h1.SetMaximum(Ymax_);
      h1.GetYaxis()->SetRangeUser(Ymin_,Ymax_);
    }

  h1.SetLineColor(1);
  h1.SetLineWidth(2);
  h1.SetFillColor(1);
  h1.SetFillStyle(3003);
  h1.Rebin(rebin);
  h1.SetName("Z/\\gamma+jet");
  h1.DrawClone("HISTE");



  //~~~~~~~~~~~~~~~~~~~~~~
  W.cd();
  TH1F h2;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h2); 
  h2.SetLineColor(2);
  h2.SetLineWidth(2);
  h2.SetFillColor(2);
  h2.SetFillStyle(3001);
  h2.Rebin(rebin);
  h2.SetName("W+jet");
  h2.DrawClone("HISTEsame");




  //~~~~~~~~~~~~~~~~~~~~~~
  ttbar.cd();
  TH1F h3;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h3); 
  h3.SetLineColor(kRed);
  h3.SetLineWidth(2);
  h3.SetFillColor(kRed);
  h3.SetFillStyle(3004);
  h3.SetName("ttbar");
  //h3.Scale(1.8);
  h3.Rebin(rebin);
  h3.DrawClone("HISTEsame");



  //~~~~~~~~~~~~~~~~~~~~~~
  QCD.cd();
  TH1F h4;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h4); 
  h4.SetLineColor(6);
  h4.SetLineWidth(2);
  h4.SetFillColor(6);
  h4.SetFillStyle(3004);
  h4.Rebin(rebin);
  h4.SetName("QCDjets");
  h4.DrawClone("HISTEsame");




  //~~~~~~~~~~~~~~~~~~~~~~
  gj.cd();
  TH1F h5;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h5); 
  h5.SetLineColor(1);
  h5.SetLineWidth(2);
  h5.SetFillColor(5);
  h5.SetFillStyle(3001);
  h5.Rebin(rebin);
  h5.SetName("\\gamma+jet");
  h5.DrawClone("HISTEsame");


  //~~~~~~~~~~~~~~~~~~~~~~
  ww.cd();
  TH1F h1_1;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h1_1); 
  h1_1.SetLineColor(2);
  h1_1.SetLineWidth(2);
  h1_1.SetFillColor(2);
  //h1_1.SetFillStyle(3003);
  h1_1.Rebin(rebin);
  h1_1.SetName("WW");
  h1_1.DrawClone("HISTEsame");


  //~~~~~~~~~~~~~~~~~~~~~~
  wz.cd();
  TH1F h1_2;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h1_2); 
  h1_2.SetLineColor(2);
  h1_2.SetLineWidth(2);
  h1_2.SetFillColor(2);
  h1_2.SetFillStyle(3004);
  h1_2.Rebin(rebin);
  h1_2.SetName("WZ");
  h1_2.DrawClone("HISTEsame");


  //~~~~~~~~~~~~~~~~~~~~~~
  zz.cd();
  TH1F h1_3;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h1_3); 
  h1_3.SetLineColor(1);
  h1_3.SetLineWidth(2);
  h1_3.SetFillColor(1);
  //h1_3.SetFillStyle(3003);
  h1_3.Rebin(rebin);
  h1_3.SetName("ZZ");
  h1_3.DrawClone("HISTEsame");


  //~~~~~~~~~~~~~~~~~~~~~~
  // Others (sum of Wjet,WW,WZ,ZZ,QCD,gammaJet)
  //~~~~~~~~~~~~~~~~~~~~~~

  int NbinsX=h1.GetNbinsX();
  double Xmin=h1.GetXaxis()->GetXmin();
  double Xmax=h1.GetXaxis()->GetXmax();

  TH1F *h_others = new TH1F("h_others","Others",NbinsX,Xmin,Xmax);
  h_others->Add(&h2);
  h_others->Add(&h4);
  h_others->Add(&h5);
  h_others->Add(&h1_1);
  h_others->Add(&h1_2);
  h_others->Add(&h1_3);

  h_others->SetLineColor(1);
  h_others->SetLineWidth(2);
  h_others->SetFillColor(1);
  h_others->SetFillStyle(3001);
  //h_others->Rebin(rebin);
  h_others->SetName("Others");
  h_others->DrawClone("HISTEsame");


  // ~~~~~~~~~~~~~~~~~~~~~~
  //      Draw Stack
  // ~~~~~~~~~~~~~~~~~~~~~~
  //  h_BgkStack.Add(&h4);
  //  h_BgkStack.Add(&h5);
  //   h_BgkStack.Add(&h1_1);
  //   h_BgkStack.Add(&h1_2);
  //   h_BgkStack.Add(&h1_3);
  //  h_BgkStack.Add(&h2);
  h_BgkStack.Add(h_others);
  h_BgkStack.Add(&h1);
  h_BgkStack.Add(&h3);

  //----------------
  h_BgkStack.Draw("HISTE");
  //----------------

  h_BgkStack.GetXaxis()->SetTitle(xtitle_);
  h_BgkStack.GetXaxis()->SetRangeUser(Xmin_,Xmax_);
  h_BgkStack.GetYaxis()->SetTitle(ytitle_);

  h_BgkStack.SetMinimum(Ymin_);

  if(Ymax_>0)
    {
      h_BgkStack.SetMaximum(Ymax_);
      h_BgkStack.GetYaxis()->SetRangeUser(Ymin_,Ymax_);
    }

  //~~~~~~~~~~~~~~~~~~~~~~
  L250.cd();
  TH1F h6;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h6); 
  h6.SetLineColor(4);
  h6.SetLineWidth(2);
  h6.SetMarkerStyle(20);
  h6.SetMarkerColor(4);
  h6.Rebin(rebin);
  h6.SetName("LQ (250 GeV)");
  //h6.DrawClone("HISTEsame");
  //----------------
  h6.DrawClone("Epsame");
  //----------------


  //~~~~~~~~~~~~~~~~~~~~~~
//   L400.cd();
//   TH1F h7;
//   ((TH1D*)gDirectory->Get(histoname))->Copy(h7); 
//   h7.SetLineColor(4);
//   h7.SetLineWidth(2);
//   h7.SetFillStyle(3003);
//   h7.SetFillColor(4);
//   h7.Rebin(rebin);
//   h7.SetName("LQ (400 GeV)");
//   h7.DrawClone("HISTEsame");

//   legend->AddEntry(&h7,"LQ (400 GeV)","f");

  //~~~~~~~~~~~~~~~~~~~~~~
  L650.cd();
  TH1F h8;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h8); 
  h8.SetLineColor(4);
  //   h8.SetFillColor(4);
  //   h8.SetFillStyle(3004);
  h8.SetLineWidth(2);
  h8.SetMarkerStyle(25);
  h8.SetMarkerColor(4);
  h8.Rebin(rebin);
  h8.SetName("LQ (650 GeV)");
  //h8.DrawClone("HISTEsame");
  h8.DrawClone("Epsame");

  //----------------
  legend->AddEntry(&h6,"LQ (250 GeV)","p");
  legend->AddEntry(&h8,"LQ (650 GeV)","p");
  
  legend->AddEntry(&h3,"ttbar","f");
  legend->AddEntry(&h1,"Z+jet","f");
  legend->AddEntry(h_others,"Others","f");
  //----------------

//   legend->AddEntry(&h2,"W+jet","f");

//   legend->AddEntry(&h1_1,"WW","f");
//   legend->AddEntry(&h1_2,"WZ","f");
//   legend->AddEntry(&h1_3,"ZZ","f");

//   legend->AddEntry(&h4,"QCDjets","f");
//   legend->AddEntry(&h5,"\\gamma+jet","f");

  
  //legend->SetMargin(0.5);
  legend->Draw();

  //~~~~~~~~~~~~~~~~~~~~~~
//   L1000.cd();
//   TH1F h9;
//   ((TH1D*)gDirectory->Get(histoname))->Copy(h9); 
//   h9.SetLineColor(4);
//   h9.SetFillStyle(3004);
//   h9.SetFillColor(4);
//   h9.SetLineWidth(3);
//   h9.Rebin(rebin);
//   h9.SetName("LQ (1000 GeV)");
//   h9.DrawClone("HISTEsame");
  
//   legend->AddEntry(&h9,"LQ (1000 GeV)","f");
  
//   legend->SetMargin(0.5);
//   legend->Draw();

  // Redraw axis
  gPad->RedrawAxis();
  
  
  c1.SaveAs(outputfilename_);


}
