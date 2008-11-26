{

gROOT->Reset();

TFile f("LLfit_and_SignCalc_M650.root");
//TFile f("LLfit_and_SignCalc_M650.root");

//-----------------

TCanvas c1;
c1.SetLogy();

TLegend *legend = new TLegend(0.655172,0.684322,0.864943,0.860169);
legend->SetFillColor(kWhite);

//-----------------

//Normalization
float normFactor = float( h_Mej_genData_OneExp_3->Integral() ) ;
h_Mej_data->SetNormFactor( h_Ntot_fit_OneExp_3->GetMean() );
h_Mej_allBkg->SetNormFactor( h_Nbkg_fit_OneExp_3->GetMean() );

//pseudo-Data
h_Mej_genData_OneExp_3->SetStats(0);
h_Mej_genData_OneExp_3->SetTitle("");
h_Mej_genData_OneExp_3->SetMarkerStyle(20);
h_Mej_genData_OneExp_3->GetYaxis()->SetRangeUser(0.1,1000);
h_Mej_genData_OneExp_3->Draw();

float thisSign = h_significance_OneExp_3->GetMean();
float thisLumi = h_Lint_OneExp_3->GetMean();

legend->AddEntry(h_Mej_genData_OneExp_3,"pseudo-data","p");
legend->AddEntry(h_Mej_data,"LQ","f");
legend->AddEntry(h_Mej_allBkg,"all background","f");


//-------------------------------------------------

//all data
h_Mej_data->SetLineColor(kBlue);
h_Mej_data->SetLineWidth(2);
h_Mej_data->Draw("HISTEsame");

//all Bkg
h_Mej_allBkg->SetLineColor(kRed);
h_Mej_allBkg->SetFillColor(kRed);
h_Mej_allBkg->SetFillStyle(3004);
h_Mej_allBkg->SetLineWidth(2);
h_Mej_allBkg->Draw("HISTEsame");

char thisSignName[100];
sprintf(thisSignName,"Significance: %2.1f",thisSign);

char thisLumiName[100];
sprintf(thisLumiName,"Int. lumi: %d%s",int(thisLumi)," pb^{-1}");


TLatex *tInfo = new TLatex();
tInfo->SetNDC();
tInfo->SetTextSize(0.035);
tInfo->DrawLatex( 0.46, 0.82, &thisLumiName[0]);
tInfo->DrawLatex( 0.46, 0.76, &thisSignName[0]);

//Draw legend
legend->Draw();

// Redraw axis
gPad->RedrawAxis();


}
