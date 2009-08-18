#include "scp.C"
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

Double_t L_for_5sigma(const Int_t fNPts, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg_100, const string& fTitle);

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 
 bool systematics = true; 

 // array of N_signal for 100 pb^-1
 Double_t N_sig_100[4] = {359.39, 163.37, 40.41, 11.56};
//  Double_t N_sig_100[10] = {359.39, 163.37, 40.41, 11.56, 4.04, 2.43, 1.49, 0.59, 0.25, 0.11};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[10] = {9.94, 5.13, 2.28, 1.10, 1.10, 1.10, 1.10, 1.10, 1.10, 1.10};
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[10] = {0.45, 0.56, 0.47, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57};
 
 // arrays of leptoquark masses
 string mass[10] = {"m250", "m300", "m400", "m500", "m600", "m650", "m700", "m800", "m900", "m1000"};
 Double_t m[10] = {250, 300, 400, 500, 600, 650, 700, 800, 900, 1000};
 
 // arrays of integrated luminosity ranges (in pb^-1) that contain 5 sigma significance for different leptoquark mass
 Double_t rangeMin[10]     = {0.6, 1.2,  6.,  30., 180.,  500., 1000.,  4500., 25000., 110000.};
 Double_t rangeMax[10]     = {2.2, 4.8, 25., 100., 500., 1000., 2500., 13000., 70000., 400000.};
 Double_t rangeMin_sys[10] = {0.6, 1.3,  7.,  30.,  300.,   5000., 100000.,  4500., 25000., 110000.};
 Double_t rangeMax_sys[10] = {2.3, 5.0, 29., 150., 2000., 100000., 300000., 13000., 70000., 400000.};
 
 // number of points used for interpolation when obtaining integrated luminosities for 5 sigma discovery at a given leptoquark mass
 Int_t NPts = 10;
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "L5sigma_vs_m.eps";
  
 // axes labels for the final plot 
 string title = ";m (GeV/c^{2});Integrated luminosity for 5#sigma_{S_{cP}} (pb^{-1})";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 550., 100, 1, 150.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {200,256,256,200,200};
 Double_t y_shaded[5] = {1,1,150,150,1};
 
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
 
 Int_t size = sizeof(N_sig_100)/sizeof(*N_sig_100);

 // array of integrated luminosities for 5 sigma discovery
 Double_t L[size];
 Double_t L_sys[size]; // with systematics included
 
 for(Int_t i = 0; i < size; i++){
  
  L[i] = L_for_5sigma(NPts, rangeMin[i], rangeMax[i], N_sig_100[i], N_bkg_100[i], 0, mass[i]);
  if(systematics) L_sys[i] = L_for_5sigma(NPts, rangeMin_sys[i], rangeMax_sys[i], N_sig_100[i], N_bkg_100[i], Sigma_N_bkg[i], mass[i]);
 }

 TGraph *L5_vs_m = new TGraph(size, m, L);
 L5_vs_m->SetLineWidth(2);
 L5_vs_m->SetLineStyle(7);
 L5_vs_m->SetLineColor(kBlue);
 L5_vs_m->SetMarkerSize(0.9);
 L5_vs_m->SetMarkerStyle(22);
 L5_vs_m->SetMarkerColor(kBlue);
 L5_vs_m->Draw("CP"); 

 TGraph *L5_vs_m_sys = new TGraph(size, m, L_sys);
 L5_vs_m_sys->SetLineWidth(2);
 L5_vs_m_sys->SetLineColor(kRed);
 L5_vs_m_sys->SetMarkerSize(0.9);
 L5_vs_m_sys->SetMarkerStyle(21);
 L5_vs_m_sys->SetMarkerColor(kRed);
 if(systematics) L5_vs_m_sys->Draw("CP"); 

 TLegend *legend = new TLegend(.27,.7,.57,.85);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(L5_vs_m_sys,"with sys. unc.","lp");
 legend->AddEntry(L5_vs_m,"no sys. unc.","lp");
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
 delete L5_vs_m;
 delete L5_vs_m_sys;
 delete bg;
 delete c;
}

Double_t ScP(const Double_t fL_int, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg) 
{
 
 Double_t N_sig       = (fL_int/100)*fN_sig_100;
 Double_t N_bkg       = (fL_int/100)*fN_bkg_100;
 Double_t Sigma_N_bkg = N_bkg*fSigma_N_bkg;
 
 cout<<">> =====================================\n";
 cout<<">> For L_int = "<<fL_int<<" pb^-1\n";
 cout<<">> =====================================\n";
 cout<<">> N_sig = "<<N_sig<<"\n";
 cout<<">> N_bkg = "<<N_bkg<<"\n";
 cout<<">> Sigma_N_bkg = "<<Sigma_N_bkg<<"\n";
 
 return scp(N_sig, N_bkg, Sigma_N_bkg, 0);
}

Double_t L_for_5sigma(const Int_t fNPts, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fN_sig_100, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle) 
{
 
 Double_t x[fNPts], y[fNPts];

 Double_t step = (fRangeMax - fRangeMin)/(fNPts-1);
   
 for (Int_t i = 0; i < fNPts; i++) {
 
  x[i] = fRangeMin + step*i;
  y[i] = ScP(x[i], fN_sig_100, fN_bkg_100, fSigma_N_bkg);
 }
 
 TCanvas *c_temp = new TCanvas("c_temp","",1120,800);
 c_temp->cd();

 string title = fTitle + ";Integrated luminosity [pb^{-1}];S_{cP}";

 TH2F *bg_temp = new TH2F("bg_temp",title.c_str(), 100, fRangeMin, fRangeMax, 100, 0.8*y[0], 1.2*y[fNPts-1]);
 bg_temp->SetStats(kFALSE);
 bg_temp->SetTitleOffset(1.,"X");
 bg_temp->SetTitleOffset(1.,"Y");
 bg_temp->Draw();
 
 TF1 *f1 = new TF1("f1","[0]+[1]*sqrt(x)",fRangeMin,fRangeMax);
 
 TGraph *scP = new TGraph(fNPts, x, y);
 scP->SetMarkerSize(1.);
 scP->SetMarkerStyle(24);
 scP->SetMarkerColor(kRed);
 scP->Draw("P"); 
 scP->Fit("f1");
 
 Double_t L = f1->GetX(5);
 Double_t N_sig = (L/100)*fN_sig_100;
 Double_t N_bkg = (L/100)*fN_bkg_100;
 Double_t N_s_b = N_sig + N_bkg;

 cout<<">> Integrated luminosity for 5 sigma discovery for "<<fTitle<<" = "<<L<<" pb^-1\n";
 cout<<">> ** N_sig = "<<N_sig<<"\n";
 cout<<">> ** N_bkg = "<<N_bkg<<"\n";
 cout<<">> ** N_s_b = "<<N_s_b<<"\n";

 if(fSigma_N_bkg != 0) c_temp->SaveAs((fTitle + "_significance_sys.png").c_str());
 else c_temp->SaveAs((fTitle + "_significance.png").c_str());
 
 delete scP;
 delete f1;
 delete bg_temp;
 delete c_temp;
 
 return L;
}
