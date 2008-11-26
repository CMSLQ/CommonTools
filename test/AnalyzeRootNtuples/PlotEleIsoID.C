{

gROOT->Reset();

TFile FileTrueEle("EleIsoID_trueEle_LQ.root");
TFile FileFakeEle("EleIsoID_fakeEle_QCD.root");

//eleHoE
TCanvas c1;
c1.SetLogy();

TLegend *legend1 = new TLegend(0.48,0.59,0.86,0.85);
legend1->SetFillColor(kWhite);

FileTrueEle.cd();

h_eleHoE_matchLQ->SetStats(0);
h_eleHoE_matchLQ->SetLineColor(kBlue);
h_eleHoE_matchLQ->SetLineWidth(2);
h_eleHoE_matchLQ->GetXaxis()->SetTitle("H/E");
h_eleHoE_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleHoE_matchLQ->DrawNormalized("HISTE");

legend1->AddEntry(h_eleHoE_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleHoE_NOmatchLQ->SetFillStyle(3003);
h_eleHoE_NOmatchLQ->SetFillColor(kRed);
h_eleHoE_NOmatchLQ->DrawNormalized("sameHISTE");

legend1->AddEntry(h_eleHoE_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend1->Draw();

c1.SaveAs("h_eleHoE.eps");

//h_eleSigmaEE
TCanvas c2;
c2.SetLogy();

TLegend *legend2 = new TLegend(0.48,0.59,0.86,0.85);
legend2->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleSigmaEE_matchLQ->SetStats(0);
h_eleSigmaEE_matchLQ->SetLineColor(kBlue);
h_eleSigmaEE_matchLQ->SetLineWidth(2);
h_eleSigmaEE_matchLQ->GetXaxis()->SetTitle("\\sigma_{\\eta\\eta}");
h_eleSigmaEE_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleSigmaEE_matchLQ->DrawNormalized("HISTE");

legend2->AddEntry(h_eleSigmaEE_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleSigmaEE_NOmatchLQ->SetFillStyle(3003);
h_eleSigmaEE_NOmatchLQ->SetFillColor(kRed);
h_eleSigmaEE_NOmatchLQ->DrawNormalized("sameHISTE");

legend2->AddEntry(h_eleSigmaEE_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend2->Draw();

c2.SaveAs("h_eleSigmaEE.eps");

//h_eleDeltaPhiTrkSC_matchLQ
TCanvas c3;
c3.SetLogy();

TLegend *legend3 = new TLegend(0.48,0.59,0.86,0.85);
legend3->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleDeltaPhiTrkSC_matchLQ->SetStats(0);
h_eleDeltaPhiTrkSC_matchLQ->SetLineColor(kBlue);
h_eleDeltaPhiTrkSC_matchLQ->SetLineWidth(2);
//h_eleDeltaPhiTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\phi Trk-SC @ ECAL");
h_eleDeltaPhiTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\phi_{in}");
h_eleDeltaPhiTrkSC_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleDeltaPhiTrkSC_matchLQ->GetXaxis()->SetRangeUser(-0.1,0.15);
h_eleDeltaPhiTrkSC_matchLQ->DrawNormalized("HISTE");

legend3->AddEntry(h_eleDeltaPhiTrkSC_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleDeltaPhiTrkSC_NOmatchLQ->SetFillStyle(3003);
h_eleDeltaPhiTrkSC_NOmatchLQ->SetFillColor(kRed);
h_eleDeltaPhiTrkSC_NOmatchLQ->DrawNormalized("sameHISTE");

legend3->AddEntry(h_eleDeltaPhiTrkSC_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend3->Draw();

c3.SaveAs("h_eleDeltaPhiTrkSC.eps");

//h_eleDeltaEtaTrkSC_matchLQ
TCanvas c4;
c4.SetLogy();

TLegend *legend4 = new TLegend(0.48,0.59,0.86,0.85);
legend4->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleDeltaEtaTrkSC_matchLQ->SetStats(0);
h_eleDeltaEtaTrkSC_matchLQ->SetLineColor(kBlue);
h_eleDeltaEtaTrkSC_matchLQ->SetLineWidth(2);
//h_eleDeltaEtaTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\eta Trk-SC @ ECAL");
h_eleDeltaEtaTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\eta_{in}");
h_eleDeltaEtaTrkSC_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleDeltaEtaTrkSC_matchLQ->GetXaxis()->SetRangeUser(-0.1,0.15);
h_eleDeltaEtaTrkSC_matchLQ->DrawNormalized("HISTE");

legend4->AddEntry(h_eleDeltaEtaTrkSC_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleDeltaEtaTrkSC_NOmatchLQ->SetFillStyle(3003);
h_eleDeltaEtaTrkSC_NOmatchLQ->SetFillColor(kRed);
h_eleDeltaEtaTrkSC_NOmatchLQ->DrawNormalized("sameHISTE");

legend4->AddEntry(h_eleDeltaEtaTrkSC_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend4->Draw();

c4.SaveAs("h_eleDeltaEtaTrkSC.eps");

//h_eleNumTrkIso_NOmatchLQ
TCanvas c5;
//c5.SetLogy();

TLegend *legend5 = new TLegend(0.48,0.59,0.86,0.85);
legend5->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleNumTrkIso_matchLQ->SetStats(0);
h_eleNumTrkIso_matchLQ->SetLineColor(kBlue);
h_eleNumTrkIso_matchLQ->SetLineWidth(2);
//h_eleNumTrkIso_matchLQ->GetXaxis()->SetTitle("NumTrkIso");
h_eleNumTrkIso_matchLQ->GetXaxis()->SetTitle("number of tracks");
h_eleNumTrkIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleNumTrkIso_matchLQ->DrawNormalized("HISTE");

legend5->AddEntry(h_eleNumTrkIso_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleNumTrkIso_NOmatchLQ->SetFillStyle(3003);
h_eleNumTrkIso_NOmatchLQ->SetFillColor(kRed);
h_eleNumTrkIso_NOmatchLQ->DrawNormalized("sameHISTE");

legend5->AddEntry(h_eleNumTrkIso_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend5->Draw();

c5.SaveAs("h_eleNumTrkIso.eps");

//h_eleTrkIso_matchLQ_matchLQ
TCanvas c6;
c6.SetLogy();

TLegend *legend6 = new TLegend(0.48,0.59,0.86,0.85);
legend6->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleTrkIso_matchLQ->SetStats(0);
h_eleTrkIso_matchLQ->SetLineColor(kBlue);
h_eleTrkIso_matchLQ->SetLineWidth(2);
h_eleTrkIso_matchLQ->GetXaxis()->SetTitle("TrkIso");
h_eleTrkIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleTrkIso_matchLQ->DrawNormalized("HISTE");

legend6->AddEntry(h_eleTrkIso_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleTrkIso_NOmatchLQ->SetFillStyle(3003);
h_eleTrkIso_NOmatchLQ->SetFillColor(kRed);
h_eleTrkIso_NOmatchLQ->DrawNormalized("sameHISTE");

legend6->AddEntry(h_eleTrkIso_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend6->Draw();

c6.SaveAs("h_eleTrkIso.eps");

//h_eleEcalIso_matchLQ_matchLQ
TCanvas c7;
c7.SetLogy();

TLegend *legend7 = new TLegend(0.48,0.59,0.86,0.85);
legend7->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleEcalIso_matchLQ->SetStats(0);
h_eleEcalIso_matchLQ->SetLineColor(kBlue);
h_eleEcalIso_matchLQ->SetLineWidth(2);
h_eleEcalIso_matchLQ->GetXaxis()->SetTitle("EcalIso");
h_eleEcalIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleEcalIso_matchLQ->DrawNormalized("HISTE");

legend7->AddEntry(h_eleEcalIso_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleEcalIso_NOmatchLQ->SetFillStyle(3003);
h_eleEcalIso_NOmatchLQ->SetFillColor(kRed);
h_eleEcalIso_NOmatchLQ->DrawNormalized("sameHISTE");

legend7->AddEntry(h_eleEcalIso_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend7->Draw();

c7.SaveAs("h_eleEcalIso.eps");


//h_eleTrkIso_matchLQ_matchLQ
TCanvas c8;
c8.SetLogy();

TLegend *legend8 = new TLegend(0.48,0.59,0.86,0.85);
legend8->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleTrkIsoAbs_matchLQ->SetStats(0);
h_eleTrkIsoAbs_matchLQ->SetLineColor(kBlue);
h_eleTrkIsoAbs_matchLQ->SetLineWidth(2);
//h_eleTrkIsoAbs_matchLQ->GetXaxis()->SetTitle("TrkIsoAbs");
h_eleTrkIsoAbs_matchLQ->GetXaxis()->SetTitle("track isolation (GeV)");
h_eleTrkIsoAbs_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleTrkIsoAbs_matchLQ->DrawNormalized("HISTE");

legend8->AddEntry(h_eleTrkIsoAbs_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleTrkIsoAbs_NOmatchLQ->SetFillStyle(3003);
h_eleTrkIsoAbs_NOmatchLQ->SetFillColor(kRed);
h_eleTrkIsoAbs_NOmatchLQ->DrawNormalized("sameHISTE");

legend8->AddEntry(h_eleTrkIsoAbs_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend8->Draw();

c8.SaveAs("h_eleTrkIsoAbs.eps");

//h_eleEcalIsoAbs_matchLQ_matchLQ
TCanvas c9;
c9.SetLogy();

TLegend *legend9 = new TLegend(0.48,0.59,0.86,0.85);
legend9->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleEcalIsoAbs_matchLQ->SetStats(0);
h_eleEcalIsoAbs_matchLQ->SetLineColor(kBlue);
h_eleEcalIsoAbs_matchLQ->SetLineWidth(2);
//h_eleEcalIsoAbs_matchLQ->GetXaxis()->SetTitle("EcalIsoAbs");
h_eleEcalIsoAbs_matchLQ->GetXaxis()->SetTitle("e.m. isolation (GeV)");
h_eleEcalIsoAbs_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleEcalIsoAbs_matchLQ->DrawNormalized("HISTE");

legend9->AddEntry(h_eleEcalIsoAbs_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleEcalIsoAbs_NOmatchLQ->SetFillStyle(3003);
h_eleEcalIsoAbs_NOmatchLQ->SetFillColor(kRed);
h_eleEcalIsoAbs_NOmatchLQ->DrawNormalized("sameHISTE");

legend9->AddEntry(h_eleEcalIsoAbs_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend9->Draw();

c9.SaveAs("h_eleEcalIsoAbs.eps");


//h_eleHcalIsoAbs_matchLQ_matchLQ
TCanvas c10;
c10.SetLogy();

TLegend *legend10 = new TLegend(0.48,0.59,0.86,0.85);
legend10->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleHcalIsoAbs_matchLQ->SetStats(0);
h_eleHcalIsoAbs_matchLQ->SetLineColor(kBlue);
h_eleHcalIsoAbs_matchLQ->SetLineWidth(2);
//h_eleHcalIsoAbs_matchLQ->GetXaxis()->SetTitle("HcalIsoAbs");
h_eleHcalIsoAbs_matchLQ->GetXaxis()->SetTitle("hadronic isolation (GeV)");
h_eleHcalIsoAbs_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleHcalIsoAbs_matchLQ->DrawNormalized("HISTE");

legend10->AddEntry(h_eleHcalIsoAbs_matchLQ,"e (LQ events)","f");

FileFakeEle.cd();
h_eleHcalIsoAbs_NOmatchLQ->SetFillStyle(3003);
h_eleHcalIsoAbs_NOmatchLQ->SetFillColor(kRed);
h_eleHcalIsoAbs_NOmatchLQ->DrawNormalized("sameHISTE");

legend10->AddEntry(h_eleHcalIsoAbs_NOmatchLQ,"jet faking e (QCD multi-jet events)","f");

legend10->Draw();

c10.SaveAs("h_eleHcalIsoAbs.eps");


}
