{

//reset
gROOT->Reset();

//--------------------------

//root files
//output from /rootNtupleMacros/src/analysisClass_elecStudies2.C
TFile eleFile("eleEffLQUE250_new2.root");
//output from /rootNtupleMacros/src/analysisClass_muonStudies2.C
TFile muonFile("muonEffLQUE250_new2.root");

//TODO: improve the code to add functions and reduce the number of code lines

//--------------------------

//Pt variable binning
//-- LQ 250
const int Nbin1=25;
const int Nbin2=7;
const int Nbin3=1;
float step1=10;
float step2=50;
float step3=400;
//--
float lastBin1=step1*(Nbin1);
float lastBin2=step1*(Nbin1) + step2*(Nbin2);
float lastBin3=step1*(Nbin1) + step2*(Nbin2) + step3*(Nbin3);
cout << "lastBin1: " << lastBin1 << endl;
cout << "lastBin2: " << lastBin2 << endl;
cout << "lastBin3: " << lastBin3 << endl;
const int NbinTot = Nbin1 + Nbin2 + Nbin3;
Float_t Bins[NbinTot];
for(int bin=0; bin<Nbin1; bin++)
{
  Bins[bin] = step1 * (bin +1) ;
  cout << Bins[bin] << endl;
}
cout << "-------" << endl;

for(int bin=Nbin1; bin<Nbin2+Nbin1; bin++)
{
  Bins[bin] = Bins[ (Nbin1-1) ] + step2 * ( bin - (Nbin1 - 1) );
  cout << Bins[bin] << endl;
}

cout << "-------" << endl;

for(int bin=Nbin2+Nbin1; bin<Nbin3+Nbin2+Nbin1; bin++)
{
  Bins[bin] = Bins[ Nbin2+Nbin1-1 ] + step3 * ( bin - (Nbin2 + Nbin1 - 1) );
  cout << Bins[bin] << endl;
}

//--------------------------

//ele
eleFile.cd();

TCanvas c1;
c1->SetGridy();
c1->SetGridx();

int Xmin = -3;
int Xmax = 3;

TGraphAsymmErrors eleEff_eta(ele_Eta_Gen_matched,ele_Eta_Gen);
TGraphAsymmErrors eleEff_eta_ID(ele_Eta_Gen_matched_ID,ele_Eta_Gen);
TGraphAsymmErrors eleEff_eta_ID_ISO(ele_Eta_Gen_matched_ID_ISO,ele_Eta_Gen);

eleEff_eta->SetMarkerSize(0.9);
eleEff_eta->SetMarkerStyle(20);
eleEff_eta->SetMarkerColor(kBlack);
eleEff_eta->GetXaxis()->SetRangeUser(Xmin,Xmax);
eleEff_eta->Draw("ap");

eleEff_eta->GetYaxis()->SetTitle("reconstruction efficiency");
eleEff_eta->GetXaxis()->SetTitle("electron \\eta_{gen}");

eleEff_eta_ID->SetMarkerSize(0.9);
eleEff_eta_ID->SetMarkerStyle(24);
eleEff_eta_ID->SetMarkerColor(kRed);
eleEff_eta_ID->Draw("samep");  

eleEff_eta_ID_ISO->SetMarkerSize(0.9);
eleEff_eta_ID_ISO->SetMarkerStyle(22);
eleEff_eta_ID_ISO->SetMarkerColor(kBlue);
eleEff_eta_ID_ISO->Draw("samep");  

TLegend *legend = new TLegend(0.369885, 0.180049, 0.630115, 0.435523);
legend->SetFillColor(kWhite);
legend->AddEntry(&eleEff_eta , "no cut (only MC match)" , "p");
legend->AddEntry(&eleEff_eta_ID , "ID cut" , "p");
legend->AddEntry(&eleEff_eta_ID_ISO , "Isolation cut" , "p");
legend->Draw();

// //draw fiducial region

// Int_t n = 5;
// Double_t x[5] = {-3,-2,-2,-3,-3};
// Double_t y[5] = {0,0,1,1,0};
// TPolyLine * pl = new TPolyLine(n,x,y,"F");
// pl->SetFillStyle(3005);   
// pl->SetFillColor(kRed);   
// pl->Draw();

// Double_t x1[5] = {-1.6,-1.4,-1.4,-1.6,-1.6};
// Double_t y1[5] = {0,0,1,1,0};
// TPolyLine * pl1 = new TPolyLine(n,x1,y1,"F");
// pl1->SetFillStyle(3005);   
// pl1->SetFillColor(kRed);   
// pl1->Draw();

// Double_t x2[5] = {1.4,1.6,1.6,1.4,1.4};
// Double_t y2[5] = {0,0,1,1,0};
// TPolyLine * pl2 = new TPolyLine(n,x2,y2,"F");
// pl2->SetFillStyle(3005);   
// pl2->SetFillColor(kRed);   
// pl2->Draw();

// Double_t x3[5] = {2,3,3,2,2};
// Double_t y3[5] = {0,0,1,1,0};
// TPolyLine * pl3 = new TPolyLine(n,x3,y3,"F");
// pl3->SetFillStyle(3005);   
// pl3->SetFillColor(kRed);   
// pl3->Draw();

TCanvas c2;
c2->SetGridy();
c2->SetGridx();

// const int  = Nbin1 + Nbin2 + Nbin3;
// Float_t Bins[NbinTot];

TH1F *ele_Pt_Gen_mod = new TH1F("ele_Pt_Gen_mod",
				"ele_Pt_Gen_mod",
				NbinTot-1, Bins);

TH1F *ele_Pt_Gen_matched_mod = new TH1F("ele_Pt_Gen_matched_mod",
					"ele_Pt_Gen_matched_mod",
					NbinTot-1, Bins);

TH1F *ele_Pt_Gen_matched_ID_mod = new TH1F("ele_Pt_Gen_matched_ID_mod",
					   "ele_Pt_Gen_matched_ID_mod",
					   NbinTot-1, Bins);

TH1F *ele_Pt_Gen_matched_ID_ISO_mod = new TH1F("ele_Pt_Gen_matched_ID_ISO_mod",
					       "ele_Pt_Gen_matched_ID_ISO_mod",
					       NbinTot-1, Bins);

//TODO: create a function instead??
//ele_Pt_Gen
int nBins_gen = ele_Pt_Gen->GetXaxis()->GetNbins();
float getxmax_gen = ele_Pt_Gen->GetXaxis()->GetXmax();
float getxmin_gen = ele_Pt_Gen->GetXaxis()->GetXmin();
float step_gen = (getxmax_gen - getxmin_gen) / float(nBins_gen);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_gen ; bin++)
{
  for(int entry=0;entry<ele_Pt_Gen->GetBinContent(bin);entry++)
    {
      ele_Pt_Gen_mod->Fill(step_gen*bin);
    }
}
//ele_Pt_Gen_mod->Draw("HIST");

//ele_Pt_Gen_matched
int nBins = ele_Pt_Gen_matched->GetXaxis()->GetNbins();
float getxmax = ele_Pt_Gen_matched->GetXaxis()->GetXmax();
float getxmin = ele_Pt_Gen_matched->GetXaxis()->GetXmin();
float step = (getxmax - getxmin) / float(nBins);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins ; bin++)
{
  for(int entry=0;entry<ele_Pt_Gen_matched->GetBinContent(bin);entry++)
    {
      ele_Pt_Gen_matched_mod->Fill(step*bin);
    }
}
//ele_Pt_Gen_matched_mod->Draw("HIST");

//ele_Pt_Gen_matched_ID
int nBins_ID = ele_Pt_Gen_matched_ID->GetXaxis()->GetNbins();
float getxmax_ID = ele_Pt_Gen_matched_ID->GetXaxis()->GetXmax();
float getxmin_ID = ele_Pt_Gen_matched_ID->GetXaxis()->GetXmin();
float step_ID = (getxmax_ID - getxmin_ID) / float(nBins_ID);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_ID ; bin++)
{
  for(int entry=0;entry<ele_Pt_Gen_matched_ID->GetBinContent(bin);entry++)
    {
      ele_Pt_Gen_matched_ID_mod->Fill(step_ID*bin);
    }
}
//ele_Pt_Gen_matched_ID_mod->Draw("HIST");

//ele_Pt_Gen_matched_ID_ISO
int nBins_ID_ISO = ele_Pt_Gen_matched_ID_ISO->GetXaxis()->GetNbins();
float getxmax_ID_ISO = ele_Pt_Gen_matched_ID_ISO->GetXaxis()->GetXmax();
float getxmin_ID_ISO = ele_Pt_Gen_matched_ID_ISO->GetXaxis()->GetXmin();
float step_ID_ISO = (getxmax_ID_ISO - getxmin_ID_ISO) / float(nBins_ID_ISO);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_ID_ISO ; bin++)
{
  for(int entry=0;entry<ele_Pt_Gen_matched_ID_ISO->GetBinContent(bin);entry++)
    {
      ele_Pt_Gen_matched_ID_ISO_mod->Fill(step_ID_ISO*bin);
    }
}
//ele_Pt_Gen_matched_ID_mod->Draw("HIST");


TGraphAsymmErrors eleEff_Pt(ele_Pt_Gen_matched_mod,ele_Pt_Gen_mod);
TGraphAsymmErrors eleEff_Pt_ID(ele_Pt_Gen_matched_ID_mod,ele_Pt_Gen_mod);
TGraphAsymmErrors eleEff_Pt_ID_ISO(ele_Pt_Gen_matched_ID_ISO_mod,ele_Pt_Gen_mod);

eleEff_Pt->SetMarkerSize(0.9);
eleEff_Pt->SetMarkerStyle(20);
eleEff_Pt->SetMarkerColor(kBlack);
//eleEff_Pt->GetXaxis()->SetRangeUser(Xmin,Xmax);
eleEff_Pt->Draw("ap");

eleEff_Pt->GetYaxis()->SetTitle("reconstruction efficiency");
eleEff_Pt->GetXaxis()->SetTitle("electron Pt_{gen} (GeV)");

eleEff_Pt_ID->SetMarkerSize(0.9);
eleEff_Pt_ID->SetMarkerStyle(24);
eleEff_Pt_ID->SetMarkerColor(kRed);
eleEff_Pt_ID->Draw("samep");  

eleEff_Pt_ID_ISO->SetMarkerSize(0.9);
eleEff_Pt_ID_ISO->SetMarkerStyle(22);
eleEff_Pt_ID_ISO->SetMarkerColor(kBlue);
eleEff_Pt_ID_ISO->Draw("samep");  

TLegend *legend = new TLegend(0.178396, 0.180049, 0.438625, 0.435523);
legend->SetFillColor(kWhite);
legend->AddEntry(&eleEff_Pt , "no cut (only MC match)" , "p");
legend->AddEntry(&eleEff_Pt_ID , "ID cut" , "p");
legend->AddEntry(&eleEff_Pt_ID_ISO , "Isolation cut" , "p");
legend->Draw();


//----------------------------------------------
//----------------------------------------------


//muon
muonFile.cd();

TCanvas c3;
c3->SetGridy();
c3->SetGridx();

int Xmin = -3;
int Xmax = 3;

TGraphAsymmErrors muonEff_eta(muon_Eta_Gen_matched,muon_Eta_Gen);
TGraphAsymmErrors muonEff_eta_ID(muon_Eta_Gen_matched_ID,muon_Eta_Gen);
TGraphAsymmErrors muonEff_eta_ID_ISO(muon_Eta_Gen_matched_ID_ISO,muon_Eta_Gen);

muonEff_eta->SetMarkerSize(0.9);
muonEff_eta->SetMarkerStyle(20);
muonEff_eta->SetMarkerColor(kBlack);
muonEff_eta->GetXaxis()->SetRangeUser(Xmin,Xmax);
muonEff_eta->Draw("ap");

muonEff_eta->GetYaxis()->SetTitle("reconstruction efficiency");
muonEff_eta->GetXaxis()->SetTitle("muon \\eta_{gen}");

muonEff_eta_ID->SetMarkerSize(0.9);
muonEff_eta_ID->SetMarkerStyle(24);
muonEff_eta_ID->SetMarkerColor(kRed);
muonEff_eta_ID->Draw("samep");  

muonEff_eta_ID_ISO->SetMarkerSize(0.9);
muonEff_eta_ID_ISO->SetMarkerStyle(22);
muonEff_eta_ID_ISO->SetMarkerColor(kBlue);
//muonEff_eta_ID_ISO->Draw("samep");  

TLegend *legend = new TLegend(0.369885, 0.180049, 0.630115, 0.435523);
legend->SetFillColor(kWhite);
legend->AddEntry(&muonEff_eta , "no cut (only MC match)" , "p");
legend->AddEntry(&muonEff_eta_ID , "ID cut" , "p");
//legend->AddEntry(&muonEff_eta_ID_ISO , "Isolation cut" , "p");
legend->Draw();

// //draw fiducial region

// Int_t n = 5;
// Double_t x[5] = {-3,-2,-2,-3,-3};
// Double_t y[5] = {0,0,1,1,0};
// TPolyLine * pl = new TPolyLine(n,x,y,"F");
// pl->SetFillStyle(3005);   
// pl->SetFillColor(kRed);   
// pl->Draw();

// Double_t x1[5] = {-1.6,-1.4,-1.4,-1.6,-1.6};
// Double_t y1[5] = {0,0,1,1,0};
// TPolyLine * pl1 = new TPolyLine(n,x1,y1,"F");
// pl1->SetFillStyle(3005);   
// pl1->SetFillColor(kRed);   
// pl1->Draw();

// Double_t x2[5] = {1.4,1.6,1.6,1.4,1.4};
// Double_t y2[5] = {0,0,1,1,0};
// TPolyLine * pl2 = new TPolyLine(n,x2,y2,"F");
// pl2->SetFillStyle(3005);   
// pl2->SetFillColor(kRed);   
// pl2->Draw();

// Double_t x3[5] = {2,3,3,2,2};
// Double_t y3[5] = {0,0,1,1,0};
// TPolyLine * pl3 = new TPolyLine(n,x3,y3,"F");
// pl3->SetFillStyle(3005);   
// pl3->SetFillColor(kRed);   
// pl3->Draw();

TCanvas c4;
c4->SetGridy();
c4->SetGridx();

// const int  = Nbin1 + Nbin2 + Nbin3;
// Float_t Bins[NbinTot];

TH1F *muon_Pt_Gen_mod = new TH1F("muon_Pt_Gen_mod",
				"muon_Pt_Gen_mod",
				NbinTot-1, Bins);

TH1F *muon_Pt_Gen_matched_mod = new TH1F("muon_Pt_Gen_matched_mod",
					"muon_Pt_Gen_matched_mod",
					NbinTot-1, Bins);

TH1F *muon_Pt_Gen_matched_ID_mod = new TH1F("muon_Pt_Gen_matched_ID_mod",
					   "muon_Pt_Gen_matched_ID_mod",
					   NbinTot-1, Bins);

TH1F *muon_Pt_Gen_matched_ID_ISO_mod = new TH1F("muon_Pt_Gen_matched_ID_ISO_mod",
						"muon_Pt_Gen_matched_ID_ISO_mod",
						NbinTot-1, Bins);

//TODO: create a function instead??
//muon_Pt_Gen
int nBins_gen = muon_Pt_Gen->GetXaxis()->GetNbins();
float getxmax_gen = muon_Pt_Gen->GetXaxis()->GetXmax();
float getxmin_gen = muon_Pt_Gen->GetXaxis()->GetXmin();
float step_gen = (getxmax_gen - getxmin_gen) / float(nBins_gen);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_gen ; bin++)
{
  for(int entry=0;entry<muon_Pt_Gen->GetBinContent(bin);entry++)
    {
      muon_Pt_Gen_mod->Fill(step_gen*bin);
    }
}
//muon_Pt_Gen_mod->Draw("HIST");

//muon_Pt_Gen_matched
int nBins = muon_Pt_Gen_matched->GetXaxis()->GetNbins();
float getxmax = muon_Pt_Gen_matched->GetXaxis()->GetXmax();
float getxmin = muon_Pt_Gen_matched->GetXaxis()->GetXmin();
float step = (getxmax - getxmin) / float(nBins);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins ; bin++)
{
  for(int entry=0;entry<muon_Pt_Gen_matched->GetBinContent(bin);entry++)
    {
      muon_Pt_Gen_matched_mod->Fill(step*bin);
    }
}
//muon_Pt_Gen_matched_mod->Draw("HIST");

//muon_Pt_Gen_matched_ID
int nBins_ID = muon_Pt_Gen_matched_ID->GetXaxis()->GetNbins();
float getxmax_ID = muon_Pt_Gen_matched_ID->GetXaxis()->GetXmax();
float getxmin_ID = muon_Pt_Gen_matched_ID->GetXaxis()->GetXmin();
float step_ID = (getxmax_ID - getxmin_ID) / float(nBins_ID);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_ID ; bin++)
{
  for(int entry=0;entry<muon_Pt_Gen_matched_ID->GetBinContent(bin);entry++)
    {
      muon_Pt_Gen_matched_ID_mod->Fill(step_ID*bin);
    }
}
//muon_Pt_Gen_matched_ID_mod->Draw("HIST");

//muon_Pt_Gen_matched_ID_ISO
int nBins_ID_ISO = muon_Pt_Gen_matched_ID_ISO->GetXaxis()->GetNbins();
float getxmax_ID_ISO = muon_Pt_Gen_matched_ID_ISO->GetXaxis()->GetXmax();
float getxmin_ID_ISO = muon_Pt_Gen_matched_ID_ISO->GetXaxis()->GetXmin();
float step_ID_ISO = (getxmax_ID_ISO - getxmin_ID_ISO) / float(nBins_ID_ISO);
//cout << "step: "  << step << endl;

for(int bin=1; bin<=nBins_ID_ISO ; bin++)
{
  for(int entry=0;entry<muon_Pt_Gen_matched_ID_ISO->GetBinContent(bin);entry++)
    {
      muon_Pt_Gen_matched_ID_ISO_mod->Fill(step_ID_ISO*bin);
    }
}
//muon_Pt_Gen_matched_ID_mod->Draw("HIST");


TGraphAsymmErrors muonEff_Pt(muon_Pt_Gen_matched_mod,muon_Pt_Gen_mod);
TGraphAsymmErrors muonEff_Pt_ID(muon_Pt_Gen_matched_ID_mod,muon_Pt_Gen_mod);
TGraphAsymmErrors muonEff_Pt_ID_ISO(muon_Pt_Gen_matched_ID_ISO_mod,muon_Pt_Gen_mod);

muonEff_Pt->SetMarkerSize(0.9);
muonEff_Pt->SetMarkerStyle(20);
muonEff_Pt->SetMarkerColor(kBlack);
//muonEff_Pt->GetXaxis()->SetRangeUser(Xmin,Xmax);
muonEff_Pt->Draw("ap");

muonEff_Pt->GetYaxis()->SetTitle("reconstruction efficiency");
muonEff_Pt->GetXaxis()->SetTitle("muon Pt_{gen} (GeV)");

muonEff_Pt_ID->SetMarkerSize(0.9);
muonEff_Pt_ID->SetMarkerStyle(24);
muonEff_Pt_ID->SetMarkerColor(kRed);
muonEff_Pt_ID->Draw("samep");  

muonEff_Pt_ID_ISO->SetMarkerSize(0.9);
muonEff_Pt_ID_ISO->SetMarkerStyle(22);
muonEff_Pt_ID_ISO->SetMarkerColor(kBlue);
//muonEff_Pt_ID_ISO->Draw("samep");  

TLegend *legend = new TLegend(0.178396, 0.180049, 0.438625, 0.435523);
legend->SetFillColor(kWhite);
legend->AddEntry(&muonEff_Pt , "no cut (only MC match)" , "p");
legend->AddEntry(&muonEff_Pt_ID , "ID cut" , "p");
//legend->AddEntry(&muonEff_Pt_ID_ISO , "Isolation cut" , "p");
legend->Draw();


//-------------------------------------------------------------
//-------------------------------------------------------------


// eff_e / eff_mu 

//Pt
TCanvas c5;

TGraphAsymmErrors ratioEff_eleOvermuon_Pt;
//TGraphErrors ratioEff_eleOvermuon_Pt;

int npoint_Pt=0;
for(int point=0; point<eleEff_Pt_ID_ISO->GetN(); point++)
{

  //cout << "point: " << point << endl;

  double x1,y1;
  double ehx1,ehy1;
  double elx1,ely1;
  double erelhy1,erelly1;

  double x2,y2;
  double ehx2,ehy2;
  double elx2,ely2;
  double erelhy2,erelly2;

  double r;  
  double ehr,elr;  
  double erelhr,erellr;

  //SetPoint(npoint,pass->GetBinCenter(b),mode);
  //void SetPointError(Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
  //  Double_t	GetErrorXhigh(Int_t i) const
  // virtual Double_t	GetErrorXlow(Int_t i) const

  eleEff_Pt_ID_ISO->GetPoint(point, x1, y1);
  ehx1 = eleEff_Pt_ID_ISO->GetErrorXhigh(point);  
  ehy1 = eleEff_Pt_ID_ISO->GetErrorYhigh(point);  
  elx1 = eleEff_Pt_ID_ISO->GetErrorXlow(point);  
  ely1 = eleEff_Pt_ID_ISO->GetErrorYlow(point);  

  muonEff_Pt_ID_ISO->GetPoint(point, x2, y2);
  ehx2 = muonEff_Pt_ID_ISO->GetErrorXhigh(point);  
  ehy2 = muonEff_Pt_ID_ISO->GetErrorYhigh(point);  
  elx2 = muonEff_Pt_ID_ISO->GetErrorXlow(point);  
  ely2 = muonEff_Pt_ID_ISO->GetErrorYlow(point);  

//   cout << "y1: " << y1 << endl;
//   cout << "y2: " << y2 << endl;

  if(y2==0 || y1==0)
    continue;

  r=y1/y2;

  //cout << "r: " << r << endl;
      
  erelhy1=ehy1/y1;
  erelhy2=ehy2/y2;
  
  erelly1=ely1/y1;
  erelly2=ely2/y2;
  
  erelhr=sqrt(erelhy1*erelhy1 + erelhy2*erelhy2);
  erellr=sqrt(erelly1*erelly1 + erelly2*erelly2);
  
  ehr=erelhr*r;
  elr=erellr*r;
  
//   cout << "r  ehr  elr x : " << endl;
//   cout << r << " " <<  ehr << " " <<  elr << " " << x1 << endl;

//   //the same..  
//   cout << "x1:" << x1 << endl;
//   cout << "x2:" << x2 << endl;

  ratioEff_eleOvermuon_Pt.SetPoint(npoint_Pt,x1,r);
  ratioEff_eleOvermuon_Pt.SetPointEXhigh(npoint_Pt, ehx1);
  ratioEff_eleOvermuon_Pt.SetPointEXlow(npoint_Pt, elx1);
  ratioEff_eleOvermuon_Pt.SetPointEYhigh(npoint_Pt, ehr);
  ratioEff_eleOvermuon_Pt.SetPointEYlow(npoint_Pt, elr);

  npoint_Pt++;

}

double firstX_Pt,firstY_Pt,lastX_Pt,lastY_Pt;
ratioEff_eleOvermuon_Pt.GetPoint(0, firstX_Pt, firstY_Pt);
ratioEff_eleOvermuon_Pt.GetPoint(ratioEff_eleOvermuon_Pt.GetN()-1, lastX_Pt, lastY_Pt);
// cout << "firstX_Pt: " << firstX_Pt << endl;
// cout << "lastX_Pt: " << lastX_Pt << endl;

ratioEff_eleOvermuon_Pt.GetYaxis()->SetTitle("ratio of reconstruction efficiency (\\epsilon_{ele}/\\epsilon_{muon})");
ratioEff_eleOvermuon_Pt.GetXaxis()->SetTitle("electron(muon) Pt_{gen} (GeV)");
ratioEff_eleOvermuon_Pt.SetMarkerSize(0.7);
ratioEff_eleOvermuon_Pt.SetMarkerStyle(20);
ratioEff_eleOvermuon_Pt.SetMarkerColor(kBlack);
ratioEff_eleOvermuon_Pt.SetMaximum(1.);
ratioEff_eleOvermuon_Pt.SetMinimum(0.5);

TF1 *f1_Pt = new TF1("f1_Pt","pol0",firstX_Pt,lastX_Pt);
ratioEff_eleOvermuon_Pt.Fit("f1_Pt","R");
ratioEff_eleOvermuon_Pt.Draw("ap");


//eta
TCanvas c6;

TGraphAsymmErrors ratioEff_eleOvermuon_eta;
//TGraphErrors ratioEff_eleOvermuon_eta;

int npoint_eta=0;
for(int point=0; point<eleEff_eta_ID_ISO->GetN(); point++)
{

  //  cout << "point: " << point << endl;

  double x1,y1;
  double ehx1,ehy1;
  double elx1,ely1;
  double erelhy1,erelly1;

  double x2,y2;
  double ehx2,ehy2;
  double elx2,ely2;
  double erelhy2,erelly2;

  double r;  
  double ehr,elr;  
  double erelhr,erellr;

  //SetPoint(npoint,pass->GetBinCenter(b),mode);
  //void SetPointError(Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
  //  Double_t	GetErrorXhigh(Int_t i) const
  // virtual Double_t	GetErrorXlow(Int_t i) const

  eleEff_eta_ID_ISO->GetPoint(point, x1, y1);
  ehx1 = eleEff_eta_ID_ISO->GetErrorXhigh(point);  
  ehy1 = eleEff_eta_ID_ISO->GetErrorYhigh(point);  
  elx1 = eleEff_eta_ID_ISO->GetErrorXlow(point);  
  ely1 = eleEff_eta_ID_ISO->GetErrorYlow(point);  

  muonEff_eta_ID_ISO->GetPoint(point, x2, y2);
  ehx2 = muonEff_eta_ID_ISO->GetErrorXhigh(point);  
  ehy2 = muonEff_eta_ID_ISO->GetErrorYhigh(point);  
  elx2 = muonEff_eta_ID_ISO->GetErrorXlow(point);  
  ely2 = muonEff_eta_ID_ISO->GetErrorYlow(point);  

//   cout << "y1: " << y1 << endl;
//   cout << "y2: " << y2 << endl;

  if(y2==0 || y1==0)
    continue;

  r=y1/y2;

  //cout << "r: " << r << endl;
      
  erelhy1=ehy1/y1;
  erelhy2=ehy2/y2;
  
  erelly1=ely1/y1;
  erelly2=ely2/y2;
  
  erelhr=sqrt(erelhy1*erelhy1 + erelhy2*erelhy2);
  erellr=sqrt(erelly1*erelly1 + erelly2*erelly2);
  
  ehr=erelhr*r;
  elr=erellr*r;
  
//   cout << "r  ehr  elr: " << endl;
//   cout << r << " " <<  ehr << " " <<  elr << endl;

//   //the same..  
//   cout << "x1:" << x1 << endl;
//   cout << "x2:" << x2 << endl;

  ratioEff_eleOvermuon_eta.SetPoint(npoint_eta,x1,r);
  ratioEff_eleOvermuon_eta.SetPointEXhigh(npoint_eta, ehx1);
  ratioEff_eleOvermuon_eta.SetPointEXlow(npoint_eta, elx1);
  ratioEff_eleOvermuon_eta.SetPointEYhigh(npoint_eta, ehr);
  ratioEff_eleOvermuon_eta.SetPointEYlow(npoint_eta, elr);

  npoint_eta++;

}

double firstX_eta,firstY_eta,lastX_eta,lastY_eta;
ratioEff_eleOvermuon_eta.GetPoint(0, firstX_eta, firstY_eta);
ratioEff_eleOvermuon_eta.GetPoint(ratioEff_eleOvermuon_eta.GetN()-1, lastX_eta, lastY_eta);
cout << "firstX_eta: " << firstX_eta << endl;
cout << "lastX_eta: " << lastX_eta << endl;

ratioEff_eleOvermuon_eta.GetYaxis()->SetTitle("ratio of reconstruction efficiency (\\epsilon_{ele}/\\epsilon_{muon})");
ratioEff_eleOvermuon_eta.GetXaxis()->SetTitle("electron(muon) \\eta_{gen} (GeV)");
ratioEff_eleOvermuon_eta.SetMarkerSize(0.7);
ratioEff_eleOvermuon_eta.SetMarkerStyle(20);
ratioEff_eleOvermuon_eta.SetMarkerColor(kBlack);
ratioEff_eleOvermuon_eta.SetMaximum(1.);
ratioEff_eleOvermuon_eta.SetMinimum(0.5);


//TF1 *f1_eta = new TF1("f1_eta","pol0",firstX_eta,lastX_eta);
TF1 *f1_eta = new TF1("f1_eta","pol0",-1.4,1.4);
ratioEff_eleOvermuon_eta.Fit("f1_eta","R");
ratioEff_eleOvermuon_eta.Draw("ap");


}
