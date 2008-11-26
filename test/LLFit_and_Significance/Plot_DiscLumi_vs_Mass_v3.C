{

  gROOT->Reset();

  const float max_Lint = 1700;
  const float max_Sign = 20;
  const float max_mass = 1000;

  vector<string> v_filename;

  v_filename.push_back("LLfit_and_SignCalc_M250.root");
  v_filename.push_back("LLfit_and_SignCalc_M400.root");
  v_filename.push_back("LLfit_and_SignCalc_M650.root");

  float mass[v_filename.size()];
  mass[0] = 250;
  mass[1] = 400;
  mass[2] = 650;

  //----------------------------------------------------

  float c0[v_filename.size()];

  for(int file=0;file<v_filename.size();file++)
    {

      TFile f(v_filename[file].c_str());
      //f.ls();
      f.cd();

      //fit function  
      TF1 f_sqrtLint ("f_sqrtLint","[0]*sqrt(x)", 0, max_Lint) ;
      f_sqrtLint->SetParameter(0,1.);
      g_SignVSLint->Fit("f_sqrtLint","R");

      c0[file]=f_sqrtLint->GetParameter(0);
      //cout << c0[file] << endl;

      f.Close();
    }

  TGraph *g_c0_vs_mass = new TGraph(v_filename.size(),mass,c0);
  g_c0_vs_mass->SetMarkerStyle(20);
  g_c0_vs_mass->Draw("ap");

  TF1 f_pol2 ("f_pol2","[0]*exp(-[1]*x)", 200, max_mass) ;
  //   f_pol2->SetParameter(0,100000000.);
  //   f_pol2->SetParameter(1,20000000.);
  //  g_c0_vs_mass->Fit("f_pol2","R");
  f_pol2->SetParameter(0,10.);
  f_pol2->SetParameter(1,0.01);
  g_c0_vs_mass->Fit("f_pol2","R");
  g_c0_vs_mass->GetXaxis()->SetLimits(200,max_mass);
  g_c0_vs_mass->GetXaxis()->SetRangeUser(200,max_mass);

  //c0 vs Mass
  float constant=f_pol2->GetParameter(0);
  float slope=f_pol2->GetParameter(1);

  cout << constant << endl;
  cout << slope << endl;


  //--
  TCanvas c1;
  c1.SetLogy();
  
  TF1 f_Ldisc ("f_Ldisc",
	       "(25/([0]*[0]))*exp(2*[1]*x)",
	       0,max_Lint);

  f_Ldisc.SetParameter(0,constant);
  f_Ldisc.SetParameter(1,slope);

  f_Ldisc.GetYaxis()->SetLimits(0.1,10000000);  
  f_Ldisc.GetYaxis()->SetRangeUser(0.1,10000000); 
  f_Ldisc.GetYaxis()->SetTitle("discovery luminosity (pb^{-1})");   
  //f_Ldisc.GetYaxis()->SetNdivisions(4);  

  float xmax=(1/(2*slope))*log(constant*constant*10000000/25);

  f_Ldisc.GetXaxis()->SetLimits(0.1,xmax);  
  f_Ldisc.GetXaxis()->SetRangeUser(0.1,xmax);  
  f_Ldisc.GetXaxis()->SetTitle("LQ mass (GeV)");  

  f_Ldisc.SetTitle("");
  f_Ldisc.Draw();


  Int_t n = 4;
  Double_t x[n] = {0,0,
		   (1/(2*slope))*log(constant*constant*10000000/25),
		   0};
  Double_t y[n] = {25/(constant*constant)
		   ,10000000
		   ,10000000
		   ,25/(constant*constant)};
  TPolyLine * pl = new TPolyLine(n,x,y,"F");
  pl->SetFillStyle(3004);   
  pl->SetFillColor(kGreen);   
  pl->Draw();


  Int_t n1 = 5;
  Double_t x1[n1] = {0,0,250,250,0};
  Double_t y1[n1] = {0.1,10000000,10000000,0.1,0.1};
  TPolyLine * p2 = new TPolyLine(n1,x1,y1,"F");
  p2->SetFillStyle(3002);   
  p2->SetFillColor(kBlack);   
  p2->Draw();


  gPad->RedrawAxis();
}
