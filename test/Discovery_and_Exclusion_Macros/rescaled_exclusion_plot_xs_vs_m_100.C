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
  bool plot7TeV = true;
  bool plot10TeV = false;

 // array of signal efficiencies
 Double_t S_eff[5] = {0.36, 0.43, 0.54, 0.59, 0.66};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
 
 // arrays of leptoquark masses
 Double_t m[10] = {250, 300, 400, 500, 600, 650, 700, 800, 900, 1000};

 // array of theoretical cross-sections for different leptoquark masses
 Double_t xsTh[10] = {10.1, 3.8, 0.75, 0.197, 0.0617, 0.0362, 0.0218, 0.0085, 0.00351, 0.00153};
 Double_t xs7Th[10] = {3.48, 1.21, 0.206, 0.0463, 0.0124, 0.00676, 0.00377, 0.00124, 0.000437, 0.00016};
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "xs95CL_vs_m.eps";
 if (plot7TeV && !plot10TeV) fileName =  "7TeV_xs95CL_vs_m.eps";
 if (!plot7TeV && plot10TeV) fileName =  "10TeV_xs95CL_vs_m.eps";
  
 // axes labels for the final plot  
 string title = ";m (GeV/c^{2});#beta^{2}#times#sigma (pb)";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 200., 650., 100, 0.01, 50.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {200,256,256,200,200};
 Double_t y_shaded[5] = {0.01,0.01,50,50,0.01};
 Double_t NEW_x_shaded[2] = {299,299};
 Double_t NEW_y_shaded[2] = {0.01,50};

 // PDF uncertainty band
 Double_t x_pdf[12] = {250,300,400,500,600,650,650,600,500,400,300,250};
 Double_t y_pdf[12] = {9.64045,3.6081,0.691125,0.176217,0.0538333,0.0312225,0.0424445,0.071356,0.221527,0.819375,4.0147,10.5293};
 Double_t y_NEWpdf[12] = {3.3321,1.142,0.187,0.04054,0.01046,0.00554,0.00792,0.01434,0.05206,0.225,1.278,3.6279};

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
 if(plot10TeV) grshade->Draw("f");

 TGraph *gr7shade = new TGraph(12,x_pdf,y_NEWpdf);
 gr7shade->SetFillColor(kYellow);
 if(plot7TeV) gr7shade->Draw("f");

 TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"F");
//  pl->SetFillStyle(3001);   
 pl->SetFillColor(kGray+1);   
 pl->Draw();
 TPolyLine *pl_NEW = new TPolyLine(2,NEW_x_shaded,NEW_y_shaded,"L");
 pl_NEW->SetLineStyle(6);   
 pl_NEW->SetLineWidth(3);   
 pl_NEW->Draw();

 gPad->RedrawAxis();
 
 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // array of 95% CL upper limits on the cross section
 Double_t xsUp[5] = {0.23722, 0.166074, 0.10131, 0.0748393, 0.0669023};
 Double_t NEWx[5] = {250,300,400,500,600};
 Double_t STRLy[5]= {0.17612, 0.112823, 0.0709512, 0.051767, 0.0495704};//rescaled with Stirling plot
 Double_t MCx[3] = {300,400,500};
  Double_t MCy[3] = {0.182471,0.0858152,0.0585313}; //7TeV MC Samples
  Double_t CRSCy[5] = {0.198158,0.123589,0.080478,0.064229,0.0575465};//rescaled with cross section ratio


//  for(Int_t i = 0; i < size; i++){
//   
//   xsUp[i] = CLA(100, 0, S_eff[i], 0, N_bkg_100[i], 0);
//  }

 TGraph *xsUp_vs_m = new TGraph(size, m, xsUp);
 xsUp_vs_m->SetLineWidth(2);
 xsUp_vs_m->SetLineStyle(9);
 xsUp_vs_m->SetLineColor(kBlue);
 xsUp_vs_m->SetMarkerSize(.9);
 xsUp_vs_m->SetMarkerStyle(22);
 xsUp_vs_m->SetMarkerColor(kBlue);
 if(plot10TeV) xsUp_vs_m->Draw("CP");

 TGraph *xsTh_vs_m = new TGraph(size+1, m, xsTh);
 xsTh_vs_m->SetLineWidth(2);
 xsTh_vs_m->SetLineColor(kBlue);
 xsTh_vs_m->SetFillColor(kYellow);
 xsTh_vs_m->SetMarkerSize(1.);
 xsTh_vs_m->SetMarkerStyle(22);
 xsTh_vs_m->SetMarkerColor(kBlue);
 if(plot10TeV) xsTh_vs_m->Draw("C");

 TGraph *xs7Th_vs_m = new TGraph(size+1, m, xs7Th);
 xs7Th_vs_m->SetLineWidth(2);
 xs7Th_vs_m->SetLineColor(kRed);
 xs7Th_vs_m->SetFillColor(kYellow);
 xs7Th_vs_m->SetMarkerSize(1.);
 xs7Th_vs_m->SetMarkerStyle(22);
 xs7Th_vs_m->SetMarkerColor(kRed);
 if(plot7TeV) xs7Th_vs_m->Draw("C");

 TGraph *STRLxsection = new TGraph(5, NEWx, STRLy);
 STRLxsection->SetMarkerStyle(23);
 STRLxsection->SetMarkerColor(kRed);
 STRLxsection->SetLineColor(kRed);
 STRLxsection->SetLineWidth(2);
 STRLxsection->SetLineStyle(9);
 STRLxsection->SetMarkerSize(.9);
 if(plot7TeV) STRLxsection->Draw("CP");

 TGraph *CRSCxsection = new TGraph(5, NEWx, CRSCy);
 CRSCxsection->SetMarkerStyle(23);
 CRSCxsection->SetMarkerColor(kGreen);
 CRSCxsection->SetLineColor(kGreen);
 CRSCxsection->SetLineWidth(2);
 CRSCxsection->SetLineStyle(9);
 CRSCxsection->SetMarkerSize(.9);
 //CRSCxsection->Draw("CP");

 TGraph *MCxsection = new TGraph(3, MCx, MCy);
 MCxsection->SetMarkerStyle(23);
 MCxsection->SetMarkerColor(kBlack);
 MCxsection->SetLineColor(kBlack);
 MCxsection->SetLineWidth(2);
 MCxsection->SetLineStyle(9);
 MCxsection->SetMarkerSize(.9);
 //MCxsection->Draw("CP");

 TLegend *legend = new TLegend(.35,.69,.90,.86);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.15);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(pl,"Tevatron exclusion (250 pb^{-1})","f");
 legend->AddEntry(pl_NEW,"Tevatron exclusion (1 fb^{-1})","l");
 if(plot10TeV) legend->AddEntry(xsTh_vs_m,"#beta^{2}#times #sigma_{theory} with PDF unc. band, #beta=1","lf");
 if(plot7TeV) legend->AddEntry(xs7Th_vs_m,"#beta^{2}#times #sigma_{theory} with PDF unc. band, #beta=1","lf");
 if(plot10TeV) legend->AddEntry(xsUp_vs_m,"95% C.L. upper limit (no sys. unc.)","lp");
 if(plot7TeV) legend->AddEntry(STRLxsection,"95% C.L. upper limit (no sys. unc.)","lp");
 //legend->AddEntry(CRSCxsection,"xsec ratio - 7TeV 95% C.L. upper limit (no sys. unc.)","lp");
 //legend->AddEntry(MCxsection,"MC Samples - 7TeV 95% C.L. upper limit (no sys. unc.)","lp");
 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextSize(0.04);
 l1.SetTextFont(62);
 l1.SetNDC();
 l1.DrawLatex(0.6,0.64,"CMS Preliminary");

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextSize(0.04);
 l2.SetTextFont(62);
 l2.SetNDC();
 l2.DrawLatex(0.6,0.55,"#intLdt=100 pb^{-1}");

 if (plot7TeV){
 TLatex l3;
 l3.SetTextAlign(12);
 l3.SetTextSize(0.037);
 l3.SetTextFont(62);
 l3.SetNDC();
 l3.DrawLatex(0.4,0.485,"EXO-08-010 scaled to #sqrt{s} = 7 TeV");
 }

 c->SetGridx();
 c->SetGridy();

 string name, extension;
 size_t pos = fileName.find(".");

 name = fileName.substr(0,pos);
 extension = fileName.substr(pos);

 c->SetLogy();
 c->SaveAs((name + "_log" + extension).c_str());

 delete pl;
 delete xsTh_vs_m;
 delete xsUp_vs_m;
 delete bg;
 delete c;
}
