#include "CLA.C"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPolyLine.h"
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
 gStyle->SetOptFit(1);
 gStyle->SetStatFont(42);
 gStyle->SetPadLeftMargin(0.13);
 gStyle->SetPadRightMargin(0.07);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetStatY(.9);
}

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 
 bool systematics = true;

 // array of signal efficiencies
 Double_t S_eff[5] = {0.36, 0.43, 0.54, 0.59, 0.66};
//  Double_t S_eff[10] = {0.36, 0.43, 0.54, 0.59, 0.66, 0.67, 0.68, 0.7, 0.7, 0.7};
  // array of relative uncertainties on the signal efficiencies
 Double_t Sigma_S_eff[5] = {0.18, 0.15, 0.12, 0.12, 0.10};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[5] = {0.19, 0.23, 0.33, 0.33, 0.33};
 
 // arrays of leptoquark masses
 Double_t m[10] = {250, 300, 400, 500, 600, 650, 700, 800, 900, 1000};

 // relative uncertainty on the integrated luminosity (0.1 = 10% uncertainty)
 Double_t Sigma_L = 0.1;
 
 // array of theoretical cross-sections for different leptoquark masses
 Double_t xsTh[10] = {10.1, 3.8, 0.75, 0.197, 0.0617, 0.0362, 0.0218, 0.0085, 0.00351, 0.00153};
 
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "xs95CL_vs_m.eps";
  
 // axes labels for the final plot  
 string title = ";m (GeV/c^{2});#beta^{2}#times#sigma (pb)";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 650., 100, 0.01, 50.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {200,256,256,200,200};
 Double_t y_shaded[5] = {0.01,0.01,50,50,0.01};

 // PDF uncertainty band
 Double_t x_pdf[12] = {250,300,400,500,600,650,650,600,500,400,300,250};
 Double_t y_pdf[12] = {9.64045,3.6081,0.691125,0.176217,0.0538333,0.0312225,0.0424445,0.071356,0.221527,0.819375,4.0147,10.5293};

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

 TGraph *grshade = new TGraph(12,x_pdf,y_pdf);
 grshade->SetFillColor(kYellow);
 grshade->Draw("f");

 TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"F");
//  pl->SetFillStyle(3001);   
 pl->SetFillColor(kGray+1);   
 pl->Draw();

 gPad->RedrawAxis();
 
 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // array of 95% CL upper limits on the cross section
//  Double_t xsUp[size];
 Double_t xsUp[5] = {0.23722, 0.166074, 0.10131, 0.0748393, 0.0669023};
//  Double_t xsUp_sys[size]; // with systematics included
 Double_t xsUp_sys[5] = {0.302162, 0.198514, 0.114258, 0.0816143, 0.0718672}; // with systematics included

//  for(Int_t i = 0; i < size; i++){
//   
//   xsUp[i] = CLA(100, 0, S_eff[i], 0, N_bkg_100[i], 0);
//   if(systematics) xsUp_sys[i] = CLA(100, 100*Sigma_L, S_eff[i], S_eff[i]*Sigma_S_eff[i], N_bkg_100[i], N_bkg_100[i]*Sigma_N_bkg[i]);
//  }

 TGraph *xsUp_vs_m = new TGraph(size, m, xsUp);
 xsUp_vs_m->SetLineWidth(2);
 xsUp_vs_m->SetLineStyle(7);
 xsUp_vs_m->SetLineColor(kBlue);
 xsUp_vs_m->SetMarkerSize(.9);
 xsUp_vs_m->SetMarkerStyle(22);
 xsUp_vs_m->SetMarkerColor(kBlue);
 xsUp_vs_m->Draw("CP");

 TGraph *xsUp_vs_m_sys = new TGraph(size, m, xsUp_sys);
 xsUp_vs_m_sys->SetLineWidth(2);
 xsUp_vs_m_sys->SetLineColor(kRed);
 xsUp_vs_m_sys->SetMarkerSize(.9);
 xsUp_vs_m_sys->SetMarkerStyle(21);
 xsUp_vs_m_sys->SetMarkerColor(kRed);
 if(systematics) xsUp_vs_m_sys->Draw("CP");

 TGraph *xsTh_vs_m = new TGraph(size+1, m, xsTh);
 xsTh_vs_m->SetLineWidth(2);
 xsTh_vs_m->SetLineColor(kGreen+2);
 xsTh_vs_m->SetFillColor(kYellow);
 xsTh_vs_m->SetMarkerSize(1.);
 xsTh_vs_m->SetMarkerStyle(22);
 xsTh_vs_m->SetMarkerColor(kGreen+2);
 xsTh_vs_m->Draw("C");

 TLegend *legend = new TLegend(.3,.69,.92,.86);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.15);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(xsTh_vs_m,"#beta^{2}#times#sigma_{theory} with PDF unc. band, #beta=1","lf");
 if(systematics) legend->AddEntry(xsUp_vs_m_sys,"95% C.L. upper limit (with sys. unc.)","lp");
 legend->AddEntry(xsUp_vs_m,"95% C.L. upper limit (no sys. unc.)","lp");
 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextSize(0.04);
 l1.SetTextFont(62);
 l1.SetNDC();
 l1.DrawLatex(0.25,0.24,"CMS Preliminary");

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextSize(0.04);
 l2.SetTextFont(62);
 l2.SetNDC();
 l2.DrawLatex(0.25,0.15,"#intLdt=100 pb^{-1}");

 c->SetGridx();
 c->SetGridy();
//  c->SaveAs(fileName.c_str());

 string name, extension;
 size_t pos = fileName.find(".");

 name = fileName.substr(0,pos);
 extension = fileName.substr(pos);

 c->SetLogy();
 c->SaveAs((name + "_log" + extension).c_str());

 delete pl;
 delete xsTh_vs_m;
 delete xsUp_vs_m;
 delete xsUp_vs_m_sys;
 delete bg;
 delete c;
}
