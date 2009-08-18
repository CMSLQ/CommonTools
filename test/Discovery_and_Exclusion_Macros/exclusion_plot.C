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

Double_t L_for_exclusion(const Int_t fNPts, const Double_t xs, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fSig_eff, const Double_t fSig_eff_sigma, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg_100, const string& fTitle);

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
//  Double_t S_eff[1] = {0.71}; // for testing of individual mass points  <----------------------------------------------
 // array of relative uncertainties on the signal efficiencies
 Double_t Sigma_S_eff[5] = {0.18, 0.15, 0.12, 0.12, 0.10};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[10] = {9.94, 5.13, 2.28, 1.10, 1.10, 1.10, 1.10, 1.10, 1.10, 1.10};
//  Double_t N_bkg_100[1] = {1.10}; // for testing of individual mass points   <----------------------------------------------
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[10] = {0.45, 0.56, 0.47, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57};
 
 // arrays of leptoquark masses
 string mass[10] = {"m250", "m300", "m400", "m500", "m600", "m650", "m700", "m800", "m900", "m1000"};
 Double_t m[10] = {250, 300, 400, 500, 600, 650, 700, 800, 900, 1000};

 // relative uncertainty on the integrated luminosity (0.1 = 10% uncertainty)
 Double_t Sigma_L = 0.1;
 
 // array of theoretical cross-sections for different leptoquark mass
 Double_t xsTh[10] = {10.1, 3.8, 0.75, 0.197, 0.0617, 0.0362, 0.0218, 0.0085, 0.00351, 0.00153};
//  Double_t xsTh[1] = {0.00153}; // for testing of individual mass points   <----------------------------------------------
 
 // arrays of integrated luminosity ranges (in pb^-1) that contain 95% CL exclusion of different leptoquark mass hypotheses
 Double_t rangeMin[10] = {0.5, 1.2,  5., 20.,  75., 150., 300., 1500.,  6000., 30000.};
 Double_t rangeMax[10] = {2.5, 3.5, 12., 40., 130., 280., 580., 2500., 14000., 60000.};
 Double_t rangeMin_sys[7] = {0.5, 1.2, 5.5, 20.,  90., 160.,  400.};
 Double_t rangeMax_sys[7] = {2.5, 3.5, 13., 42., 150., 300., 1200.};
//  Double_t rangeMin[1] = {20000}; // for testing of individual mass points   <----------------------------------------------
//  Double_t rangeMax[1] = {50000}; // for testing of individual mass points   <----------------------------------------------
 
 // number of points used for interpolation when obtaining 95% CL upper limit on the cross section at a given leptoquark mass
 Int_t NPts = 5;
 
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "L95CL_vs_m.eps";
  
 // axes labels for the final plot  
 string title = ";m (GeV/c^{2});Int. luminosity for 95% C.L. exclusion (pb^{-1})";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 650., 100, 0.5, 150.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {200,256,256,200,200};
 Double_t y_shaded[5] = {0.5,0.5,150,150,0.5};
 
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

 TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"F");
//  pl->SetFillStyle(3001);   
 pl->SetFillColor(kGray+1);   
 pl->Draw();

 gPad->RedrawAxis();
 
 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // array of integrated luminosities for 95% CL exclusion
//  Double_t L[size];
 Double_t L[7] = {0.862826, 1.93192, 8.11311, 29.9338, 106.959};
//  Double_t L_sys[size]; // with systematics included
 Double_t L_sys[7] = {0.994139, 2.14055, 8.7662, 32.5288, 119.045}; // with systematics included
 
//  for(Int_t i = 0; i < size; i++){
//   
//   L[i] = L_for_exclusion(NPts, xsTh[i], rangeMin[i], rangeMax[i], 0, S_eff[i], 0, N_bkg_100[i], 0, mass[i]);
//   if(systematics) L_sys[i] = L_for_exclusion(NPts, xsTh[i], rangeMin_sys[i], rangeMax_sys[i], Sigma_L, S_eff[i], Sigma_S_eff[i], N_bkg_100[i], Sigma_N_bkg[i], mass[i]);
//  }

 TGraph *L95_vs_m = new TGraph(size, m, L);
 L95_vs_m->SetLineWidth(2);
 L95_vs_m->SetLineStyle(7);
 L95_vs_m->SetLineColor(kBlue);
 L95_vs_m->SetMarkerSize(.9);
 L95_vs_m->SetMarkerStyle(22);
 L95_vs_m->SetMarkerColor(kBlue);
 L95_vs_m->Draw("CP");

 TGraph *L95_vs_m_sys = new TGraph(size, m, L_sys);
 L95_vs_m_sys->SetLineWidth(2);
 L95_vs_m_sys->SetLineColor(kRed);
 L95_vs_m_sys->SetMarkerSize(.9);
 L95_vs_m_sys->SetMarkerStyle(21);
 L95_vs_m_sys->SetMarkerColor(kRed);
 if(systematics) L95_vs_m_sys->Draw("CP");

 TLegend *legend = new TLegend(.25,.7,.55,.85);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(L95_vs_m_sys,"with sys. unc.","lp");
 legend->AddEntry(L95_vs_m,"no sys. unc.","lp");
 if(systematics) legend->Draw();

//  TLatex l1;
//  l1.SetTextAlign(12);
//  l1.SetTextSize(0.04);
//  l1.SetTextFont(62);
//  l1.SetNDC();
//  l1.DrawLatex(0.6,0.24,"CMS Preliminary");

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
 delete legend;
 delete L95_vs_m;
 delete L95_vs_m_sys;
 delete bg;
 delete c;
}

Double_t xs_up(const Double_t fL_int, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg) 
{
 
 Double_t N_bkg       = (fL_int/100)*fN_bkg_100;
 Double_t Sigma_N_bkg = N_bkg*fSigma_N_bkg;
 
 cout<<">> =====================================\n";
 cout<<">> For L_int = "<<fL_int<<" pb^-1\n";
 cout<<">> =====================================\n";
 cout<<">> N_bkg = "<<N_bkg<<"\n";
 cout<<">> Sigma_N_bkg = "<<Sigma_N_bkg<<"\n";
 
 Double_t retval = CLA(fL_int, fL_int*fSigma_L, fS_eff, fS_eff*fSigma_S_eff, N_bkg, Sigma_N_bkg);
 return retval;
}

Double_t L_for_exclusion(const Int_t fNPts, const Double_t fxsTh, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle) 
{
 
 Double_t x[fNPts], y[fNPts];

 Double_t step = (fRangeMax - fRangeMin)/(fNPts-1);
   
 for (Int_t i = 0; i < fNPts; i++) {
 
  x[i] = fRangeMin + step*i;
  y[i] = xs_up(x[i], fSigma_L, fS_eff, fSigma_S_eff, fN_bkg_100, fSigma_N_bkg);
 }
 
 TCanvas *c_temp = new TCanvas("c_temp","",1120,800);
 c_temp->cd();

 string title = fTitle + ";L_{int} [pb^{-1}];95% C.L. upper limit on #sigma [pb]";

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

 Double_t L = f1->GetX(fxsTh);

 cout<<">> Integrated luminosity for 95% CL exclusion of "<<fTitle<<" = "<<L<<" pb^-1\n";
 
 if(fSigma_N_bkg != 0) c_temp->SaveAs((fTitle + "_exclusion_sys.png").c_str());
 else c_temp->SaveAs((fTitle + "_exclusion.png").c_str());
 
 delete xsection;
 delete f1;
 delete bg_temp;
 delete c_temp;
 
 return L;
}
