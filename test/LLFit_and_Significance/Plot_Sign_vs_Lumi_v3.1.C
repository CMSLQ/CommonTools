{

gROOT->Reset();

const float max_Lint = 1000;
const float max_Sign = 20;
const float max_mass = 1000;

//----------------------------------------------------


//## Significance vs Lint ##

TCanvas c1;
c1.SetLogx();
c1.SetGridx();
c1.SetGridy();

TLegend *legend = new TLegend(0.135057,0.684322,0.478448,0.860169);
legend->SetFillColor(kWhite);



//## 250
TFile f250("LLfit_and_SignCalc_M250.root");
f250.cd();

//default settings
g_SignVSLint->SetTitle(0);

//set axis
g_SignVSLint->GetYaxis()->SetLimits(0,max_Sign);
g_SignVSLint->GetYaxis()->SetRangeUser(0,max_Sign);
g_SignVSLint->GetYaxis()->SetTitle("significance");

g_SignVSLint->GetXaxis()->SetLimits(0,max_Lint);
g_SignVSLint->GetXaxis()->SetRangeUser(0,max_Lint);
g_SignVSLint->GetXaxis()->SetTitle("integrated luminosity (pb^{-1})");

//draw graph
g_SignVSLint->SetMarkerStyle(20);
g_SignVSLint->SetMarkerColor(kRed);
g_SignVSLint->SetLineColor(kRed);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("apL");

legend->AddEntry(&g_SignVSLint,"M(LQ)=250GeV","p");

//## 250 R down
TFile f250_Rdown("LLfit_and_SignCalc_M250_Rdown.root");
f250_Rdown.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(20);
g_SignVSLint->SetMarkerColor(kRed);
g_SignVSLint->SetLineColor(kRed);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");


//## 250 R up
TFile f250_Rup("LLfit_and_SignCalc_M250_Rup.root");
f250_Rup.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(20);
g_SignVSLint->SetMarkerColor(kRed);
g_SignVSLint->SetLineColor(kRed);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");



//## 400
TFile f400("LLfit_and_SignCalc_M400.root");
f400.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(21);
g_SignVSLint->SetMarkerColor(kBlue);
g_SignVSLint->SetLineColor(kBlue);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");

legend->AddEntry(&g_SignVSLint,"M(LQ)=400GeV","p");

//## 400 R down
TFile f400_Rdown("LLfit_and_SignCalc_M400_Rdown.root");
f400_Rdown.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(21);
g_SignVSLint->SetMarkerColor(kBlue);
g_SignVSLint->SetLineColor(kBlue);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");


//## 400 R up
TFile f400_Rup("LLfit_and_SignCalc_M400_Rup.root");
f400_Rup.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(21);
g_SignVSLint->SetMarkerColor(kBlue);
g_SignVSLint->SetLineColor(kBlue);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");


//## 650
TFile f650("LLfit_and_SignCalc_M650.root");
f650.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(23);
g_SignVSLint->SetMarkerColor(kBlack);
g_SignVSLint->SetLineColor(kBlack);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");

legend->AddEntry(&g_SignVSLint,"M(LQ)=650GeV","p");

//## 650 R down
TFile f650_Rdown("LLfit_and_SignCalc_M650_Rdown.root");
f650_Rdown.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(23);
g_SignVSLint->SetMarkerColor(kBlack);
g_SignVSLint->SetLineColor(kBlack);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");


//## 650 R up
TFile f650_Rup("LLfit_and_SignCalc_M650_Rup.root");
f650_Rup.cd();

//draw graph
g_SignVSLint->SetMarkerStyle(23);
g_SignVSLint->SetMarkerColor(kBlack);
g_SignVSLint->SetLineColor(kBlack);
g_SignVSLint->SetMarkerSize(1);
g_SignVSLint->Draw("pLsame");



//Draw 5 significance line
TLine line(0, 5, max_Lint, 5);
line.SetLineWidth(3);
line.SetLineStyle(2);
line.Draw();

//TLatex
TLatex *tInfo = new TLatex();
tInfo->SetNDC();
tInfo->SetTextSize(0.04);
tInfo->DrawLatex( 0.109195, 0.326271, "5 \\sigma discovery");

//Legend

legend->Draw();


gPad->RedrawAxis();
}
