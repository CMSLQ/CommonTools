void drawcompare1(char* namehisto1, char* namehisto2,TFile *file1, TCanvas *canv, bool logY, bool norm){

  TPad fPads1("pad1", "", 0.00, 0.00, 0.90, 0.99);   fPads1.Draw();
  TLegend *legend1 = new TLegend(0.56,0.68,0.86,0.85);
  legend1->SetFillColor(kWhite);

  fPads1.cd();
  if(logY) 
    fPads1.SetLogy();  //------------------------------------
  file1->cd();
  TH1F y1;
  ((TH1D*)gDirectory->Get(namehisto1))->Copy(y1); y1.Sumw2();
  y1.SetFillColor(kBlue);
  y1.SetFillStyle(3004);
  y1.Rebin(2);
  y1.Draw("hist");
  double inter1 = y1.Integral();

  TH1F y2;
  ((TH1D*)gDirectory->Get(namehisto2))->Copy(y2);   y2.Sumw2();
  double inter2 = y2.Integral();
  if(norm)
    y2.SetNormFactor(inter1);
  y2.SetMarkerStyle(8);
  y2.SetMarkerSize(.5);
  y2.Rebin(2);
  y2.DrawCopy("pesame");
  legend1->AddEntry("y1","80<Mee<100 GeV","f");
  legend1->AddEntry("y2","100<Mee GeV","p");
  legend1->Draw();

  cout << "inter 1 fast: " << inter1 << endl;
  cout << "inter 2 full: " << inter2 << endl;
  cout << "y1.Integral() fast: " << y1.Integral() << endl;
  cout << "y2.Integral() full: " << y2.Integral() << endl;
  cout << endl;

  canv->Print("comparison.ps");
//   string name = namehisto1;
//   name+=".gif";
//   const char * newName = name.c_str();
//   canv->Print(newName,"gif"); 
}


void comparison(char *inputfile1){

  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);  // Show overflow, underflow + SumOfWeights 
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();

  TCanvas *c0 = new TCanvas("c0","--c0--",0,0,1200,900);
  c0->Clear();
  TFile input1(inputfile1);
 
  c0->Print("comparison.ps["); 
  //c0->Print("comparison.gif[","gif"); 

  // rebinnings and reranging
  input1.cd(); 
  ((TH1D*)gDirectory->Get("Mej_inside"))->SetTitle("Mej (reco)");
  ((TH1D*)gDirectory->Get("Mee_inside"))->SetTitle("Mee (reco)");
  ((TH1D*)gDirectory->Get("pTee_gen_inside"))->SetTitle("pTee at Gen Level");

   drawcompare1("Mej_inside","Mej_above",&input1 , c0, true,true);
   drawcompare1("Mee_inside","Mee_above",&input1 , c0, true,false);
   drawcompare1("pTee_gen_inside","pTee_gen_above",&input1 , c0, true,true);
 
   c0->Print("comparison.ps]"); 
   //c0->Print("comparison.gif]","gif"); 
  
}
