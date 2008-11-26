{

  gROOT->Reset();
  gStyle->SetErrorX(0);

  gSystem->Load("libRooFit");
  using namespace RooFit;
  using namespace std;

  //----------------------------------------------------------- 

  //Read file
  //TFile f_LQ("lq1stgen_M250_full167_100pb.root");
  TFile f_LQ("lq1stgen_M650_full167_100pb.root");
  TFile f_ttbar("ttbar.root");
  TFile f_zjet("zjet.root");
  TFile f_wjet("wjet.root");
  TFile f_ww("ww.root");
  TFile f_zz("zz.root");
  TFile f_wz("wz.root");
  TFile f_qcdjet("qcdjet.root");
  TFile f_gammajet("gammajet.root");

  //Integrated luminosity 
  float intLumi=0.1; //(fb-1) is the integrated luminosity used 
                     // to rescale the Mej histograms above

  //output file
  //TFile outputfile("LLfit_and_SignCalc_M250.root","RECREATE");
  TFile outputfile("LLfit_and_SignCalc_M650.root","RECREATE");
  outputfile.cd();

  //Numbers of pesudo-experiments
  int N_exp=100; 

  //Points for the LL fit and significance calculation 
  //   const int N_point = 1;
  //   int Nev_LQ_expect[N_point] = {10};
    
  //   const int N_point = 3;
  //   int Nev_LQ_expect[N_point] = {5,15,25};

  const int N_point = 4;
  int Nev_LQ_expect[N_point] = {5,15,25,50};

  int rebin=5;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //calculated using Nev_LQ_expect and sigmaXeff
  int Nev_ttbar_expect[N_point]; 
  int Nev_zjet_expect[N_point]; 
  int Nev_wjet_expect[N_point]; 
  int Nev_qcdjet_expect[N_point]; 
  int Nev_gammajet_expect[N_point]; 
  int Nev_ww_expect[N_point]; 
  int Nev_wz_expect[N_point]; 
  int Nev_zz_expect[N_point]; 
  int Nev_all_expect[N_point];
  int Nev_bkg_expect[N_point];

  //significance
  float Significance[N_point];
  float e_Significance[N_point];

  //Lint
  float Lint[N_point];
  float e_Lint[N_point];

  //----------------------------------------------------------- 

  //Get histo
  TH1F *h_Mej_LQ=(TH1F*) f_LQ->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_ttbar=(TH1F*) f_ttbar->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_zjet=(TH1F*) f_zjet->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_wjet=(TH1F*) f_wjet->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_ww=(TH1F*) f_ww->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_wz=(TH1F*) f_wz->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_zz=(TH1F*) f_zz->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_qcdjet=(TH1F*) f_qcdjet->Get("h_Mej_best_Meecut_Stcut");  
  TH1F *h_Mej_gammajet=(TH1F*) f_gammajet->Get("h_Mej_best_Meecut_Stcut");  


  //CrossSection x efficiencies (fb)
  //N = Lint * sigma * eff 
  float xsectionXeff_LQ = h_Mej_LQ->Integral()/intLumi;
  float xsectionXeff_ttbar = h_Mej_ttbar->Integral()/intLumi;
  float xsectionXeff_zjet = h_Mej_zjet->Integral()/intLumi;
  float xsectionXeff_wjet = h_Mej_wjet->Integral()/intLumi;
  float xsectionXeff_ww = h_Mej_ww->Integral()/intLumi;
  float xsectionXeff_wz = h_Mej_wz->Integral()/intLumi;
  float xsectionXeff_zz = h_Mej_zz->Integral()/intLumi;
  float xsectionXeff_qcdjet = h_Mej_qcdjet->Integral()/intLumi;
  float xsectionXeff_gammajet = h_Mej_gammajet->Integral()/intLumi;

  //Rebin histo
  h_Mej_LQ->Rebin(rebin);
  h_Mej_ttbar->Rebin(rebin);
  h_Mej_zjet->Rebin(rebin);
  h_Mej_wjet->Rebin(rebin);
  h_Mej_ww->Rebin(rebin);
  h_Mej_wz->Rebin(rebin);
  h_Mej_zz->Rebin(rebin);
  h_Mej_qcdjet->Rebin(rebin);
  h_Mej_gammajet->Rebin(rebin);

  //Create stack histograms
  int Nbins = h_Mej_LQ->GetXaxis()->GetNbins();
  float xmin = h_Mej_LQ->GetXaxis()->GetXmin();
  float xmax = h_Mej_LQ->GetXaxis()->GetXmax();
  cout << "Nbins: " << Nbins << endl;
  cout << "xmin: " << xmin << endl;
  cout << "xmax: " << xmax << endl;

  //   //Mej
  //   Float_t Lower_Mej[9];
  //   Lower_Mej[0]=0;
  //   Lower_Mej[1]=20;
  //   Lower_Mej[2]=40;
  //   Lower_Mej[3]=60;
  //   Lower_Mej[4]=120;
  //   Lower_Mej[5]=180;
  //   Lower_Mej[6]=240;
  //   Lower_Mej[7]=400;
  //   Lower_Mej[8]=1000;
  // TH1F *h_Mej_data=new TH1F ("h_Mej_data","h_Mej_data",8,Lower_Mej);
  

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

  /*
  TH1F *h_Mej_zjet_ttbar_LQ=new TH1F("h_Mej_zjet_ttbar_LQ","h_Mej_zjet_ttbar_LQ",Nbins,xmin,xmax);
  h_Mej_zjet_ttbar_LQ->Sumw2();
  h_Mej_zjet_ttbar_LQ->Add(h_Mej_zjet);
  h_Mej_zjet_ttbar_LQ->Add(h_Mej_ttbar);
  h_Mej_zjet_ttbar_LQ->Add(h_Mej_LQ);

  TH1F *h_Mej_zjet_ttbar=new TH1F("h_Mej_zjet_ttbar","h_Mej_zjet_ttbar",Nbins,xmin,xmax);
  h_Mej_zjet_ttbar->Sumw2();
  h_Mej_zjet_ttbar->Add(h_Mej_zjet);
  h_Mej_zjet_ttbar->Add(h_Mej_ttbar);

  //Set histogram style
  int color_LQ=4;//kBlue
  h_Mej_zjet_ttbar_LQ->SetMarkerColor(color_LQ);
  h_Mej_zjet_ttbar_LQ->SetLineColor(color_LQ);
  h_Mej_zjet_ttbar_LQ->SetLineWidth(3);
  h_Mej_zjet_ttbar_LQ->SetFillColor(kWhite);
  h_Mej_zjet_ttbar_LQ->SetMarkerStyle(20);
  h_Mej_zjet_ttbar_LQ->SetMarkerSize(1);

  int color_ttbar=3;//kGreen
  h_Mej_zjet_ttbar->SetMarkerColor(color_ttbar);
  h_Mej_zjet_ttbar->SetLineColor(color_ttbar);
  h_Mej_zjet_ttbar->SetFillColor(color_ttbar);
  h_Mej_zjet_ttbar->SetLineWidth(2);
  h_Mej_zjet_ttbar->SetFillStyle(3003);

  int color_zjet=1;//kRed
  h_Mej_zjet->SetMarkerColor(color_zjet);
  h_Mej_zjet->SetLineColor(color_zjet);
  h_Mej_zjet->SetLineWidth(2);
  h_Mej_zjet->SetFillColor(color_zjet);
  h_Mej_zjet->SetFillStyle(3003);

  int color_data=1;//kBlack
  h_Mej_data->SetMarkerColor(color_data);
  h_Mej_data->SetMarkerStyle(20);
  h_Mej_data->SetMarkerSize(1);


  //##### Draw histo #####
  TCanvas c1;
  //   c1.SetLogy();
  h_Mej_data->Draw("HISTE");
  h_Mej_zjet_ttbar_LQ->Draw("sameHIST");
  h_Mej_zjet_ttbar->Draw("sameHIST");
  h_Mej_zjet->Draw("sameHIST");
  h_Mej_data->Draw("sameE1");

  //Draw titles
  h_Mej_data->SetTitle(0);
  h_Mej_data->GetXaxis()->SetTitle("Mej (GeV)");
  h_Mej_data->GetYaxis()->SetTitle("number of events / 100 pb^{-1}");

  c1.Write();
  c1.Close();
  */

  //LLfit and significance calculation
  string fitOption="EMRT";

  for(int point=0;point<N_point;point++)
    {

      //~~~~~~~~~~~~~~~~~~~~~~

      //## declaration of plots
      TH1F *h_Ntot = new TH1F("h_Ntot","",10000,-100.,10000.);
      char h_nameNtot[100];
      sprintf(h_nameNtot,"h_Ntot_%d",point);
      h_Ntot->SetName(h_nameNtot);
      
      TH1F *h_NLQ = new TH1F("h_NLQ","",1000,-100.,1000.);
      char h_nameNLQ[100];
      sprintf(h_nameNLQ,"h_NLQ_%d",point);
      h_NLQ->SetName(h_nameNLQ);
      
      TH1F *h_Nbkg = new TH1F("h_Nbkg","",10000,-100.,10000.);
      char h_nameNbkg[100];
      sprintf(h_nameNbkg,"h_Nbkg_%d",point);
      h_Nbkg->SetName(h_nameNbkg);

      TH1F *h_NLQ_pull = new TH1F("h_NLQ_pull","",50,-3.,3.);
      char h_nameNLQ_pull[100];
      sprintf(h_nameNLQ_pull,"h_NLQ_pull_%d",point);
      h_NLQ_pull->SetName(h_nameNLQ_pull);

      TH1F *h_Nbkg_pull = new TH1F("h_Nbkg_pull","",50,-3.,3.);
      char h_nameNbkg_pull[100];
      sprintf(h_nameNbkg_pull,"h_Nbkg_pull_%d",point);
      h_Nbkg_pull->SetName(h_nameNbkg_pull);

      TH1F *h_covQual = new TH1F("h_covQual","",11,-0.5,10.5);
      char h_namecovQual[100];
      sprintf(h_namecovQual,"h_covQual_%d",point);
      h_covQual->SetName(h_namecovQual);

      TH1F *h_minNll_s_plus_b = new TH1F("h_minNll_s_plus_b","",1000,-1000.,10000);
      char h_nameminNll_s_plus_b[100];
      sprintf(h_nameminNll_s_plus_b,"h_minNll_s_plus_b_%d",point);
      h_minNll_s_plus_b->SetName(h_nameminNll_s_plus_b);

      TH1F *h_minNll_b = new TH1F("h_minNll_b","",1000,-1000.,10000);
      char h_nameminNll_b[100];
      sprintf(h_nameminNll_b,"h_minNll_b_%d",point);
      h_minNll_b->SetName(h_nameminNll_b);

      TH1F *h_significance = new TH1F("h_significance","",60,0.,20);
      char h_namesignificance[100];
      sprintf(h_namesignificance,"h_significance_%d",point);
      h_significance->SetName(h_namesignificance);

      TH1F *h_NLQ_b = new TH1F("h_NLQ_b","",1000,-100.,1000.);
      char h_nameNLQ_b[100];
      sprintf(h_nameNLQ_b,"h_NLQ_b_%d",point);
      h_NLQ_b->SetName(h_nameNLQ_b);
      
      TH1F *h_Nbkg_b = new TH1F("h_Nbkg_b","",10000,-100.,10000.);
      char h_nameNbkg_b[100];
      sprintf(h_nameNbkg_b,"h_Nbkg_b_%d",point);
      h_Nbkg_b->SetName(h_nameNbkg_b);

      TH1F *h_covQual_b = new TH1F("h_covQual_b","",11,-0.5,10.5);
      char h_namecovQual_b[100];
      sprintf(h_namecovQual_b,"h_covQual_b_%d",point);
      h_covQual_b->SetName(h_namecovQual_b);



      //just for one experiment (see later)

      TH1F *h_NLQ_exp_OneExp = new TH1F("h_NLQ_exp_OneExp","",1000,-100.,1000.);
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

      TH1F *h_NLQ_fit_OneExp = new TH1F("h_NLQ_fit_OneExp","",1000,-100.,1000.);
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
      
      TH1F *h_significance_OneExp = new TH1F("h_significance_OneExp","",60,0.,20);
      char h_namesignificance_OneExp[100];
      sprintf(h_namesignificance_OneExp,"h_significance_OneExp_%d",point);
      h_significance_OneExp->SetName(h_namesignificance_OneExp);


      //~~~~~~~~~~~~~~~~~~~~~~


      //## number of expected events for this point

      float currentLint = float(Nev_LQ_expect[point]/xsectionXeff_LQ);
      Lint[point] = currentLint*1000;
      e_Lint[point] = 0;

      Nev_ttbar_expect[point] = int(xsectionXeff_ttbar*currentLint);
      Nev_zjet_expect[point] = int(xsectionXeff_zjet*currentLint);
      Nev_wjet_expect[point] = int(xsectionXeff_wjet*currentLint);
      Nev_ww_expect[point] = int(xsectionXeff_ww*currentLint);
      Nev_wz_expect[point] = int(xsectionXeff_wz*currentLint);
      Nev_zz_expect[point] = int(xsectionXeff_zz*currentLint);
      Nev_qcdjet_expect[point] = int(xsectionXeff_qcdjet*currentLint);
      Nev_gammajet_expect[point] = int(xsectionXeff_gammajet*currentLint);

      Nev_bkg_expect[point] = Nev_zjet_expect[point] + Nev_ttbar_expect[point] 
	+ Nev_wjet_expect[point] + Nev_qcdjet_expect[point] 
	+ Nev_gammajet_expect[point] + Nev_zz_expect[point]  
	+ Nev_ww_expect[point] + Nev_wz_expect[point];  

      Nev_all_expect[point] = Nev_LQ_expect[point] + Nev_bkg_expect[point];



      //## Create roofit hist and pdf for all events
      float Mej_min=0.;
      float Mej_max=1000.;

      RooRealVar Mej("Mej","Mej",Mej_min,Mej_max,"GeV");

      //** all
      RooDataHist RDH_eejj_all("RDH_eejj_all","Mej eejj sample all",Mej,h_Mej_data);
      RooHistPdf Pdf_eejj_all("allMej","allMej" ,RooArgList(Mej),RDH_eejj_all);

      //** LQ
      RooDataHist RDH_LQ("RDH_LQ","LQ signal",Mej,h_Mej_LQ);
      RooHistPdf Pdf_LQ("LQMej","LQMej" ,RooArgList(Mej),RDH_LQ); 
      
      //** background 
      RooDataHist RDH_bkg("RDH_bkg","BKG background",Mej,h_Mej_allBkg);
      RooHistPdf Pdf_bkg("bkgMej","bkgMej" ,RooArgList(Mej),RDH_bkg); 
      

      //## Toy MC experiments

      //# model PDF
      double minValue=0.;
      RooRealVar N_bkg("N_bkg","N_bkg",Nev_bkg_expect[point],minValue,Nev_all_expect[point]);
      RooRealVar N_LQ("N_LQ","N_LQ",Nev_LQ_expect[point],minValue,Nev_all_expect[point]);
      RooAddPdf *modelFit=new RooAddPdf("modelFit","Model Fit",RooArgList(Pdf_bkg,Pdf_LQ),
					RooArgList(N_bkg,N_LQ));

      //# Toy MC
      string fitOption_mod = (fitOption+"r").c_str();
      RooMCStudy toyMC(Pdf_eejj_all,*modelFit,Mej,"",fitOption_mod.c_str());
      toyMC.generateAndFit(N_exp,Nev_all_expect[point],kTRUE);


      //# Loop over toy MC experiments
      float minNll_s_plus_b[N_exp];
      float minNll_b[N_exp];

      float covQual_s_plus_b[N_exp];
      float covQual_b[N_exp];

      vector<float> sign_vec;

      int OneGoodExpDone=false;

      for(int exp=0;exp<N_exp;exp++)
	{

	  cout << endl;
	  cout << "==========================" << endl;
	  cout << "Experiment: " << exp << endl;
	  cout << "==========================" << endl;

	  //## Signal + background hyphothesis

	  RooRealVar *N_LQFinal_toy = (RooRealVar*)toyMC.fitResult(exp)->floatParsFinal().find("N_LQ");
	  RooRealVar *N_bkgFinal_toy = (RooRealVar*)toyMC.fitResult(exp)->floatParsFinal().find("N_bkg");

	  covQual_s_plus_b[exp]=toyMC.fitResult(exp)->covQual();

	  float pull_LQ=( N_LQFinal_toy->getVal() - Nev_LQ_expect[point] ) / N_LQFinal_toy->getError();
	  float pull_bkg=( N_bkgFinal_toy->getVal() - Nev_bkg_expect[point] ) / N_bkgFinal_toy->getError();

	  h_Ntot->Fill(N_LQFinal_toy->getVal() + N_bkgFinal_toy->getVal());
	  h_NLQ->Fill(N_LQFinal_toy->getVal());
	  h_Nbkg->Fill(N_bkgFinal_toy->getVal());

	  h_NLQ_pull->Fill( pull_LQ );
	  h_Nbkg_pull->Fill( pull_bkg );

	  h_covQual->Fill(covQual_s_plus_b[exp]);

	  minNll_s_plus_b[exp] = toyMC.fitResult(exp)->minNll();

	  h_minNll_s_plus_b->Fill(minNll_s_plus_b[exp]); 


	  //## Only background hypthothesis

	  N_LQ.setVal(0.);
	  N_LQ.setConstant(kTRUE);
	  //N_bkg.setVal(2*Nev_all_expect[point]);
	  N_bkg.setVal(10000);

	  RooAbsData *thisGenData = toyMC.genData(exp);
	  RooFitResult *OnlyBkgFitResult = (RooFitResult*) modelFit->fitTo(*thisGenData,fitOption_mod.c_str()); 

	  RooRealVar *N_bkgFinal_toy1 = (RooRealVar *)OnlyBkgFitResult->floatParsFinal().find("N_bkg");

	  covQual_b[exp]=OnlyBkgFitResult->covQual();
	  
	  h_NLQ_b->Fill(N_LQ.getVal());
	  h_Nbkg_b->Fill(N_bkgFinal_toy1->getVal());

	  minNll_b[exp] = OnlyBkgFitResult->minNll();
	  
	  h_minNll_b->Fill(minNll_b[exp]); 

	  h_covQual_b->Fill(covQual_b[exp]);


	  //## Significance calculation for each experiment
	  
	  float currentSign = sqrt(2*(-minNll_s_plus_b[exp] + minNll_b[exp] ));

	  if(covQual_s_plus_b[exp]==3 && covQual_b[exp]==3 
	     && currentSign>=0 && currentSign<10000)
	    {
	      sign_vec.push_back(currentSign);
	      h_significance->Fill(currentSign); 
	      
	      if(OneGoodExpDone==false)
		{
		  //cout << "OneGoodExpDone: " << exp << endl;

		  //--------------------------------------
		  //Create plot of Mej for this experiment

		  h_Nbkg_exp_OneExp->Fill(Nev_bkg_expect[point]);
		  h_Ntot_exp_OneExp->Fill(Nev_all_expect[point]);
		  h_NLQ_exp_OneExp->Fill(Nev_LQ_expect[point]);
		  
		  h_Nbkg_exp_OneExp->Write();
		  h_Ntot_exp_OneExp->Write();
		  h_NLQ_exp_OneExp->Write();

		  char h_nameMej_genData_OneExp[100];
		  sprintf(h_nameMej_genData_OneExp,"h_Mej_genData_OneExp_%d",point);

		  TH1F *h_Mej_genData_OneExp 
		    = (TH1F*) thisGenData->createHistogram("h_Mej_genData_OneExp", 
						   Mej, Binning(Nbins));

		  h_Mej_genData_OneExp->SetName(h_nameMej_genData_OneExp);
		  h_Mej_genData_OneExp->Write();


		  h_NLQ_fit_OneExp->Fill(N_LQFinal_toy->getVal());
		  h_Nbkg_fit_OneExp->Fill(N_bkgFinal_toy->getVal());
		  h_Ntot_fit_OneExp->Fill(N_LQFinal_toy->getVal()+N_bkgFinal_toy->getVal());

		  h_NLQ_fit_OneExp->Write();
		  h_Nbkg_fit_OneExp->Write();
		  h_Ntot_fit_OneExp->Write();

		  h_Lint_OneExp->Fill(Lint[point]);
		  h_Lint_OneExp->Write();

		  h_significance_OneExp->Fill(currentSign);
		  h_significance_OneExp->Write();


		  //--------------------------------------
		  
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
      
      Significance[point]=h_significance->GetMean();
      e_Significance[point]=Significance[point]*0.1; //???????????????????????????????

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


//Draw histo
// TCanvas c1;
//   c1.SetLogy();
// h_Mej_data->DrawClone("HISTE");
//h_Mej_bkg_ttbar_LQ->Draw("sameHIST");
//h_Mej_bkg_ttbar->Draw("sameHIST");
//h_Mej_bkg->Draw("sameHIST");
//h_Mej_data->Draw("sameE1");

//Draw titles
//h_Mej_data->SetTitle(0);
//h_Mej_data->GetXaxis()->SetTitle("Mej (GeV)");
//h_Mej_data->GetYaxis()->SetTitle("number of events / 100 pb^{-1}");

//   //Draw legend
//   TLegend *legend = new TLegend(0.602011,0.580508,0.857759,0.851695);
//   legend->SetFillColor(kWhite);
//   legend->AddEntry(h_Mej_data,"data","p");
//   legend->AddEntry(h_Mej_bkg_ttbar_LQ,"LQ","f");
//   legend->AddEntry(h_Mej_bkg_ttbar,"ttbar","f");
//   legend->AddEntry(h_Mej_bkg,"bkg","f");
//   legend->Draw();

//   //Axis range
//   float minY=0.01;
//   float maxY=h_Mej_data->GetBinContent(1)*1.3;
//   h_Mej_data->GetYaxis()->SetRangeUser(minY,maxY);

//   // Redraw axis
//   gPad->RedrawAxis();

}
