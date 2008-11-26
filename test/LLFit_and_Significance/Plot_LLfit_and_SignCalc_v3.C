{

  gROOT->Reset();

  TFile inputFile("LLfit_and_SignCalc_M250.root");
  //TFile inputFile("LLfit_and_SignCalc_M400.root");
  //TFile inputFile("LLfit_and_SignCalc_M650.root");


  inputFile->ls();


  const int N_point=6;
  const float max_Lint=1000;
  const float max_Sign=20;

  //----------------------------------------------------

  //## Significance vs Lint
  TCanvas csign;

  //set axis
  g_SignVSLint->GetYaxis()->SetLimits(0,max_Sign);
  g_SignVSLint->GetYaxis()->SetRangeUser(0,max_Sign);
  g_SignVSLint->GetYaxis()->SetTitle("significance");

  g_SignVSLint->GetXaxis()->SetLimits(0,max_Lint);
  g_SignVSLint->GetXaxis()->SetRangeUser(0,max_Lint);
  g_SignVSLint->GetXaxis()->SetTitle("integrated luminosity (pb^{-1})");

  //draw graph
  g_SignVSLint->Draw("ap");

  //fit function  
  TF1 f_sqrtLint ("f_sqrtLint","[0]*sqrt(x)", 0, max_Lint) ;
  f_sqrtLint->SetParameter(0,1.);
  g_SignVSLint->Fit("f_sqrtLint","R");

  //Draw 5 significance line
  TLine line(0, 5, max_Lint, 5);
  line.Draw();

  //## Pull

  TCanvas cmean;
  TCanvas crms;

  TGraph *g_pullLQ_mean;
  TGraph *g_pullLQ_rms;
  TGraph *g_pullBkg_mean;
  TGraph *g_pullBkg_rms;

  float Npoint[N_point];
  float Lint[N_point];

  float pullLQ_mean[N_point];
  float pullBkg_mean[N_point];

  float pullLQ_rms[N_point];
  float pullBkg_rms[N_point];

  char histonameLQ[200];
  char histonameBkg[200];

  char histonameLint[200];

  for(int point=0;point<N_point;point++)
    {

      
      sprintf(histonameLint,"h_Lint_OneExp_%d",point);
      sprintf(histonameLQ,"h_NLQ_pull_%d",point);
      sprintf(histonameBkg,"h_Nbkg_pull_%d",point);

      TH1F *h_Lint=(TH1F*) inputFile->Get(histonameLint);  
      TH1F *h_pullLQ=(TH1F*) inputFile->Get(histonameLQ);  
      TH1F *h_pullBkg=(TH1F*) inputFile->Get(histonameBkg);  


      Npoint[point]=point;
      Lint[point]=h_Lint->GetMean();

      pullLQ_mean[point]=h_pullLQ->GetMean();
      pullLQ_rms[point]=h_pullLQ->GetRMS();

      pullBkg_mean[point]=h_pullBkg->GetMean();
      pullBkg_rms[point]=h_pullBkg->GetRMS();


    }


  g_pullLQ_mean = new TGraph(N_point,Lint,pullLQ_mean);
  g_pullLQ_rms = new TGraph(N_point,Lint,pullLQ_rms);

  g_pullBkg_mean = new TGraph(N_point,Lint,pullBkg_mean);
  g_pullBkg_rms = new TGraph(N_point,Lint,pullBkg_rms);


  //set axis 
  g_pullLQ_mean->GetYaxis()->SetLimits(-5,5);
  g_pullLQ_mean->GetYaxis()->SetRangeUser(-5,5);
  g_pullLQ_mean->GetYaxis()->SetTitle("Mean( N_{fit} - N_{expect} / \\sigma_{fit})");

  g_pullLQ_mean->GetXaxis()->SetTitle("integrate luminosity (pb-1)");

  g_pullLQ_rms->GetYaxis()->SetLimits(0,2);
  g_pullLQ_rms->GetYaxis()->SetRangeUser(0,2);
  g_pullLQ_rms->GetYaxis()->SetTitle("RMS( N_{fit} - N_{expect} / \\sigma_{fit})");

  g_pullLQ_rms->GetXaxis()->SetTitle("integrate luminosity (pb-1)");

  //set style and colors
  g_pullLQ_mean->SetMarkerColor(kBlue);
  g_pullBkg_mean->SetMarkerColor(kGreen);

  g_pullLQ_rms->SetMarkerColor(kBlue);
  g_pullBkg_rms->SetMarkerColor(kGreen);

  g_pullLQ_mean->SetMarkerStyle(20);
  g_pullBkg_mean->SetMarkerStyle(21);

  g_pullLQ_rms->SetMarkerStyle(20);
  g_pullBkg_rms->SetMarkerStyle(21);



  //draw
  cmean.cd();

  g_pullLQ_mean->Draw("ap");
  g_pullBkg_mean->Draw("psame");

  TLegend *legend = new TLegend(0.655172,0.684322,0.864943,0.860169);
  legend->SetFillColor(kWhite);
  legend->AddEntry(g_pullLQ_mean,"LQ","p");
  legend->AddEntry(g_pullBkg_mean,"All bkg","p");
  legend->Draw();

  crms.cd();

  g_pullLQ_rms->Draw("ap");
  g_pullBkg_rms->Draw("psame");

  TLegend *legend1 = new TLegend(0.655172,0.684322,0.864943,0.860169);
  legend1->SetFillColor(kWhite);
  legend1->AddEntry(g_pullLQ_rms,"LQ","p");
  legend1->AddEntry(g_pullBkg_rms,"All bkg","p");
  legend1->Draw();

  

}
