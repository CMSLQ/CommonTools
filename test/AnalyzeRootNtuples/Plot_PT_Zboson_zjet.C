{
gROOT->Reset();

TFile f("zjet.root");

TCanvas c1;
//c1.SetLogy();

h_pThat_sel->SetStats(0);
h_pThat_sel->Rebin(4);
h_pThat_sel_MeecutInside->Rebin(4);

h_pThat_sel->GetXaxis()->SetTitle("gen P_{T} ee pair (GeV)");
h_pThat_sel->GetYaxis()->SetTitle("a.u.");
h_pThat_sel->GetXaxis()->SetRangeUser(0,600);
h_pThat_sel->SetMarkerStyle(20);
h_pThat_sel->SetMarkerSize(0.7);
h_pThat_sel->DrawNormalized();

h_pThat_sel_MeecutInside->SetFillColor(kRed);
h_pThat_sel_MeecutInside->SetFillStyle(3004);
h_pThat_sel_MeecutInside->DrawNormalized("sameHISTE");

int N_eejj_out=h_pThat_sel->Integral();
int N_eejj_in=h_pThat_sel_MeecutInside->Integral();


char title_eejj_out[200];
sprintf(title_eejj_out,"eejj - M_{ee} > 100 GeV");

char title_eejj_in[200];
sprintf(title_eejj_in,"eejj - 80 GeV < M_{ee} < 100 GeV");

TLegend *legend = new TLegend(0.452586,0.612288,0.875,0.851695);
legend->SetFillColor(kWhite);

legend->AddEntry(h_pThat_sel,&title_eejj_out[0],"p");
legend->AddEntry(h_pThat_sel_MeecutInside,&title_eejj_in[0],"f");

legend->Draw();

TLatex *tInfo = new TLatex();
tInfo->SetNDC();
tInfo->SetTextSize(0.04);
tInfo->DrawLatex( 0.704023, 0.536017, "z/\\gamma+jet events");

}
