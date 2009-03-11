{
TFile file1("Mar10_LQ400/analysisClass_Optimization_plots.root");
  
TCanvas c1;

file1->cd();
TH1F Signal;
((TH1D*)gDirectory->Get("histo1D__LQtoUE_M250__Nevent"))->Copy(Signal); Signal.Sumw2();
//((TH1D*)gDirectory->Get("histo1D__LQtoUE_M400__Nevent"))->Copy(Signal); Signal.Sumw2();

TH1F Backgnd;
((TH1D*)gDirectory->Get("histo1D__ALLBKG__Nevent"))->Copy(Backgnd); Backgnd.Sumw2();
TH1F TTbar;
((TH1D*)gDirectory->Get("histo1D__TTbar__Nevent"))->Copy(TTbar); TTbar.Sumw2();
TH1F Zjet;
((TH1D*)gDirectory->Get("histo1D__Z__Nevent"))->Copy(Zjet); Zjet.Sumw2();

TH1D Signif ("Significance","Significance",10000000,0.0,10000000);

double maxSig = -1;
int cuts = -1;
for (int i=1;i<10000000;i++){
  double s = Signal.GetBinContent(i);
  double b = Backgnd.GetBinContent(i);
  double sig = 0;
  if ((s+b)>0) sig = s / sqrt(s+b);
  //double sig = s / sqrt(b);
  Signif.SetBinContent(i,sig);
  if (sig>maxSig) {
    maxSig = sig;
    cuts = i-1;  //bin #s are shifted from opt value by 1 since bin 0 is underflow
  }
  if (i==432+1) { //bins are shifted by opt value by 1
    double ttbar = TTbar.GetBinContent(i);
    double zjet = Zjet.GetBinContent(i);
    cout << "Signal at " << i-1 << ": "<< s << "\t" << "Background: " << b << "\t" << "Signif: " << sig << "\t";
    cout << "TTbar: " << ttbar << "\t" << "Zjet: " << zjet <<  endl;
  }
//   if (i==2244412+1) { //bins are shifted by opt value by 1
//     double ttbar = TTbar.GetBinContent(i);
//     double zjet = Zjet.GetBinContent(i);
//     cout << "Signal at " << i-1 << ": "<< s << "\t" << "Background: " << b << "\t" << "Signif: " << sig << "\t";
//     cout << "TTbar: " << ttbar << "\t" << "Zjet: " << zjet <<  endl;
//   }
//   if (i==2244413+1) { //bins are shifted by opt value by 1
//     double ttbar = TTbar.GetBinContent(i);
//     double zjet = Zjet.GetBinContent(i);
//     cout << "Signal at " << i-1 << ": "<< s << "\t" << "Background: " << b << "\t" << "Signif: " << sig << "\t";
//     cout << "TTbar: " << ttbar << "\t" << "Zjet: " << zjet <<  endl;
//   }
 }

Signif->Draw();
std::cout << "Significance: " << maxSig << "\t" << "Cuts: " << cuts << std::endl;
std::cout << "Sig: " << Signal.GetBinContent(cuts) << "\t" << "Bknd: " << Backgnd.GetBinContent(cuts) << std::endl;
 
c1->Print("Signif.gif","gif");

TCanvas c2;
c2->cd();

TH2D * ele1pt_ele2pt = new TH2D("","",10,0,10,10,0,10);

int ele1ptBin = 0;
int ele2ptBin = 0;
double MaxSig = 0;
for (int j=1;j<10000000;j++){
  ele1ptBin = j / 100000 + 1;
  ele2ptBin = (j / 10000)%10 + 1;
  double z = Signif.GetBinContent(j);
  if (z>MaxSig) MaxSig=z;
  if (j%10000==0) MaxSig=0;
  //ele1pt_ele2pt.SetBinContent(ele1ptBin,ele2ptBin,z);
  ele1pt_ele2pt.SetBinContent(ele1ptBin,ele2ptBin,MaxSig);
}
gStyle->SetPalette(1);
//ele1pt_ele2pt->Draw("LEGO2");
ele1pt_ele2pt->Draw("SURF3");
ele1pt_ele2pt->GetXaxis().SetTitle("ele1ptBin");
ele1pt_ele2pt->GetYaxis().SetTitle("ele2ptBin");
c2->Print("ele1ele2.gif","gif");

TCanvas c3;
c3->cd();

TH2D * ele1pt_jet1pt = new TH2D("","",10,0,10,10,0,10);

ele1ptBin = 0;
int jet1ptBin = 0;
double MaxSig_1 = 0;
double MaxSig_2 = 0;
double MaxSig_3 = 0;
double MaxSig_4 = 0;
double MaxSig_5 = 0;
double MaxSig_6 = 0;
double MaxSig_7 = 0;
double MaxSig_8 = 0;
double MaxSig_9 = 0;
double MaxSig_10 = 0;
for (int j=1;j<10000000;j++){
  ele1ptBin = j / 1000000 + 1;
  jet1ptBin = (j / 10000)%10 + 1;
  double z = Signif.GetBinContent(j);
  if ((jet1ptBin==1)&&(MaxSig_1<z)){
    MaxSig_1=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_1);
  }
  if ((jet1ptBin==2)&&(MaxSig_2<z)){
    MaxSig_2=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_2);
  }
  if ((jet1ptBin==3)&&(MaxSig_3<z)){
    MaxSig_3=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_3);
  }
  if ((jet1ptBin==4)&&(MaxSig_4<z)){
    MaxSig_4=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_4);
  }
  if ((jet1ptBin==5)&&(MaxSig_5<z)){
    MaxSig_5=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_5);
  }
  if ((jet1ptBin==6)&&(MaxSig_6<z)){
    MaxSig_6=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_6);
  }
  if ((jet1ptBin==7)&&(MaxSig_7<z)){
    MaxSig_7=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_7);
  }
  if ((jet1ptBin==8)&&(MaxSig_8<z)){
    MaxSig_8=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_8);
  }
  if ((jet1ptBin==9)&&(MaxSig_9<z)){
    MaxSig_9=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_9);
  }
  if ((jet1ptBin==10)&&(MaxSig_10<z)){
    MaxSig_10=z;
  ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_10);
  }
  if (j%1000000==0){
   MaxSig_1 = 0;
   MaxSig_2 = 0;
   MaxSig_3 = 0;
   MaxSig_4 = 0;
   MaxSig_5 = 0;
   MaxSig_6 = 0;
   MaxSig_7 = 0;
   MaxSig_8 = 0;
   MaxSig_9 = 0;
   MaxSig_10 = 0;
  }
}
gStyle->SetPalette(1);
//ele1pt_jet1pt->Draw("LEGO2");
ele1pt_jet1pt->Draw("SURF3");
ele1pt_jet1pt->GetXaxis().SetTitle("ele1ptBin");
ele1pt_jet1pt->GetYaxis().SetTitle("jet1ptBin");
c3->Print("ele1jet1.gif","gif");

TCanvas c4;
c4->cd();

TH2D * ele1pt_sT = new TH2D("","",10,0,10,10,0,10);
TH1D * sT = new TH1D("","",10,0,10);

ele1ptBin = 0;
int StBin = 0;
double MaxSig_1 = 0;
double MaxSig_2 = 0;
double MaxSig_3 = 0;
double MaxSig_4 = 0;
double MaxSig_5 = 0;
double MaxSig_6 = 0;
double MaxSig_7 = 0;
double MaxSig_8 = 0;
double MaxSig_9 = 0;
double MaxSig_10 = 0;
for (int j=1;j<10000000;j++){
  ele1ptBin = j / 1000000 + 1;
  StBin = (j / 100)%10 + 1;
  //cout << StBin << "\t" << j << endl;
  double z = Signif.GetBinContent(j);
  if ((StBin==1)&&(MaxSig_1<z)){
    MaxSig_1=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_1);
  }
  if ((StBin==2)&&(MaxSig_2<z)){
    MaxSig_2=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_2);
  }
  if ((StBin==3)&&(MaxSig_3<z)){
    MaxSig_3=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_3);
  }
  if ((StBin==4)&&(MaxSig_4<z)){
    MaxSig_4=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_4);
  }
  if ((StBin==5)&&(MaxSig_5<z)){
    MaxSig_5=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_5);
  }
  if ((StBin==6)&&(MaxSig_6<z)){
    MaxSig_6=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_6);
  }
  if ((StBin==7)&&(MaxSig_7<z)){
    MaxSig_7=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_7);
  }
  if ((StBin==8)&&(MaxSig_8<z)){
    MaxSig_8=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_8);
  }
  if ((StBin==9)&&(MaxSig_9<z)){
    MaxSig_9=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_9);
  }
  if ((StBin==10)&&(MaxSig_10<z)){
    MaxSig_10=z;
  ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_10);
  }
  if (j%1000000==0){  //moving to next ele1ptBin, reset sT max values
   MaxSig_1 = 0;
   MaxSig_2 = 0;
   MaxSig_3 = 0;
   MaxSig_4 = 0;
   MaxSig_5 = 0;
   MaxSig_6 = 0;
   MaxSig_7 = 0;
   MaxSig_8 = 0;
   MaxSig_9 = 0;
   MaxSig_10 = 0;
  }
}
sT->SetBinContent(1,MaxSig_1);
sT->SetBinContent(2,MaxSig_2);
sT->SetBinContent(3,MaxSig_3);
sT->SetBinContent(4,MaxSig_4);
sT->SetBinContent(5,MaxSig_5);
sT->SetBinContent(6,MaxSig_6);
sT->SetBinContent(7,MaxSig_7);
sT->SetBinContent(8,MaxSig_8);
sT->SetBinContent(9,MaxSig_9);
sT->SetBinContent(10,MaxSig_10);

gStyle->SetPalette(1);
//ele1pt_sT->Draw("LEGO2");
ele1pt_sT->Draw("SURF3");
ele1pt_sT->GetXaxis().SetTitle("ele1ptBin");
ele1pt_sT->GetYaxis().SetTitle("StBin");
//sT->Draw("hist");
//sT->GetXaxis().SetTitle("sT Bin");
c4->Print("sT.gif","gif");


}


