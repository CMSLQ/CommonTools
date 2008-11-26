 void MejMassFits(TH1F* HistName) {

   // set up RooFit
   gSystem->Load("libRooFit");
   using namespace RooFit;
   
   gROOT->SetStyle("Plain");
   
   // get histogram and convert it to a RooFit understandble format
   TH1* hh = HistName->Clone("hh");
   hh->Rebin(1);
   RooRealVar Mej("Mej","Mej LQ",40.,1000,"GeV");
   RooDataHist RDH_data("RDH_data","dataset with Mej mass",Mej,hh);
   RooHistPdf Pdf_data("dataMej","dataMej" ,RooArgList(Mej),RDH_data); 

   //gen data accordingly to histo
   RooDataSet *data = Pdf_data.generate(Mej,5000) ;
   
   // make signal function
   //    RooRealVar mean("mean","mean",250,200,300);
   //    RooRealVar sigma("sigma","sigma",100,0.,200.);
   //    RooGaussian sig("sig","signal pdf",Mej,mean,sigma);
   
   RooRealVar mean("mean","mean",650,600.,700.);
   RooRealVar sigma("sigma","sigma",30,0.,100);
   RooRealVar alpha("alpha","alpha",0.1,0.,1.);
   RooRealVar n("n","n",0.1,0.,+100);
   RooCBShape sig("sig","signal pdf",Mej,mean,sigma,alpha,n);


   // make background function
   RooRealVar c0("c0","coeff #0",1.0,-5.,5.);
   RooRealVar c1("c1","coeff #1",0.1,-5.,5.);
   RooRealVar c2("c2","coeff #2",0.1,-5.,5.);
   RooRealVar c3("c3","coeff #3",0.1,-5.,5.); 
   RooChebychev bkg("bkg","bkgd pdf",Mej,RooArgList(c0,c1,c2,c3));

   // --- Build Argus background PDF ---
   //    RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,-1.) ;
   //    RooRealVar cutoff("cutoff","argus cutoff",1001,1000,2000) ;
   //    RooRealVar p("p","p",0.5,0,10);
   //    RooArgusBG bkg("argus","Argus PDF",Mej,cutoff,argpar,p);
   
   // we are interested in getting the number of signal/background events
   RooRealVar nsig("nsig","weel reco signal",500,0.,10000.);
   RooRealVar nbkg("nbkg","bad reco signal",500,0.,50000.);
   
   //define the model
   RooAddPdf model("model","signal+bkgd",RooArgList(sig,bkg),RooArgList(nsig,nbkg));
   
   //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
   RooFitResult *fitres = model.fitTo(*data,Extended(kTRUE),Minos(kFALSE),PrintLevel(0),Save(kTRUE));
   
   // make new canvas  
   TCanvas *TopMass = new TCanvas("TopMass","Top Mass");
   
   // make new frame and plot histogram and fit results on it 
   RooPlot *Massframe = Mej.frame();
   
   data.plotOn(Massframe,MarkerColor(kBlack));
   model.plotOn(Massframe,Components(sig),LineStyle(kDashed),LineColor(kRed));
   model.plotOn(Massframe,Components(bkg),LineStyle(kDashed),LineColor(kGreen));
   model.plotOn(Massframe,LineColor(kBlack));
   
   Massframe->Draw();
   
   // print the results of the fit   
   fitres->Print(); 
   
   // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
   cout<<" Fit chi square/dof = "<<Massframe->chiSquare(10)<<endl; 
   
 }


