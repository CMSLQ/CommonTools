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

Double_t beta_for_exclusion(const Int_t fNPts, const Double_t fxsTh, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle);

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 
  bool plot7TeV = false;
  bool plot10TeV = true;

 // array of signal efficiencies
 Double_t S_eff[5] = {0.36, 0.43, 0.54, 0.59, 0.66};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
 Double_t N_bkg_100_7TeV[5] = {4.068, 1.937, 0.816, 0.3841, 0.3841};
 
 // arrays of leptoquark masses
 string mass[5] = {"m250", "m300", "m400", "m500", "m600"};
 Double_t m[5] = {250, 300, 400, 500, 600};
 
 
 // array of theoretical cross-sections for different leptoquark mass
 Double_t xsTh[5] = {9.64045, 3.6081, 0.691125, 0.176217, 0.0538333};
 Double_t xs7Th[10] = {3.48, 1.21, 0.206, 0.0463, 0.0124, 0.00676, 0.00377, 0.00124, 0.000437, 0.00016};
 
 // arrays of beta^2 ranges that contain 95% CL exclusion of different leptoquark mass hypotheses
 Double_t rangeMin[5] = {0.018, 0.03, 0.09, 0.3,  0.9};
 Double_t rangeMax[5] = { 0.03, 0.08,  0.2, 0.55, 1.5};
 Double_t rangeMin_7TeV[5] = {0.03,  0.08,  0.35,  1.0, 3.5};
 Double_t rangeMax_7TeV[5] = {0.08, 0.25,  0.55, 1.5, 5.0};

 // number of points used for interpolation
 Int_t NPts = 5;
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "beta_vs_m_excl.eps";
 if (plot7TeV && !plot10TeV) fileName = "7TeV_beta_vs_m_excl.eps";
 if (!plot7TeV && plot10TeV) fileName = "10TeV_beta_vs_m_excl.eps";
  
 // axes labels for the final plot 
 string title = ";m (GeV/c^{2});#beta";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 650., 100, 0., 1.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 //TF1 *fit1 = new TF1("fit1","[0]*exp([1]*x)",200,600);
 TF1 *fit1 = new TF1("fit1","[0]+[1]*x+[2]*pow(x,2)",200,600);

 //TF1 *fit2 = new TF1("fit2","[0]*exp([1]*x)",200,600);
 TF1 *fit2 = new TF1("fit2","[0]+[1]*x+[2]*pow(x,2)",200,600);
 
 // region excluded by Tevatron limits
 Double_t x_shaded[14] = {200,205,210,215,220,225,230,235,240,245,250,255,256,200};  // 250 pb-1
 Double_t y_shaded[14] = {sqrt(0.0267547),sqrt(0.0380903),sqrt(0.0531225),sqrt(0.0736446),sqrt(0.10221),sqrt(0.142373),sqrt(0.194058),sqrt(0.265091),sqrt(0.385415),sqrt(0.520114),sqrt(0.721552),sqrt(0.945015),1,1};
 Double_t NEW_x_shaded[14] = {216,220,225,231,235,254,268,276,284,289,293,296,297,299};
 Double_t NEW_y_shaded[14] = {0.02,0.04,0.06,0.08,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1};  // 1 fb-1

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
 TGraph *NEWexcl = new TGraph(14,NEW_x_shaded,NEW_y_shaded);
 NEWexcl->SetLineStyle(6);
 NEWexcl->SetLineWidth(3);
 NEWexcl->Draw("C");

 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // array of minimum beta^2 values for 95% CL exclusion
//  Double_t beta[size];
 Double_t beta[5] = {0.156865, 0.214541, 0.382863, 0.655572, 1.11471};
 Double_t beta_ln[5] = {log(0.156865), log(0.214541), log(0.382863), log(0.655572), log(1.11471)};
 Double_t beta_7TeV[5] = {0.221188, 0.307225, 0.59993, 1.14052, 2.08235};
 Double_t beta_7TeV_ln[5] = {log(0.221188), log(0.307225), log(0.59993), log(1.14052), log(2.08235)};

//  for(Int_t i = 0; i < size; i++){
  
//   beta_7TeV[i] = beta_for_exclusion(NPts, xs7Th[i], rangeMin_7TeV[i], rangeMax_7TeV[i], 0, S_eff[i], 0, N_bkg_100_7TeV[i], 0, mass[i]);
//   beta_7TeV_ln[i] = log(beta_7TeV[i]);
//  }

 TGraph *beta_vs_m = new TGraph(size, m, beta);
 TGraph *beta_vs_m_fit = new TGraph(size, m, beta_ln);
 beta_vs_m->SetLineWidth(2);
 beta_vs_m->SetLineStyle(7);
 beta_vs_m->SetLineColor(kBlue);
 beta_vs_m->SetMarkerSize(.9);
 beta_vs_m->SetMarkerStyle(22);
 beta_vs_m->SetMarkerColor(kBlue); 
 beta_vs_m_fit->Fit("fit1");
 TF1 *final1 = new TF1("final1","exp(fit1)",200,600);
 final1->SetLineWidth(2);
 final1->SetLineStyle(7);
 final1->SetLineColor(kBlue);
 final1->SetRange(200,600);

 TGraph *beta_vs_m_7TeV = new TGraph(size, m, beta_7TeV);
 TGraph *beta_vs_m_7TeV_fit = new TGraph(size, m, beta_7TeV_ln);
 beta_vs_m_7TeV->SetLineWidth(2);
 beta_vs_m_7TeV->SetLineStyle(7);
 beta_vs_m_7TeV->SetLineColor(kRed);
 beta_vs_m_7TeV->SetMarkerSize(.9);
 beta_vs_m_7TeV->SetMarkerStyle(22);
 beta_vs_m_7TeV->SetMarkerColor(kRed); 
 beta_vs_m_7TeV_fit->Fit("fit2");
 TF1 *final2 = new TF1("final2","exp(fit2)",200,600);
 final2->SetLineWidth(2);
 final2->SetLineStyle(7);
 final2->SetLineColor(kRed);
 final2->SetRange(200,600);

 const Int_t n = 21;
 Double_t x[n], ymin[n], ymin_sys[n], ymax[n];
 for(Int_t i=0;i<n;i++) {
   x[i] = 200+20*i;
   if (plot10TeV) ymin[i] = final1->Eval(x[i]);
   else if (plot7TeV) ymin[i] = final2->Eval(x[i]);
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
 grshade->SetFillStyle(3004);
 if (plot10TeV) grshade->SetFillColor(kBlue);
 if (plot10TeV) grshade->SetLineColor(kBlue);
 if (plot7TeV) grshade->SetFillColor(kRed);
 if (plot7TeV) grshade->SetLineColor(kRed);
 grshade->Draw("F");
 
 gPad->RedrawAxis();
 
 if (plot10TeV) beta_vs_m->Draw("P");
 if (plot10TeV) final1->Draw("same");
 if (plot7TeV) beta_vs_m_7TeV->Draw("Psame");
 if (plot7TeV) final2->Draw("same");
 
 TLegend *legend = new TLegend(.14,.65,.54,.85);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.2);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(grexcl,"Tevatron exclusion (250 pb^{-1})","f");
 legend->AddEntry(NEWexcl,"Tevatron exclusion (1 fb^{-1})","l");
 if (plot10TeV) legend->AddEntry(beta_vs_m,"95% C.L. (no sys. unc.)","lp");
 if (plot7TeV) legend->AddEntry(beta_vs_m_7TeV,"95% C.L. (no sys. unc.)","lp");
 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextSize(0.04);
 l1.SetTextFont(62);
 l1.SetNDC();
 l1.DrawLatex(0.6,0.34,"CMS Preliminary");

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextSize(0.04);
 l2.SetTextFont(62);
 l2.SetNDC();
 l2.DrawLatex(0.6,0.25,"#intLdt=100 pb^{-1}");

 if (plot7TeV){
 TLatex l3;
 l3.SetTextAlign(12);
 l3.SetTextSize(0.035);
 l3.SetTextFont(62);
 l3.SetNDC();
 l3.DrawLatex(0.4,0.16,"EXO-08-010 scaled to #sqrt{s} = 7 TeV");
 }

 c->SetGridx();
 c->SetGridy();
 c->SaveAs(fileName.c_str());

 
 delete fit1;
 delete final1;
 delete fit2;
 delete grexcl;
 delete grshade;
 delete legend;
 delete beta_vs_m;
 delete beta_vs_m_fit;
 delete bg;
 delete c;
}

Double_t beta_for_exclusion(const Int_t fNPts, const Double_t fxsTh, const Double_t fRangeMin, const Double_t fRangeMax, const Double_t fSigma_L, const Double_t fS_eff, const Double_t fSigma_S_eff, const Double_t fN_bkg_100, const Double_t fSigma_N_bkg, const string& fTitle) 
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

 cout<<">> beta for 95% CL exclusion of "<<fTitle<<" = "<<sqrt(beta2)<<"\n";
 
 if(fSigma_N_bkg != 0) c_temp->SaveAs((fTitle + "_exclusion_sys.png").c_str());
 else c_temp->SaveAs((fTitle + "_exclusion.png").c_str());
 
 delete xsection;
 delete f1;
 delete bg_temp;
 delete c_temp;
 
 return sqrt(beta2);
}
