#include "scp.C"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"

using namespace std;

void style()
{
 gStyle->Reset("Default");
 gStyle->SetCanvasColor(0);
 gStyle->SetPadColor(0);
 gStyle->SetTitleFillColor(10);
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetStatColor(0);
 gStyle->SetPadBorderMode(0);
 gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
 gStyle->SetFrameBorderMode(0);
 gStyle->SetPalette(1);
   
   //gStyle->SetOptStat(kFALSE);
 gStyle->SetOptStat(111110);
 gStyle->SetOptFit(0);
 gStyle->SetStatFont(42);
 gStyle->SetPadLeftMargin(0.13);
 gStyle->SetPadRightMargin(0.07);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetStatY(.9);
}

Double_t beta_for_5sigma(const Int_t fNPts, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg_100, const string& fTitle);

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 
 bool systematics = true;

 // array of N_signal for 100 pb^-1
 Double_t N_sig_100[5] = {359.39, 163.37, 40.41, 11.56, 4.04};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[5] = {0.19, 0.23, 0.33, 0.33, 0.33};
 
 // arrays of leptoquark masses
 string mass[5] = {"m250", "m300", "m400", "m500", "m600"};
 Double_t m[5] = {250, 300, 400, 500, 600};
 
 // arrays of beta^2 ranges that contain 5 sigma significance for different leptoquark mass
 Double_t rangeMin[5] = {0.03,  0.05,  0.15,  0.4, 1.4};
 Double_t rangeMax[5] = {0.07, 0.125,  0.38, 1.05, 2.9};
 Double_t rangeMin_sys[5] = {0.04, 0.07,  0.2, 0.5, 1.6};
 Double_t rangeMax_sys[5] = {0.08, 0.16, 0.48, 1.3, 3.6};
 
 // number of points used for interpolation
 Int_t NPts = 10;
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "beta_vs_m.eps";
  
 // axes labels for the final plot 
 string title = ";m (GeV/c^{2});#beta";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 550., 100, 0., 1.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 TF1 *fit1 = new TF1("fit1","[0]*exp([1]*x)",200,600);
 fit1->SetLineWidth(2);
 fit1->SetLineStyle(7);
 fit1->SetLineColor(kBlue);

 TF1 *fit2 = new TF1("fit2","[0]*exp([1]*x)",200,600);
 fit2->SetLineWidth(2);
 fit2->SetLineColor(kRed);
 
 // region excluded by Tevatron limits
 Double_t x_shaded[14] = {200,205,210,215,220,225,230,235,240,245,250,255,256,200};
 Double_t y_shaded[14] = {sqrt(0.0267547),sqrt(0.0380903),sqrt(0.0531225),sqrt(0.0736446),sqrt(0.10221),sqrt(0.142373),sqrt(0.194058),sqrt(0.265091),sqrt(0.385415),sqrt(0.520114),sqrt(0.721552),sqrt(0.945015),1,1};

 // **********************************************
 // *  Don't change anything below this point!   *
 // **********************************************
 
  // turn on/off batch mode
 gROOT->SetBatch(kTRUE);
 // set ROOT style
 style();
 
 TCanvas *c = new TCanvas("c","",800,800);
 c->cd();

 bg->Draw();
 
 TGraph *grshade = new TGraph(14,x_shaded,y_shaded);
 grshade->SetFillColor(kGray+1);
 grshade->Draw("f");
 
 gPad->RedrawAxis();

 Int_t size = sizeof(N_sig_100)/sizeof(*N_sig_100);

 // array of minimum beta^2 values for 5 sigma significance
 Double_t beta[size];
 Double_t beta_sys[size]; // with systematics included
 
 for(Int_t i = 0; i < size; i++){
  
  beta[i] = beta_for_5sigma(NPts, rangeMin[i], rangeMax[i], N_sig_100[i], N_bkg_100[i], 0, mass[i]);
  if(systematics) beta_sys[i] = beta_for_5sigma(NPts, rangeMin_sys[i], rangeMax_sys[i], N_sig_100[i], N_bkg_100[i], Sigma_N_bkg[i], mass[i]);
 }

 TGraph *beta_vs_m = new TGraph(size, m, beta);
 TGraph *beta_vs_m_fit = new TGraph(size, m, beta);
 beta_vs_m->SetLineWidth(2);
 beta_vs_m->SetLineStyle(7);
 beta_vs_m->SetLineColor(kBlue);
 beta_vs_m->SetMarkerSize(.9);
 beta_vs_m->SetMarkerStyle(22);
 beta_vs_m->SetMarkerColor(kBlue);
 beta_vs_m->Draw("P"); 
 beta_vs_m_fit->Fit("fit1");
 fit1->SetRange(200,600);
 fit1->Draw("same");

 TGraph *beta_vs_m_sys = new TGraph(size, m, beta_sys);
 TGraph *beta_vs_m_sys_fit = new TGraph(size, m, beta_sys);
 beta_vs_m_sys->SetLineWidth(2);
 beta_vs_m_sys->SetLineColor(kRed);
 beta_vs_m_sys->SetMarkerSize(.9);
 beta_vs_m_sys->SetMarkerStyle(21);
 beta_vs_m_sys->SetMarkerColor(kRed);
 if(systematics) beta_vs_m_sys->Draw("P"); 
 if(systematics) beta_vs_m_sys_fit->Fit("fit2");
 if(systematics) {fit2->SetRange(200,600); fit2->Draw("same");}

 TLegend *legend = new TLegend(.26,.72,.7,.85);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.2);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(beta_vs_m_sys,"5#sigma_{S_{cP}} (with sys. unc.)","lp");
 legend->AddEntry(beta_vs_m,"5#sigma_{S_{cP}} (no sys. unc.)","lp");
 if(systematics) legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextSize(0.04);
 l1.SetTextFont(62);
 l1.SetNDC();
 l1.DrawLatex(0.6,0.24,"CMS Preliminary");

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextSize(0.04);
 l2.SetTextFont(62);
 l2.SetNDC();
 l2.DrawLatex(0.6,0.15,"#intLdt=100 pb^{-1}");

 c->SetGridx();
 c->SetGridy();
 c->SaveAs(fileName.c_str());

//  string name, extension;
//  size_t pos = fileName.find(".");
// 
//  name = fileName.substr(0,pos);
//  extension = fileName.substr(pos);
// 
//  c->SetLogy();
//  c->SaveAs((name + "_log" + extension).c_str());
 
 delete fit1;
 delete fit2;
 delete grshade;
 delete legend;
 delete beta_vs_m;
 delete beta_vs_m_fit;
 delete beta_vs_m_sys;
 delete beta_vs_m_sys_fit;
 delete bg;
 delete c;
}

Double_t ScP(const Double_t beta2, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg) 
{
 
 Double_t N_sig_100 = beta2*fN_sig_100;
 
 cout<<">> =====================================\n";
 cout<<">> For beta^2 = "<<beta2<<"\n";
 cout<<">> =====================================\n";
 cout<<">> N_sig_100 = "<<N_sig_100<<"\n";
 
 return scp(N_sig_100, fN_bkg_100, fN_bkg_100*fSigma_N_bkg, 0);
}

Double_t beta_for_5sigma(const Int_t fNPts, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle) 
{
 
 Double_t x[fNPts], y[fNPts];

 Double_t step = (fRangeMax - fRangeMin)/(fNPts-1);
   
 for (Int_t i = 0; i < fNPts; i++) {
 
  x[i] = fRangeMin + step*i;
  y[i] = ScP(x[i], fN_sig_100, fN_bkg_100, fSigma_N_bkg);
 }
 
 TCanvas *c_temp = new TCanvas("c_temp","",1120,800);
 c_temp->cd();

 string title = fTitle + ";#beta^{2};S_{cP}";

 TH2F *bg_temp = new TH2F("bg_temp",title.c_str(), 100, fRangeMin, fRangeMax, 100, 0.8*y[0], 1.2*y[fNPts-1]);
 bg_temp->SetStats(kFALSE);
 bg_temp->SetTitleOffset(1.,"X");
 bg_temp->SetTitleOffset(1.,"Y");
 bg_temp->Draw();
 
 TGraph *scP = new TGraph(fNPts, x, y);
 scP->SetMarkerSize(1.);
 scP->SetMarkerStyle(24);
 scP->SetMarkerColor(kRed);
 scP->Draw("P"); 
 scP->Fit("pol2");

 TF1 *fit = (TF1*)scP->GetFunction("pol2"); 
 Double_t beta2 = fit->GetX(5);
 Double_t N_sig = beta2*fN_sig_100;
 Double_t N_s_b = N_sig + fN_bkg_100;
 
 cout<<">> beta for 5 sigma discovery for "<<fTitle<<" = "<<sqrt(beta2)<<"\n";
 cout<<">> ** N_sig = "<<N_sig<<"\n";
 cout<<">> ** N_bkg = "<<fN_bkg_100<<"\n";
 cout<<">> ** N_s_b = "<<N_s_b<<"\n";
 
 if(fSigma_N_bkg != 0) c_temp->SaveAs((fTitle + "_significance_beta_sys.png").c_str());
 else c_temp->SaveAs((fTitle + "_significance_beta.png").c_str());

 delete scP;
 delete bg_temp;
 delete c_temp;
 
 return sqrt(beta2);
}
