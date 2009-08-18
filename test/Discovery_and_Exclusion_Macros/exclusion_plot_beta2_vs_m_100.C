#include "CLA.C"
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

Double_t beta2_for_exclusion(const Int_t fNPts, const Double_t fxsTh, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle);

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 
 bool systematics = true;

 // array of signal efficiencies
 Double_t S_eff[5] = {0.36, 0.43, 0.54, 0.59, 0.66};
//  Double_t S_eff[1] = {0.66}; // for testing of individual mass points  <----------------------------------------------
 // array of relative uncertainties on the signal efficiencies
 Double_t Sigma_S_eff[5] = {0.18, 0.15, 0.12, 0.12, 0.10};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
//  Double_t N_bkg_100[1] = {1.27}; // for testing of individual mass points   <----------------------------------------------
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[5] = {0.19, 0.23, 0.33, 0.33, 0.33};
//  Double_t Sigma_N_bkg[1] = {0.46}; // for testing of individual mass points   <----------------------------------------------
 
 // arrays of leptoquark masses
 string mass[5] = {"m250", "m300", "m400", "m500", "m600"};
 Double_t m[5] = {250, 300, 400, 500, 600};
 
 // relative uncertainty on the integrated luminosity (0.1 = 10% uncertainty)
 Double_t Sigma_L = 0.1;
 
 // array of theoretical cross-sections for different leptoquark mass
 Double_t xsTh[5] = {9.64045, 3.6081, 0.691125, 0.176217, 0.0538333};
 
 // arrays of beta^2 ranges that contain 95% CL exclusion of different leptoquark mass hypotheses
 Double_t rangeMin[5] = {0.018, 0.03, 0.09, 0.3,  0.9};
 Double_t rangeMax[5] = { 0.03, 0.08,  0.2, 0.55, 1.5};
 Double_t rangeMin_sys[5] = {0.02, 0.04,  0.1, 0.35, 1.0};
 Double_t rangeMax_sys[5] = {0.05, 0.09, 0.25, 0.6 , 1.6};
//  Double_t rangeMin[1] = {0.8}; // for testing of individual mass points   <----------------------------------------------
//  Double_t rangeMax[1] = {1.4}; // for testing of individual mass points   <----------------------------------------------
//  Double_t rangeMin_sys[1] = {0.9}; // for testing of individual mass points   <----------------------------------------------
//  Double_t rangeMax_sys[1] = {1.5}; // for testing of individual mass points   <----------------------------------------------
 
 // number of points used for interpolation
 Int_t NPts = 5;
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "beta2_vs_m_excl.eps";
  
 // axes labels for the final plot 
 string title = ";m (GeV/c^{2});#beta^{2}";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 650., 100, 0., 1.);
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
 Double_t y_shaded[14] = {0.0267547,0.0380903,0.0531225,0.0736446,0.10221,0.142373,0.194058,0.265091,0.385415,0.520114,0.721552,0.945015,1,1};

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
 
 TGraph *grexcl = new TGraph(14,x_shaded,y_shaded);
 grexcl->SetFillColor(kGray+1);
 grexcl->Draw("f");

 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // array of minimum beta^2 values for 95% CL exclusion
//  Double_t beta2[size];
 Double_t beta2[5] = {0.0246067, 0.0460278, 0.146584, 0.429775, 1.24257};
//  Double_t beta2_sys[size]; // with systematics included
 Double_t beta2_sys[5] = {0.031345, 0.0550194, 0.16532, 0.467803, 1.33682}; // with systematics included
 
//  for(Int_t i = 0; i < size; i++){
//   
//   beta2[i] = beta2_for_exclusion(NPts, xsTh[i], rangeMin[i], rangeMax[i], 0, S_eff[i], 0, N_bkg_100[i], 0, mass[i]);
//   if(systematics) beta2_sys[i] = beta2_for_exclusion(NPts, xsTh[i], rangeMin_sys[i], rangeMax_sys[i], Sigma_L, S_eff[i], Sigma_S_eff[i], N_bkg_100[i], Sigma_N_bkg[i], mass[i]);
//  }

 TGraph *beta2_vs_m = new TGraph(size, m, beta2);
 beta2_vs_m->SetLineWidth(2);
 beta2_vs_m->SetLineStyle(7);
 beta2_vs_m->SetLineColor(kBlue);
 beta2_vs_m->SetMarkerSize(.9);
 beta2_vs_m->SetMarkerStyle(22);
 beta2_vs_m->SetMarkerColor(kBlue); 
 beta2_vs_m->Fit("fit1");
 fit1->SetRange(200,600);

 TGraph *beta2_vs_m_sys = new TGraph(size, m, beta2_sys);
 beta2_vs_m_sys->SetLineWidth(2);
 beta2_vs_m_sys->SetLineColor(kRed);
 beta2_vs_m_sys->SetMarkerSize(.9);
 beta2_vs_m_sys->SetMarkerStyle(21);
 beta2_vs_m_sys->SetMarkerColor(kRed); 
 if(systematics) beta2_vs_m_sys->Fit("fit2");
 if(systematics) fit2->SetRange(200,600);
  
 const Int_t n = 21;
 Double_t x[n], ymin[n], ymin_sys[n], ymax[n];
 for(Int_t i=0;i<n;i++) {
   x[i] = 200+20*i;
   ymin[i] = fit1->Eval(x[i]);
   if(systematics) ymin_sys[i] = fit2->Eval(x[i]); else ymin_sys[i] = fit1->Eval(x[i]);
   ymax[i] = 1;
 }
 TGraph *grshade = new TGraph(2*n);
 TGraph *grshade_sys = new TGraph(2*n);
 for(Int_t i=0;i<n;i++) {
    grshade->SetPoint(i,x[i],ymax[i]);
    grshade->SetPoint(n+i,x[n-i-1],ymin[n-i-1]);
    grshade_sys->SetPoint(i,x[i],ymax[i]);
    grshade_sys->SetPoint(n+i,x[n-i-1],ymin_sys[n-i-1]);
 }
 grshade->SetFillStyle(3005);
 grshade->SetFillColor(kBlue);
 grshade->Draw("f");
 grshade_sys->SetFillStyle(3004);
 grshade_sys->SetFillColor(kRed);
 grshade_sys->Draw("f");
 
 gPad->RedrawAxis();
 
 beta2_vs_m->Draw("P");
 fit1->Draw("same");
 if(systematics) beta2_vs_m_sys->Draw("P");
 if(systematics) fit2->Draw("same");
 
 TLegend *legend = new TLegend(.25,.68,.7,.85);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.2);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(beta2_vs_m_sys,"95% C.L. (with sys. unc.)","lp");
 legend->AddEntry(beta2_vs_m,"95% C.L. (no sys. unc.)","lp");
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
 delete grexcl;
 delete grshade;
 delete grshade_sys;
 delete legend;
 delete beta2_vs_m;
 delete beta2_vs_m_sys;
 delete bg;
 delete c;
}

Double_t beta2_for_exclusion(const Int_t fNPts, const Double_t fxsTh, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle) 
{
 
 Double_t x[fNPts], y[fNPts];

 Double_t step = (fRangeMax - fRangeMin)/(fNPts-1);
   
 for (Int_t i = 0; i < fNPts; i++) {
 
  x[i] = fRangeMin + step*i;
  y[i] = CLA(100, 100*fSigma_L, fS_eff*x[i], fS_eff*x[i]*fSigma_S_eff, fN_bkg_100, fN_bkg_100*fSigma_N_bkg);
 }
 
 TCanvas *c_temp = new TCanvas("c_temp","",1120,800);
 c_temp->cd();

 string title = fTitle + ";#beta^{2};95% C.L. upper limit on #sigma [pb]";

 TH2F *bg_temp = new TH2F("bg_temp",title.c_str(), 100, fRangeMin, fRangeMax, 100, 0.8*y[fNPts-1], 1.2*y[0]);
 bg_temp->SetStats(kFALSE);
 bg_temp->SetTitleOffset(1.,"X");
 bg_temp->SetTitleOffset(1.,"Y");
 bg_temp->Draw();
 
 TF1 *f1 = new TF1("f1","[0]+[1]/pow(x,[2])",fRangeMin,fRangeMax);
 f1->SetParameters(0.,1.,1.);
 f1->SetParLimits(2, 0.45, 1.1);
 
 TGraph *xsection = new TGraph(fNPts, x, y);
 xsection->SetMarkerSize(1.);
 xsection->SetMarkerStyle(24);
 xsection->SetMarkerColor(kRed);
 xsection->Draw("P");
 xsection->Fit("f1");

 Double_t beta2 = f1->GetX(fxsTh);

 cout<<">> beta^2 for 95% CL exclusion of "<<fTitle<<" = "<<beta2<<"\n";
 
 if(fSigma_N_bkg != 0) c_temp->SaveAs((fTitle + "_exclusion_sys.png").c_str());
 else c_temp->SaveAs((fTitle + "_exclusion.png").c_str());
 
 delete xsection;
 delete f1;
 delete bg_temp;
 delete c_temp;
 
 return beta2;
}
