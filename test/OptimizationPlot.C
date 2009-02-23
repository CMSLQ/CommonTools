{
TFile file1("Feb23_6var/analysisClass_Optimization_plots.root");
//TFile file2("Feb20_Bkgnd/analysisClass_Optimization___TTJets-madgraph__Fall08_IDEAL_V9_v1__GEN-SIM-RECO.root");
  
TCanvas c1;

c1->SetGridy();

file1->cd();
TH1F Signal;
((TH1D*)gDirectory->Get("histo1D__LQtoUE_M250__Nevent"))->Copy(Signal); Signal.Sumw2();

//file2->cd();
TH1F Backgnd;
((TH1D*)gDirectory->Get("histo1D__ALLBKG__Nevent"))->Copy(Backgnd); Backgnd.Sumw2();

TH1F Signif ("","",1000001,-0.5,1000000);

double maxSig = -1;
int cuts = -1;
for (int i=1;i<1000000;i++){
  double s = Signal.GetBinContent(i);
  double b = Backgnd.GetBinContent(i);
  //double b = 100;
  double sig = s / sqrt(s+b);
  //double sig = s / b;
  Signif.SetBinContent(i,sig);
  if (sig>maxSig) {
    maxSig = sig;
    cuts = i-1;  //bin #s are shifted from opt value by 1 since bin 0 is underflow
  }
}

Signif->Draw();
std::cout << "Significance: " << maxSig << "\t" << "Cuts: " << cuts << endl;
std::cout << "Sig: " << Signal.GetBinContent(cuts) << "\t" << "Bknd: " << Backgnd.GetBinContent(cuts) << endl;
 
c1->Print("Signif.ps");
}


