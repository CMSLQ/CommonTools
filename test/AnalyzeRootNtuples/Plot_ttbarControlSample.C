{
gROOT->Reset();

TFile f("ttbar.root");

TCanvas c1;
c1.SetLogy();

h_Mej_best_Meecut_Stcut->SetStats(0);
h_Mej_best_Meecut_Stcut->Rebin(2);
h_Memuj_best_Memucut_Stcut->Rebin(2);

h_Mej_best_Meecut_Stcut->GetXaxis()->SetTitle("M(lj) (GeV)");
h_Mej_best_Meecut_Stcut->GetYaxis()->SetTitle("a.u.");
h_Mej_best_Meecut_Stcut->SetMarkerStyle(20);
h_Mej_best_Meecut_Stcut->SetMarkerSize(0.7);
h_Mej_best_Meecut_Stcut->DrawNormalized();

h_Memuj_best_Memucut_Stcut->SetFillColor(kRed);
h_Memuj_best_Memucut_Stcut->SetFillStyle(3004);
h_Memuj_best_Memucut_Stcut->DrawNormalized("sameHISTE");

int N_eejj=h_Mej_best_Meecut_Stcut->Integral();
int N_emujj=h_Memuj_best_Memucut_Stcut->Integral();


char title_eejj[200];
sprintf(title_eejj,"eejj (%d entries in 100 pb^{-1})",N_eejj);

char title_emujj[200];
sprintf(title_emujj,"e\\mujj (%d entries in 100 pb^{-1})",N_emujj);

TLegend *legend = new TLegend(0.456897,0.641949,0.859195,0.851695);
legend->SetFillColor(kWhite);

legend->AddEntry(h_Mej_best_Meecut_Stcut,&title_eejj[0],"p");
legend->AddEntry(h_Memuj_best_Memucut_Stcut,&title_emujj[],"f");

legend->Draw();

TLatex *tInfo = new TLatex();
tInfo->SetNDC();
tInfo->SetTextSize(0.04);
tInfo->DrawLatex( 0.70546, 0.565678, "ttbar events");

}
