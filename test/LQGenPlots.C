{

//TFile f("data/output/genStudies_LQ1stgen_M250.root");
TFile f("data/output/genStudies_LQ1stgen_M400.root");

f.ls();

//###########################################################################

TCanvas c1;
c1.SetLogy();

h_deltaRmin_genJet_ele->SetTitle("");

h_deltaRmin_genJet_ele->GetXaxis()->SetTitle("\\DeltaR_{min}=\\sqrt{\\Delta\\phi^{2}+\\Delta\\eta^{2}}");
h_deltaRmin_genJet_ele->GetXaxis()->SetRangeUser(0,2);
h_deltaRmin_genJet_ele->SetLineWidth(2);

h_deltaRMin_genJet_genJetNearestEle->SetLineColor(kRed);
h_deltaRMin_genJet_genJetNearestEle->SetLineWidth(2);

h_deltaRmin_genJet_ele->Draw();
h_deltaRMin_genJet_genJetNearestEle->Draw("same");

TLegend *legend1 = new TLegend(0.164706,0.616541,0.746218,0.869674);
legend1->SetFillColor(kWhite);
legend1->AddEntry(h_deltaRmin_genJet_ele,"genJet with minimum \\DeltaR from genEle","l");
legend1->AddEntry(h_deltaRMin_genJet_genJetNearestEle,"genJet with min \\DeltaR from genJetWithMinDeltaRFromEle","l");
legend1->Draw();


//###########################################################################

TCanvas c2;
c2.SetLogy();

h_deltaRmin_genJet_quark->SetTitle("");

h_deltaRmin_genJet_quark->GetXaxis()->SetTitle("\\DeltaR_{min}=\\sqrt{\\Delta\\phi^{2}+\\Delta\\eta^{2}}");
h_deltaRmin_genJet_quark->GetXaxis()->SetRangeUser(0,2);
h_deltaRmin_genJet_quark->SetLineWidth(2);

h_deltaRMin_genJet_genJetNearestQuark->SetLineColor(kRed);
h_deltaRMin_genJet_genJetNearestQuark->SetLineWidth(2);

h_deltaRmin_genJet_quark->Draw();
h_deltaRMin_genJet_genJetNearestQuark->Draw("same");

TLegend *legend2 = new TLegend(0.164706,0.656541,0.746218,0.869674);
legend2->SetFillColor(kWhite);
legend2->AddEntry(h_deltaRmin_genJet_quark,"genJet with minimum \\DeltaR from genQuark","l");
legend2->AddEntry(h_deltaRMin_genJet_genJetNearestQuark,"genJet with min \\DeltaR from genJetWithMinDeltaRFromQuark","l");
legend2->Draw();


//###########################################################################

TCanvas c3;
c3.SetLogy();

h_EgenJetOverEgenEle_MatchCands->SetTitle("");

h_EgenJetOverEgenEle_MatchCands->Rebin(2);
h_EgenJetOverEgenEle_ForDeltaRmin->Rebin(2);

h_EgenJetOverEgenEle_MatchCands->GetXaxis()->SetTitle("E_genJet / E_genEle");
h_EgenJetOverEgenEle_MatchCands->GetXaxis()->SetRangeUser(0,2);
h_EgenJetOverEgenEle_MatchCands->SetLineWidth(2);

h_EgenJetOverEgenEle_ForDeltaRmin->SetLineColor(kRed);
h_EgenJetOverEgenEle_ForDeltaRmin->SetLineWidth(2);

h_EgenJetOverEgenEle_MatchCands->Draw();
h_EgenJetOverEgenEle_ForDeltaRmin->Draw("same");

TLegend *legend3 = new TLegend(0.606723,0.496241,0.979832,0.684211);
legend3->SetFillColor(kWhite);
legend3->AddEntry(h_EgenJetOverEgenEle_ForDeltaRmin,"genJet with min DeltaR from genEle","l");
legend3->AddEntry(h_EgenJetOverEgenEle_MatchCands,"genJet matched (cut on \\DeltaR_{min})","l");
legend3->Draw();

//###########################################################################

TCanvas c4;
c4.SetLogy();

h_EgenJetOverEgenQuark_MatchCands->SetTitle("");

h_EgenJetOverEgenQuark_MatchCands->Rebin(2);
h_EgenJetOverEgenQuark_ForDeltaRmin->Rebin(2);

h_EgenJetOverEgenQuark_MatchCands->GetXaxis()->SetTitle("E_genJet / E_genQuark");
h_EgenJetOverEgenQuark_MatchCands->GetXaxis()->SetRangeUser(0,2);
h_EgenJetOverEgenQuark_MatchCands->SetLineWidth(2);

h_EgenJetOverEgenQuark_ForDeltaRmin->SetLineColor(kRed);
h_EgenJetOverEgenQuark_ForDeltaRmin->SetLineWidth(2);

h_EgenJetOverEgenQuark_MatchCands->Draw();
h_EgenJetOverEgenQuark_ForDeltaRmin->Draw("same");

TLegend *legend4 = new TLegend(0.606724,0.496241,0.979842,0.684211);
legend4->SetFillColor(kWhite);
legend4->AddEntry(h_EgenJetOverEgenQuark_ForDeltaRmin,"genJet with min DeltaR from genQuark","l");
legend4->AddEntry(h_EgenJetOverEgenQuark_MatchCands,"genJet matched (cut on \\DeltaR_{min})","l");
legend4->Draw();

//###########################################################################

TCanvas c5;

h_Pt1stGenJetNoMatch->SetTitle("");

h_Pt1stGenJetNoMatch->GetXaxis()->SetTitle("genJet Pt (GeV)");
h_Pt1stGenJetNoMatch->GetXaxis()->SetRangeUser(0,700);
h_Pt1stGenJetNoMatch->SetLineWidth(2);

h_Pt2ndGenJetQuarkMatch->SetLineColor(kBlue);
h_Pt2ndGenJetQuarkMatch->SetLineWidth(2);

h_Pt1stGenJetQuarkMatch->SetLineColor(kRed);
h_Pt1stGenJetQuarkMatch->SetLineWidth(2);

h_Pt1stGenJetNoMatch->Draw();
h_Pt2ndGenJetQuarkMatch->Draw("same");
h_Pt1stGenJetQuarkMatch->Draw("same");

TLegend *legend5 = new TLegend(0.22521,0.639098,0.860504,0.857143);
legend5->SetFillColor(kWhite);
legend5->AddEntry(h_Pt1stGenJetQuarkMatch,"1st (in Pt) genJet matched with QuarkFromLQ","l");
legend5->AddEntry(h_Pt2ndGenJetQuarkMatch,"2nd (in Pt) genJet matched with QuarkFromLQ","l");
legend5->AddEntry(h_Pt1stGenJetNoMatch,"1st (in Pt) genJet of the event recoil (not from LQ decay)","l");
legend5->Draw();


//###########################################################################

TCanvas c6;

h_LQmassAlgo_With3Jets->SetTitle("");

h_LQmassAlgo_With3Jets->Rebin(2);
h_LQmassAlgo_With2Jets->Rebin(2);

h_LQmassAlgo_With3Jets->GetXaxis()->SetTitle("M_{ej} (GeV)");
h_LQmassAlgo_With3Jets->GetXaxis()->SetRangeUser(0,1000);
h_LQmassAlgo_With3Jets->SetLineWidth(2);

h_LQmassAlgo_With3Jets->SetLineColor(kRed);
h_LQmassAlgo_With3Jets->SetLineWidth(2);

h_LQmassAlgo_With2Jets->SetLineColor(kBlue);
h_LQmassAlgo_With2Jets->SetLineWidth(2);

h_LQmassAlgo_With3Jets->Draw();
h_LQmassAlgo_With2Jets->DrawClone("same");

TLegend *legend6 = new TLegend(0.487395,0.711779,0.865546,0.854637);
legend6->SetFillColor(kWhite);
legend6->AddEntry(h_LQmassAlgo_With2Jets,"using 2 electrons and 2 jets","l");
legend6->AddEntry(h_LQmassAlgo_With3Jets,"using 2 electrons and 3 jets","l");
legend6->Draw();


//###########################################################################

TCanvas c7;
//c7.SetLogy();

h_LQmassAlgo2_With3Jets->Rebin(2);
//rebinning of h_LQmassAlgo_With3Jets already done above

h_LQmassAlgo2_With3Jets->SetTitle("");

h_LQmassAlgo2_With3Jets->GetXaxis()->SetTitle("M_{ej} (GeV)");
h_LQmassAlgo2_With3Jets->GetXaxis()->SetRangeUser(0,1000);
h_LQmassAlgo2_With3Jets->SetLineWidth(2);

h_LQmassAlgo2_With3Jets->SetLineColor(kBlack);
h_LQmassAlgo2_With3Jets->SetLineWidth(2);

h_LQmassAlgo_With3Jets->SetLineColor(kRed);
h_LQmassAlgo_With3Jets->SetLineWidth(2);

h_LQmassAlgo2_With3Jets->Draw();
h_LQmassAlgo_With3Jets->DrawClone("same");

TLegend *legend7 = new TLegend(0.381,0.704,0.874,0.862);
legend7->SetFillColor(kWhite);
legend7->AddEntry(h_LQmassAlgo_With3Jets,"using 2ele/3jets + minimum \\DeltaM","l");
legend7->AddEntry(h_LQmassAlgo2_With3Jets,"using 2ele/3jets + minimum \\DeltaM_{rel}","l");
legend7->Draw();


}
