{
gROOT->Reset();

TFile f("zjet.root");

TCanvas c1;
c1.SetLogy();

h_Mej_best_Meecut_Stcut->SetStats(0);
h_Mej_best_Meecut_Stcut->Rebin(4);
h_Mej_best_MeecutInside_Stcut->Rebin(4);

h_Mej_best_Meecut_Stcut->GetXaxis()->SetTitle("M(ej) (GeV)");
h_Mej_best_Meecut_Stcut->GetYaxis()->SetTitle("a.u.");
h_Mej_best_Meecut_Stcut->SetMarkerStyle(20);
h_Mej_best_Meecut_Stcut->SetMarkerSize(0.7);
h_Mej_best_Meecut_Stcut->DrawNormalized();

h_Mej_best_MeecutInside_Stcut->SetFillColor(kRed);
h_Mej_best_MeecutInside_Stcut->SetFillStyle(3004);
h_Mej_best_MeecutInside_Stcut->DrawNormalized("sameHISTE");

int N_eejj_out=h_Mej_best_Meecut_Stcut->Integral();
int N_eejj_in=h_Mej_best_MeecutInside_Stcut->Integral();


char title_eejj_out[200];
sprintf(title_eejj_out,"eejj - M_{ee} > 100 GeV (%d entries in 100 pb^{-1})",N_eejj_out);

char title_eejj_in[200];
sprintf(title_eejj_in,"eejj - 80 GeV < M_{ee} < 100 GeV (%d entries in 100 pb^{-1})",N_eejj_in);

TLegend *legend = new TLegend(0.452586,0.612288,0.875,0.851695);
legend->SetFillColor(kWhite);

legend->AddEntry(h_Mej_best_Meecut_Stcut,&title_eejj_out[0],"p");
legend->AddEntry(h_Mej_best_MeecutInside_Stcut,&title_eejj_in[0],"f");

legend->Draw();

TLatex *tInfo = new TLatex();
tInfo->SetNDC();
tInfo->SetTextSize(0.04);
tInfo->DrawLatex( 0.704023, 0.536017, "z/\\gamma+jet events");

}
