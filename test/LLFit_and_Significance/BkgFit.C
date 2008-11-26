{

gROOT->Reset();
gStyle->SetErrorX(0);

gSystem->Load("libRooFit");
using namespace RooFit;
using namespace std;


//----------------------------------------------------------- 
//## Modify only this part

//## Background input files 
TFile f_ttbar("ttbar.root");
TFile f_zjet("zjet.root");
TFile f_wjet("wjet.root");
TFile f_ww("ww.root");
TFile f_zz("zz.root");
TFile f_wz("wz.root");
TFile f_qcdjet("qcdjet.root");
TFile f_gammajet("gammajet.root");

int rebin=1;

//----------------------------------------------------------- 


//## Get histo
TH1F *h_Mej_ttbar=(TH1F*) f_ttbar->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_zjet=(TH1F*) f_zjet->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_wjet=(TH1F*) f_wjet->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_ww=(TH1F*) f_ww->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_wz=(TH1F*) f_wz->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_zz=(TH1F*) f_zz->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_qcdjet=(TH1F*) f_qcdjet->Get("h_Mej_best_Meecut_Stcut");  
TH1F *h_Mej_gammajet=(TH1F*) f_gammajet->Get("h_Mej_best_Meecut_Stcut");  


//## Rebin histo
h_Mej_ttbar->Rebin(rebin);
h_Mej_zjet->Rebin(rebin);
h_Mej_wjet->Rebin(rebin);
h_Mej_ww->Rebin(rebin);
h_Mej_wz->Rebin(rebin);
h_Mej_zz->Rebin(rebin);
h_Mej_qcdjet->Rebin(rebin);
h_Mej_gammajet->Rebin(rebin);


//## Create stack histograms
int Nbins = h_Mej_ttbar->GetXaxis()->GetNbins();
float xmin = h_Mej_ttbar->GetXaxis()->GetXmin();
float xmax = h_Mej_ttbar->GetXaxis()->GetXmax();
cout << "Nbins: " << Nbins << endl;
cout << "xmin: " << xmin << endl;
cout << "xmax: " << xmax << endl;

TH1F *h_Mej_allBkg=new TH1F("h_Mej_allBkg","h_Mej_allBkg",Nbins,xmin,xmax);
h_Mej_allBkg->Sumw2();
h_Mej_allBkg->Add(h_Mej_zjet);
h_Mej_allBkg->Add(h_Mej_ttbar);
h_Mej_allBkg->Add(h_Mej_wjet);
h_Mej_allBkg->Add(h_Mej_ww);
h_Mej_allBkg->Add(h_Mej_wz);
h_Mej_allBkg->Add(h_Mej_zz);
h_Mej_allBkg->Add(h_Mej_qcdjet);
h_Mej_allBkg->Add(h_Mej_gammajet);



//## Mej var
RooRealVar Mej("Mej","Mej",0.,1000.,"GeV");


//## Create histograms and pdf in RooFit format
RooDataHist RDH_data("data","data",Mej,h_Mej_allBkg);
RooHistPdf Pdf_data("dataMej","dataMej" ,RooArgList(Mej),RDH_data); 


//## Crystall Ball to model the background
RooRealVar m0("m0","m0",100,50.,150.);
RooRealVar sigma("sigma","sigma",30,0.,60);
RooRealVar alpha("alpha","alpha",-0.4,-100,0.);
RooRealVar n("n","n",10,0.,+100);
RooCBShape Pdf_bkg("bkgMej","bkgMej",Mej,m0,sigma,alpha,n);

RooRealVar N_bkg("N_bkg","N_bkg",100,0.,10000);
RooAddPdf *modelFit=new RooAddPdf("modelFit","Fit model",RooArgList(Pdf_bkg),RooArgList(N_bkg));



//1) ## Generate a toyMC sample and fit
RooDataSet *data = Pdf_data.generate(Mej,10000) ;
RooFitResult* fitresult = Pdf_bkg.fitTo(*data,Extended(),Minos(kFALSE),Save(kTRUE)) ;

// or

//2) ## Fit the bkg histogram with the fit model
//RooFitResult* fitresult = Pdf_bkg.fitTo(RDH_data,Extended(),Minos(kFALSE),Save(kTRUE)) ;



//## Plot histo data and fit result together
RooPlot* mesframe = Mej.frame(Name("Mejframe"),Title("Only bkg")) ;
data->plotOn(Mejframe) ;
//RDH_data.plotOn(Mejframe) ;
modelFit.plotOn(Mejframe) ;

Mejframe->Draw();

//## Print fit result
fitresult->Print(); 


// RooRealVar *m0_fit = (RooRealVar *)fitresult->floatParsFinal().find("m0");
// RooRealVar *sigma_fit = (RooRealVar *)fitresult->floatParsFinal().find("sigma");
// RooRealVar *alpha_fit = (RooRealVar *)fitresult->floatParsFinal().find("alpha");
// RooRealVar *n_fit = (RooRealVar *)fitresult->floatParsFinal().find("n");


}
