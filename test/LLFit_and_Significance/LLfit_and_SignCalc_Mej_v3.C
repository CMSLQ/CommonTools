{

  gROOT->Reset();
  gStyle->SetErrorX(0);

  gSystem->Load("libRooFit");
  using namespace RooFit;
  using namespace std;

  //----------------------------------------------------------- 
  //----------------------------------------------------------- 
  // Modify only this part

  //## Integrated luminosity 
  float intLumi=0.1; //(fb-1) is the integrated luminosity used 
  // to rescale the Mej histograms below


  //## Mej variable 
  float Mej_min=0.;
  float Mej_max=1000.;

  RooRealVar Mej("Mej","Mej",Mej_min,Mej_max,"GeV");


  //## "Data" 
  //TFile f_LQ("lq1stgen_M250_full167_100pb.root");
  //TFile f_LQ("lq1stgen_M400_full167_100pb.root");
  TFile f_LQ("lq1stgen_M650_full167_100pb.root");
  //TFile f_LQ("lq1stgen_M1000_full167_100pb.root");
  TFile f_ttbar("ttbar.root");
  TFile f_zjet("zjet.root");
  TFile f_wjet("wjet.root");
  TFile f_ww("ww.root");
  TFile f_zz("zz.root");
  TFile f_wz("wz.root");
  TFile f_qcdjet("qcdjet.root");
  TFile f_gammajet("gammajet.root");

  // name of histogram to fit
  char histoname[200];
  sprintf(histoname,"h_Mej_best_Meecut_Stcut");
  //sprintf(histoname,"h_Mej_allComb_Meecut_Stcut");

  // rebin "data" histogram
  int rebin=5;

  // number of combination N_comb per event 
  float N_comb=2; //for Mej best
  //float N_comb=4  //for Mej allComb



  //## Fit model for signal

  //TFile f_LQ_fit("lq1stgen_M250_full167_100pb.root");
  //TFile f_LQ_fit("lq1stgen_M400_full167_100pb.root");
  TFile f_LQ_fit("lq1stgen_M650_full167_100pb.root");
  //TFile f_LQ_fit("lq1stgen_M1000_full167_100pb.root");

  // rebin signal histogram
  int rebin_signal=2;


  //## Fit model for background

  //ttbar control sample
  TFile f_ttbar_cs("ttbar.root"); 

  char histoname_ttbar_cs[200];
  //sprintf(histoname_ttbar_cs,"h_Memuj_allComb_Memucut_Stcut");
  sprintf(histoname_ttbar_cs,"h_Memuj_best_Memucut_Stcut");

  //z+jet control sample
  TFile f_zjet_cs("zjet.root");

  char histoname_zjet_cs[200];
  //sprintf(histoname_zjet_cs,"h_Mej_allComb_MeecutInside_Stcut");
  sprintf(histoname_zjet_cs,"h_Mej_best_MeecutInside_Stcut");

  // rebin control sample histogram
  int rebin_ttbar_cs=2;
  int rebin_zjet_cs=2;

  //for MLQ=1000 GeV
  //int rebin_ttbar_cs=4;
  //int rebin_zjet_cs=4;


  // ratio of ttbar/zjet in the signal sample and systematic error
  
  int WhichR=+1;


  //## Output file
  //TFile outputfile("LLfit_and_SignCalc_M250_Rdown.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M400_Rdown.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M650_Rdown.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M1000_Rdown.root","RECREATE");

  //TFile outputfile("LLfit_and_SignCalc_M250.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M400.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M650.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M1000.root","RECREATE");

  //TFile outputfile("LLfit_and_SignCalc_M250_Rup.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M400_Rup.root","RECREATE");
  TFile outputfile("LLfit_and_SignCalc_M650_Rup.root","RECREATE");
  //TFile outputfile("LLfit_and_SignCalc_M1000_Rup.root","RECREATE");


  outputfile.cd();


  //## Numbers of pesudo-experiments
  int N_exp=500; 


  //## Points for the LL fit and significance calculation 

//   const int N_point = 1;
//   int Nev_LQ_expect[N_point] = {50};
    
  //M(LQ) 250 GeV
//   const int N_point = 6;
//   int Nev_LQ_expect[N_point] = {10,15,20,30,50,200};

  //M(LQ) 400 GeV
//   const int N_point = 6;
//   int Nev_LQ_expect[N_point] = {5,10,15,20,50,100};

  //M(LQ) 650 GeV
  const int N_point = 6;
  int Nev_LQ_expect[N_point] = {5,8,10,15,20,30};


  //## Fit options
  string fitOption="EMRT";


  //----------------------------------------------------------- 
  //----------------------------------------------------------- 

  //## Declaration of variables 

  //calculated using Nev_LQ_expect and sigmaXeff
  float Nev_ttbar_expect[N_point]; 
  float Nev_zjet_expect[N_point]; 
  float Nev_wjet_expect[N_point]; 
  float Nev_qcdjet_expect[N_point]; 
  float Nev_gammajet_expect[N_point]; 
  float Nev_ww_expect[N_point]; 
  float Nev_wz_expect[N_point]; 
  float Nev_zz_expect[N_point]; 
  float Nev_all_expect[N_point];
  float Nev_bkg_expect[N_point];

  float Nev_ttbar_cs_expect[N_point]; 
  float Nev_zjet_cs_expect[N_point]; 


  //significance
  float Significance[N_point];
  float e_Significance[N_point];

  //Lint
  float Lint[N_point];
  float e_Lint[N_point];


  //## Get histo
  TH1F *h_Mej_LQ=(TH1F*) f_LQ->Get(histoname);  
  TH1F *h_Mej_ttbar=(TH1F*) f_ttbar->Get(histoname);  
  TH1F *h_Mej_zjet=(TH1F*) f_zjet->Get(histoname);  
  TH1F *h_Mej_wjet=(TH1F*) f_wjet->Get(histoname);  
  TH1F *h_Mej_ww=(TH1F*) f_ww->Get(histoname);  
  TH1F *h_Mej_wz=(TH1F*) f_wz->Get(histoname);  
  TH1F *h_Mej_zz=(TH1F*) f_zz->Get(histoname);  
  TH1F *h_Mej_qcdjet=(TH1F*) f_qcdjet->Get(histoname);  
  TH1F *h_Mej_gammajet=(TH1F*) f_gammajet->Get(histoname);  

  TH1F *h_Mej_LQ_fit=(TH1F*) f_LQ_fit->Get(histoname);  

  TH1F *h_Mej_ttbar_cs=(TH1F*) f_ttbar_cs->Get(histoname_ttbar_cs);  
  TH1F *h_Mej_zjet_cs=(TH1F*) f_zjet_cs->Get(histoname_zjet_cs);  

  //## CrossSection x efficiencies (fb)
  //N = Lint * sigma * eff 
  float xsectionXeff_LQ = h_Mej_LQ->Integral()/(intLumi*N_comb);
  float xsectionXeff_ttbar = h_Mej_ttbar->Integral()/(intLumi*N_comb);
  float xsectionXeff_zjet = h_Mej_zjet->Integral()/(intLumi*N_comb);
  float xsectionXeff_wjet = h_Mej_wjet->Integral()/(intLumi*N_comb);
  float xsectionXeff_ww = h_Mej_ww->Integral()/(intLumi*N_comb);
  float xsectionXeff_wz = h_Mej_wz->Integral()/(intLumi*N_comb);
  float xsectionXeff_zz = h_Mej_zz->Integral()/(intLumi*N_comb);
  float xsectionXeff_qcdjet = h_Mej_qcdjet->Integral()/(intLumi*N_comb);
  float xsectionXeff_gammajet = h_Mej_gammajet->Integral()/(intLumi*N_comb);

  float xsectionXeff_ttbar_cs = h_Mej_ttbar_cs->Integral()/(intLumi*N_comb);
  float xsectionXeff_zjet_cs = h_Mej_zjet_cs->Integral()/(intLumi*N_comb);

  //## Rebin histo
  h_Mej_LQ->Rebin(rebin);
  h_Mej_ttbar->Rebin(rebin);
  h_Mej_zjet->Rebin(rebin);
  h_Mej_wjet->Rebin(rebin);
  h_Mej_ww->Rebin(rebin);
  h_Mej_wz->Rebin(rebin);
  h_Mej_zz->Rebin(rebin);
  h_Mej_qcdjet->Rebin(rebin);
  h_Mej_gammajet->Rebin(rebin);

  h_Mej_ttbar_cs->Rebin(rebin_ttbar_cs);
  h_Mej_zjet_cs->Rebin(rebin_zjet_cs);

  h_Mej_LQ_fit->Rebin(rebin_signal);

  //## Create stack histograms
  int Nbins = h_Mej_LQ->GetXaxis()->GetNbins();
  float xmin = h_Mej_LQ->GetXaxis()->GetXmin();
  float xmax = h_Mej_LQ->GetXaxis()->GetXmax();
  cout << "Nbins: " << Nbins << endl;
  cout << "xmin: " << xmin << endl;
  cout << "xmax: " << xmax << endl;

  TH1F *h_Mej_data=new TH1F("h_Mej_data","h_Mej_data",Nbins,xmin,xmax);
  h_Mej_data->Sumw2();
  h_Mej_data->SetStats(0);
  h_Mej_data->Add(h_Mej_LQ);
  h_Mej_data->Add(h_Mej_zjet);
  h_Mej_data->Add(h_Mej_ttbar);
  h_Mej_data->Add(h_Mej_wjet);
  h_Mej_data->Add(h_Mej_ww);
  h_Mej_data->Add(h_Mej_wz);
  h_Mej_data->Add(h_Mej_zz);
  h_Mej_data->Add(h_Mej_qcdjet);
  h_Mej_data->Add(h_Mej_gammajet);

  h_Mej_data->Write(); 

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

  h_Mej_allBkg->Write(); 


  //## LLfit and significance calculation

  for(int point=0;point<N_point;point++)
    {

      //## declaration of plots

      //plots for all experiments
      TH1F *h_Ntot = new TH1F("h_Ntot","",10000,-100.,10000.);
      char h_nameNtot[100];
      sprintf(h_nameNtot,"h_Ntot_%d",point);
      h_Ntot->SetName(h_nameNtot);
      
      TH1F *h_NLQ = new TH1F("h_NLQ","",10000,-100.,10000.);
      char h_nameNLQ[100];
      sprintf(h_nameNLQ,"h_NLQ_%d",point);
      h_NLQ->SetName(h_nameNLQ);
      
      TH1F *h_Nbkg = new TH1F("h_Nbkg","",10000,-100.,10000.);
      char h_nameNbkg[100];
      sprintf(h_nameNbkg,"h_Nbkg_%d",point);
      h_Nbkg->SetName(h_nameNbkg);

      TH1F *h_NLQ_pull = new TH1F("h_NLQ_pull","",50,-5.,5.);
      char h_nameNLQ_pull[100];
      sprintf(h_nameNLQ_pull,"h_NLQ_pull_%d",point);
      h_NLQ_pull->SetName(h_nameNLQ_pull);

      TH1F *h_Nbkg_pull = new TH1F("h_Nbkg_pull","",50,-5.,5.);
      char h_nameNbkg_pull[100];
      sprintf(h_nameNbkg_pull,"h_Nbkg_pull_%d",point);
      h_Nbkg_pull->SetName(h_nameNbkg_pull);

      TH1F *h_covQual = new TH1F("h_covQual","",6,-0.5,5.5);
      char h_namecovQual[100];
      sprintf(h_namecovQual,"h_covQual_%d",point);
      h_covQual->SetName(h_namecovQual);

      TH1F *h_minNll_s_plus_b = new TH1F("h_minNll_s_plus_b","",10000,-10000.,10000);
      char h_nameminNll_s_plus_b[100];
      sprintf(h_nameminNll_s_plus_b,"h_minNll_s_plus_b_%d",point);
      h_minNll_s_plus_b->SetName(h_nameminNll_s_plus_b);

      TH1F *h_minNll_b = new TH1F("h_minNll_b","",10000,-10000.,10000);
      char h_nameminNll_b[100];
      sprintf(h_nameminNll_b,"h_minNll_b_%d",point);
      h_minNll_b->SetName(h_nameminNll_b);

      TH1F *h_significance = new TH1F("h_significance","",200,0.,100);
      char h_namesignificance[100];
      sprintf(h_namesignificance,"h_significance_%d",point);
      h_significance->SetName(h_namesignificance);

      TH1F *h_NLQ_b = new TH1F("h_NLQ_b","",10000,-100.,10000.);
      char h_nameNLQ_b[100];
      sprintf(h_nameNLQ_b,"h_NLQ_b_%d",point);
      h_NLQ_b->SetName(h_nameNLQ_b);
      
      TH1F *h_Nbkg_b = new TH1F("h_Nbkg_b","",10000,-100.,10000.);
      char h_nameNbkg_b[100];
      sprintf(h_nameNbkg_b,"h_Nbkg_b_%d",point);
      h_Nbkg_b->SetName(h_nameNbkg_b);

      TH1F *h_covQual_b = new TH1F("h_covQual_b","",6,-0.5,5.5);
      char h_namecovQual_b[100];
      sprintf(h_namecovQual_b,"h_covQual_b_%d",point);
      h_covQual_b->SetName(h_namecovQual_b);


      //plots just for one experiment

      TH1F *h_NLQ_exp_OneExp = new TH1F("h_NLQ_exp_OneExp","",10000,-100.,10000.);
      char h_nameNLQ_exp_OneExp[100];
      sprintf(h_nameNLQ_exp_OneExp,"h_NLQ_exp_OneExp_%d",point);
      h_NLQ_exp_OneExp->SetName(h_nameNLQ_exp_OneExp);
      
      TH1F *h_Nbkg_exp_OneExp = new TH1F("h_Nbkg_exp_OneExp","",10000,-100.,10000.);
      char h_nameNbkg_exp_OneExp[100];
      sprintf(h_nameNbkg_exp_OneExp,"h_Nbkg_exp_OneExp_%d",point);
      h_Nbkg_exp_OneExp->SetName(h_nameNbkg_exp_OneExp);

      TH1F *h_Ntot_exp_OneExp = new TH1F("h_Ntot_exp_OneExp","",10000,-100.,10000.);
      char h_nameNtot_exp_OneExp[100];
      sprintf(h_nameNtot_exp_OneExp,"h_Ntot_exp_OneExp_%d",point);
      h_Ntot_exp_OneExp->SetName(h_nameNtot_exp_OneExp);

      TH1F *h_NLQ_fit_OneExp = new TH1F("h_NLQ_fit_OneExp","",10000,-100.,10000.);
      char h_nameNLQ_fit_OneExp[100];
      sprintf(h_nameNLQ_fit_OneExp,"h_NLQ_fit_OneExp_%d",point);
      h_NLQ_fit_OneExp->SetName(h_nameNLQ_fit_OneExp);
      
      TH1F *h_Nbkg_fit_OneExp = new TH1F("h_Nbkg_fit_OneExp","",10000,-100.,10000.);
      char h_nameNbkg_fit_OneExp[100];
      sprintf(h_nameNbkg_fit_OneExp,"h_Nbkg_fit_OneExp_%d",point);
      h_Nbkg_fit_OneExp->SetName(h_nameNbkg_fit_OneExp);

      TH1F *h_Ntot_fit_OneExp = new TH1F("h_Ntot_fit_OneExp","",10000,-100.,10000.);
      char h_nameNtot_fit_OneExp[100];
      sprintf(h_nameNtot_fit_OneExp,"h_Ntot_fit_OneExp_%d",point);
      h_Ntot_fit_OneExp->SetName(h_nameNtot_fit_OneExp);

      TH1F *h_Lint_OneExp = new TH1F("h_Lint_OneExp","",1000,0.,10000.);
      char h_nameLint_OneExp[100];
      sprintf(h_nameLint_OneExp,"h_Lint_OneExp_%d",point);
      h_Lint_OneExp->SetName(h_nameLint_OneExp);
      
      TH1F *h_significance_OneExp = new TH1F("h_significance_OneExp","",500,0.,100);
      char h_namesignificance_OneExp[100];
      sprintf(h_namesignificance_OneExp,"h_significance_OneExp_%d",point);
      h_significance_OneExp->SetName(h_namesignificance_OneExp);


      //## number of expected events for this point

      float currentLint = float(Nev_LQ_expect[point]/xsectionXeff_LQ);
      Lint[point] = currentLint*1000;
      e_Lint[point] = 0;

      Nev_ttbar_expect[point] = float(xsectionXeff_ttbar*currentLint);
      Nev_zjet_expect[point] = float(xsectionXeff_zjet*currentLint);
      Nev_wjet_expect[point] = float(xsectionXeff_wjet*currentLint);
      Nev_ww_expect[point] = float(xsectionXeff_ww*currentLint);
      Nev_wz_expect[point] = float(xsectionXeff_wz*currentLint);
      Nev_zz_expect[point] = float(xsectionXeff_zz*currentLint);
      Nev_qcdjet_expect[point] = float(xsectionXeff_qcdjet*currentLint);
      Nev_gammajet_expect[point] = float(xsectionXeff_gammajet*currentLint);

      Nev_ttbar_cs_expect[point] = float(xsectionXeff_ttbar_cs*currentLint);
      Nev_zjet_cs_expect[point] = float(xsectionXeff_zjet_cs*currentLint);

      Nev_bkg_expect[point] = Nev_zjet_expect[point] + Nev_ttbar_expect[point] 
	+ Nev_wjet_expect[point] + Nev_qcdjet_expect[point] 
	+ Nev_gammajet_expect[point] + Nev_zz_expect[point]  
	+ Nev_ww_expect[point] + Nev_wz_expect[point];  

      Nev_all_expect[point] = Nev_LQ_expect[point] + Nev_bkg_expect[point];

      //number of LQs
      float Num_LQ=Nev_LQ_expect[point]*N_comb;
      float Num_Bkg1=Nev_ttbar_expect[point]*N_comb;
      float Num_Bkg2=Nev_zjet_expect[point]*N_comb;
      float Num_AllBkg=Nev_bkg_expect[point]*N_comb;
      float Num_All=Nev_all_expect[point]*N_comb;
      float Num_ttbar_cs=Nev_ttbar_cs_expect[point]*N_comb;
      float Num_zjet_cs=Nev_zjet_cs_expect[point]*N_comb;

      //## Create histograms in Roofit format and PDF's

      //all data
      RooDataHist RDH_eejj_all("RDH_eejj_all","Mej eejj sample all",Mej,h_Mej_data);
      RooHistPdf Pdf_eejj_all("allMej","allMej" ,RooArgList(Mej),RDH_eejj_all);

      //pdf signal
      RooDataHist RDH_LQ("RDH_LQ","LQ signal",Mej,h_Mej_LQ_fit);
      RooHistPdf Pdf_LQ("LQMej","LQMej" ,RooArgList(Mej),RDH_LQ); 

      //pdf bkg (ttbar)
      RooDataHist RDH_bkg1("RDH_bkg1","ttbar",Mej,h_Mej_ttbar_cs);
      RooHistPdf Pdf_bkg1("Bkg1Mej","ttbar" ,RooArgList(Mej),RDH_bkg1); 

      //pdf bkg2 (z+jet)
      RooDataHist RDH_bkg2("RDH_bkg2","z+jet",Mej,h_Mej_zjet_cs);
      RooHistPdf Pdf_bkg2("Bkg2Mej","z+jet" ,RooArgList(Mej),RDH_bkg2); 


      //## Fit model


      //Ratios control samples
      //------------------------------
      float ratio_cs=Num_ttbar_cs/Num_zjet_cs;
      //------------------------------
      
      //relative error on ratioBkg 
      //is the same as 
      //relative error on ratio_cs

      float e_r_Num_ttbar_cs=sqrt(Num_ttbar_cs)/Num_ttbar_cs;
      float e_r_Num_zjet_cs=sqrt(Num_zjet_cs)/Num_zjet_cs;

      float e_r_ratio_cs=sqrt(e_r_Num_ttbar_cs*e_r_Num_ttbar_cs 
			      + e_r_Num_zjet_cs*e_r_Num_zjet_cs);

      //Ratios signal samples
      //------------------------------
      float ratioBkgdef=Num_Bkg1/Num_Bkg2;
      //float ratioBkg=MyRatioValue;
      float ratioBkg=0.;
      //------------------------------

      if(WhichR==-1)
	ratioBkg=ratioBkgdef-ratioBkgdef*e_r_ratio_cs;

      else
	if(WhichR==0)
	  ratioBkg=ratioBkgdef;

      else
	if(WhichR==+1)
	  ratioBkg=ratioBkgdef+ratioBkgdef*e_r_ratio_cs;

      //       RooRealVar N_bkg1("N_bkg1","N_bkg1", Num_Bkg2*ratioBkg);
      //       RooRealVar N_bkg2("N_bkg2","N_bkg2", Num_Bkg2);
      RooRealVar N_bkg1("N_bkg1","N_bkg1", ratioBkg/(1+ratioBkg) );
      RooRealVar N_bkg2("N_bkg2","N_bkg2", 1/(1+ratioBkg) );
      N_bkg1->setConstant(kTRUE);
      N_bkg2->setConstant(kTRUE);

      RooAddPdf *myAllBkg=new RooAddPdf("allBkg","allBkg",
					RooArgList(Pdf_bkg1,Pdf_bkg2),
					RooArgList(N_bkg1,N_bkg2));

      //signal + all bkg
      RooRealVar N_LQ("N_LQ","N_LQ", Num_LQ, 0., Num_All);
      RooRealVar N_AllBkg("N_AllBkg","N_AllBkg", Num_AllBkg, 0., Num_All);

      RooAddPdf *modelFit=new RooAddPdf("modelFit","Model Fit",
					RooArgList(*myAllBkg,Pdf_LQ),
					RooArgList(N_AllBkg,N_LQ));


      //## Make toy MC experiments

      //define toy MC
      string fitOption_mod = (fitOption+"r").c_str();
      RooMCStudy toyMC(Pdf_eejj_all,*modelFit,Mej,"",fitOption_mod.c_str());
      //RooMCStudy toyMC(Pdf_eejj_all,*modelFit,Mej,"",Extended(),Minos(kFALSE),Save(kTRUE));

      //generate and fit
      toyMC.generateAndFit(N_exp,int(Num_All),kTRUE);



      //## Loop over toy MC experiments
      float minNll_s_plus_b[N_exp];
      float minNll_b[N_exp];

      float covQual_s_plus_b[N_exp];
      float covQual_b[N_exp];

      vector<float> sign_vec;

      int OneGoodExpDone=false;
      int NextExp=0;

      for(int exp=0;exp<N_exp;exp++)
	{

	  cout << endl;
	  cout << "==========================" << endl;
	  cout << "Experiment: " << exp << endl;
	  cout << "==========================" << endl;
	  cout << endl;

	  //## Signal + background hyphothesis


	  //retrieve fit results
	  RooRealVar *N_LQFinal_toy = (RooRealVar*)toyMC.fitResult(exp)->floatParsFinal().find("N_LQ");
	  RooRealVar *N_bkgFinal_toy = (RooRealVar*)toyMC.fitResult(exp)->floatParsFinal().find("N_AllBkg");

	  //fit quality
	  covQual_s_plus_b[exp]=toyMC.fitResult(exp)->covQual();
	  minNll_s_plus_b[exp] = toyMC.fitResult(exp)->minNll();
	  float pull_LQ=( N_LQFinal_toy->getVal() - Num_LQ ) / N_LQFinal_toy->getError();
	  float pull_bkg=( N_bkgFinal_toy->getVal() - Num_AllBkg ) / N_bkgFinal_toy->getError();

	  //fill histograms
	  h_covQual->Fill(covQual_s_plus_b[exp]);
	  
	  if(covQual_s_plus_b[exp]==3)
	    {
	      h_Ntot->Fill( (N_LQFinal_toy->getVal() + N_bkgFinal_toy->getVal()) / N_comb );
	      h_NLQ->Fill(N_LQFinal_toy->getVal() / N_comb);
	      h_Nbkg->Fill(N_bkgFinal_toy->getVal() / N_comb);
	      h_NLQ_pull->Fill( pull_LQ );
	      h_Nbkg_pull->Fill( pull_bkg );
	      h_minNll_s_plus_b->Fill(minNll_s_plus_b[exp]); 
	    }

	  //## Only background hypthothesis

	  //signal set to zero
	  N_LQ.setVal(0.);
	  N_LQ.setConstant(kTRUE);

	  //****************
	  //N_AllBkg.setVal(Num_AllBkg);
	  N_AllBkg.setVal(Num_All);
	  //N_AllBkg.setConstant(kTRUE);
	  //****************

	  // retrieve generated data for this experiment
	  RooAbsData *thisGenData = toyMC.genData(exp);


	  //fit data with only bkg hyphothesis
	  //****************
	  RooFitResult *OnlyBkgFitResult = (RooFitResult*) modelFit->fitTo(*thisGenData,fitOption_mod.c_str()); 
	  //****************
	  //RooFitResult *OnlyBkgFitResult 
	  //= (RooFitResult*) modelFit->fitTo(*thisGenData,Extended(),Minos(kFALSE),Save(kTRUE)); 


	  //retrieve fit results
	  //****************
	  RooRealVar *N_bkgFinal_toy1 = (RooRealVar *)OnlyBkgFitResult->floatParsFinal().find("N_AllBkg");
	  //****************


	  //fit quality
	  //****************
	  covQual_b[exp]=OnlyBkgFitResult->covQual();
	  minNll_b[exp] = OnlyBkgFitResult->minNll();
	  //covQual_b[exp]= toyMC.fitResult(exp)->covQual();
	  //minNll_b[exp] = toyMC.fitResult(exp)->minNll();
	  //****************


	  //fill histograms
	  h_covQual_b->Fill(covQual_b[exp]);

	  if(covQual_b[exp]==3)
	    {
	      h_NLQ_b->Fill(N_LQ.getVal() / N_comb);
	      //****************
	      h_Nbkg_b->Fill(N_bkgFinal_toy1->getVal() / N_comb);
	      //****************
	      h_minNll_b->Fill(minNll_b[exp]); 
	    }


	  //## Significance calculation for each experiment

	  //-------------------------------------------------------------------	  
	  float currentSign = sqrt(2*(-minNll_s_plus_b[exp] + minNll_b[exp] ));
	  //-------------------------------------------------------------------	  


	  if(covQual_s_plus_b[exp]==3 && covQual_b[exp]==3 
	     && currentSign>=0 && currentSign<10000)
	    {
	      sign_vec.push_back(currentSign);
	      h_significance->Fill(currentSign); 
	      
	      if(OneGoodExpDone==false 
		 && fabs(h_NLQ_pull->GetMean() - pull_LQ)<h_NLQ_pull->GetRMS() 
		 && fabs(h_Nbkg_pull->GetMean() - pull_bkg)<h_Nbkg_pull->GetRMS() 
		 )
		{
		  cout << "OneGoodExpDone: " << exp << endl;

		  //--------------------------------------
		  //Create plots for this experiment


		  //number of expected signal and background events
		  h_NLQ_exp_OneExp->Fill(Nev_LQ_expect[point]);
		  h_Nbkg_exp_OneExp->Fill(Nev_bkg_expect[point]);
		  h_Ntot_exp_OneExp->Fill(Nev_all_expect[point]);

		  h_NLQ_exp_OneExp->Write();		  
		  h_Nbkg_exp_OneExp->Write();
		  h_Ntot_exp_OneExp->Write();


		  //integrated luminosity for this experiment
		  h_Lint_OneExp->Fill(Lint[point]);
		  h_Lint_OneExp->Write();

		  h_significance_OneExp->Fill(currentSign);
		  h_significance_OneExp->Write();


		  //gen data
		  char h_nameMej_genData_OneExp[100];
		  sprintf(h_nameMej_genData_OneExp,"h_Mej_genData_OneExp_%d",point);

		  TH1F *h_Mej_genData_OneExp 
		    = (TH1F*) thisGenData->createHistogram("h_Mej_genData_OneExp",
							   Mej, Binning( int(float(Mej_max/50)) ) ); 
		                                           //Mej, Binning(Nbins) );

		  h_Mej_genData_OneExp->SetName(h_nameMej_genData_OneExp);
		  h_Mej_genData_OneExp->Write();


		  //fit results in S+B hyphothesis
		  h_NLQ_fit_OneExp->Fill(N_LQFinal_toy->getVal() / N_comb);
		  h_Nbkg_fit_OneExp->Fill(N_bkgFinal_toy->getVal() / N_comb);
		  h_Ntot_fit_OneExp->Fill((N_LQFinal_toy->getVal()+N_bkgFinal_toy->getVal()) / N_comb);

		  h_NLQ_fit_OneExp->Write();
		  h_Nbkg_fit_OneExp->Write();
		  h_Ntot_fit_OneExp->Write();

		  //###### TEMPORARY SOLUTION #######

		  //S+B
		  TCanvas c1;
		  N_LQ.setConstant(kFALSE);

		  N_LQ.setVal(N_LQFinal_toy->getVal());		  
		  N_AllBkg.setVal(N_bkgFinal_toy->getVal());		  


		  RooPlot* Mejframe1 = Mej.frame(Name("Mejframe1"),Title("sig + bkg")) ;
		  //thisGenData->plotOn(Mejframe1, Binning( int(float(Nbins)) ) ) ;
		  thisGenData->plotOn(Mejframe1, Binning( int(float(Mej_max/50)) ) ) ;
		  modelFit->plotOn(Mejframe1) ;
		  modelFit->plotOn(Mejframe1, Components(*myAllBkg), 
				   FillColor(kBlack), FillStyle(3001), DrawOption("F")) ;
 		  modelFit->plotOn(Mejframe1, Components(Pdf_LQ), 
 				   FillColor(kBlue), FillStyle(3002), DrawOption("F")) ;

		  
		  Mejframe1->Draw();
		  Mejframe1->Write();

		  c1.Write();
		  c1.Close();

		  //B
		  TCanvas c2;

		  N_LQ.setVal(0.);		
		  N_AllBkg.setVal(N_bkgFinal_toy1->getVal());		  

		  RooPlot* Mejframe2 = Mej.frame(Name("Mejframe2"),Title("only bkg")) ;
		  //thisGenData->plotOn(Mejframe2, Binning( int(float(Nbins)) ) ) ;
		  thisGenData->plotOn(Mejframe2, Binning( int(float(Mej_max/50)) ) ) ;
		  modelFit->plotOn(Mejframe2) ;
		  modelFit->plotOn(Mejframe2, Components(*myAllBkg), 
				   FillColor(kBlack), FillStyle(3001), DrawOption("F")) ;
		  
		  Mejframe2->Draw();
		  Mejframe2->Write();

		  c2.Write();
		  c2.Close();

		  //--------------------------------------
		  
		  // 		  NextExp++;

		  // 		  if(NextExp==2)
		  OneGoodExpDone=true;
		}
	      
	    }//end good fit



	}// end loop over experiments


      //## Calculate median of significance distribution
      const int N_goodExp = sign_vec.size();
      //       float sign_array[N_goodExp];
      //       for(int i=0;i<N_goodExp;i++)
      // 	{
      // 	  sign_array[i]=sign_vec[i];
      // 	  cout << i << endl;
      // 	  //cout << sign_array[i] << endl;
      // 	  cout << sign_vec[i] << endl;
      // 	}	  
      //Significance[point]=TMath::Median(N_goodExp,sign_array);

      //--------- IMPROVE IT -----------
      //???????????????????????????????
      
      Significance[point]=h_significance->GetMean();
      e_Significance[point]=h_significance->GetRMS()/sqrt(float(N_goodExp)); 
      //e_Significance[point]=Significance[point]*0.1; 

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


      //## Cout fit results for this luminosity (point)

      cout << "************************************" << endl;
      cout << "--- Point: " << point << endl;
      cout << "Lint["<<point<<"] (pb): " << Lint[point] << endl;
      cout << "************************************" << endl;
      cout << "Nev_all_expect["<<point<<"]: " << Nev_all_expect[point] << endl;
      cout << "Nev_LQ_expect["<<point<<"]: " << Nev_LQ_expect[point] << endl;
      cout << "Nev_bkg_expect["<<point<<"]: " << Nev_bkg_expect[point] << endl;
      cout << "************************************" << endl;
      cout << "Nev_ttbar_expect["<<point<<"]: " << Nev_ttbar_expect[point] << endl;
      cout << "Nev_zjet_expect["<<point<<"]: " << Nev_zjet_expect[point] << endl;
      cout << "Nev_wjet_expect["<<point<<"]: " << Nev_wjet_expect[point] << endl;
      cout << "Nev_ww_expect["<<point<<"]: " << Nev_ww_expect[point] << endl;
      cout << "Nev_wz_expect["<<point<<"]: " << Nev_wz_expect[point] << endl;
      cout << "Nev_zz_expect["<<point<<"]: " << Nev_zz_expect[point] << endl;
      cout << "Nev_qcdjet_expect["<<point<<"]: " << Nev_qcdjet_expect[point] << endl;
      cout << "Nev_gammajet_expect["<<point<<"]: " << Nev_gammajet_expect[point] << endl;
      cout << "Nev_ttbar_cs_expect["<<point<<"]: " << Nev_ttbar_cs_expect[point] << endl;
      cout << "Nev_zjet_cs_expect["<<point<<"]: " << Nev_zjet_cs_expect[point] << endl;
      cout << "************************************" << endl;
      cout << "N_goodExp: " << N_goodExp << endl; 
      cout << "Significance["<<point<<"]: " << Significance[point] << endl;
      cout << "e_Significance["<<point<<"]: " << e_Significance[point] << endl;
      cout << "************************************" << endl;


      //## Write histogram for this luminosity
      h_covQual->Write();
      h_Ntot->Write();
      h_NLQ->Write();
      h_Nbkg->Write();
      h_NLQ_pull->Write();
      h_Nbkg_pull->Write();
      h_minNll_s_plus_b->Write();

      h_NLQ_b->Write();
      h_Nbkg_b->Write();
      h_covQual_b->Write();
      h_minNll_b->Write();

      h_significance->Write();

    }


  //-----------------------------------------------------------------------------------------------


  //# Plot significance vs Lint for the current LQ mass

  TGraphErrors *g_sign_vs_Lint = 
    new TGraphErrors(N_point,Lint,Significance,e_Lint,e_Significance);
  g_sign_vs_Lint->SetName("g_SignVSLint");
  g_sign_vs_Lint->SetMarkerStyle(20);
  g_sign_vs_Lint->Write();


  outputfile.Close();


}
