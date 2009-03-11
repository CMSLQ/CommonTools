#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re
import ROOT
from array import array

#--- ROOT general options
gStyle.SetOptStat(0)
gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetPadTickX(1);
gStyle.SetPadTickY(1);
#--- TODO: WHY IT DOES NOT LOAD THE DEFAULT ROOTLOGON.C ? ---#

#--- Root files
#output from /rootNtupleMacros/src/analysisClass_elecStudies2.C
name_File1 = "eleEffLQUE250.root"
File1 = TFile(name_File1)

#output from /rootNtupleMacros/src/analysisClass_muonStudies2.C
name_File2 = "muonEffLQCMu250.root"
File2 = TFile(name_File2)


#--- Define functions
## DONT'T MODIFY WITHIN "# %%%%%%% BEGIN %%%%%%%"  and "# %%%%%%% END %%%%%%% "

# %%%%%%% BEGIN %%%%%%% 

def GetHisto( histoName , file ):
    histo = file.Get( histoName )
    if( not histo):
        print "ERROR: histo " + histoName + " not found in " + file.GetName()
        print "exiting..."
        sys.exit()
    return histo



def GetEffFixBinning( num , den , m_size , m_style , m_color , xtitle , ytitle , min , max):
    GraphEffTemp = TGraphAsymmErrors( num , den )
    GraphEff = TGraphAsymmErrors()
    counter=0
    for point in range( 0 , GraphEffTemp.GetN() ):
        x = ROOT.Double(1)
        y = ROOT.Double(1)
        GraphEffTemp.GetPoint( point , x , y) 
        if( x > max or x < min ):
            continue
        GraphEff.SetPoint(counter,x,y);
        GraphEff.SetPointError(counter,
                               GraphEffTemp.GetErrorXlow(point),
                               GraphEffTemp.GetErrorXhigh(point),
                               GraphEffTemp.GetErrorYlow(point),
                               GraphEffTemp.GetErrorYhigh(point) )
        counter = counter + 1

    GraphEff.SetMarkerSize( m_size )
    GraphEff.SetMarkerStyle( m_style )
    GraphEff.SetMarkerColor( m_color )
    GraphEff.GetXaxis().SetTitle(xtitle)
    GraphEff.GetYaxis().SetTitle(ytitle)

    return GraphEff



def GetEffVarBinning( num , den , m_size , m_style , m_color , xtitle , ytitle , min , max,
                      Nbin1, step1, Nbin2, step2, Nbin3, step3):

    #--- adjustment for variable binning
    lastBin1 = step1*(Nbin1)
    lastBin2 = step1*(Nbin1) + step2*(Nbin2)
    lastBin3 = step1*(Nbin1) + step2*(Nbin2) + step3*(Nbin3)
    NbinTot = Nbin1 + Nbin2 + Nbin3;
    Bins = []

    for bin in range( 0 , Nbin1 ):
        Bins.append(step1 * ( bin - (-1) ) )

    for bin in range( Nbin1 , Nbin2 + Nbin1 ):
        Bins.append(Bins[ Nbin1 - 1 ] + step2 * ( bin - ( Nbin1 - 1 ) ) )

    for bin in range( Nbin2 + Nbin1 , Nbin3 + Nbin2 + Nbin1 ):
        Bins.append(Bins[ Nbin2 + Nbin1 - 1 ] + step3 * ( bin - ( Nbin2 + Nbin1 - 1 ) ) )

    #print Bins
    BinsFinal = array( 'f', Bins ) 

    num_varBin = TH1F("num_varBin",
                      "num_varBin",
                      NbinTot - 1 , BinsFinal );
    nBins_num = num.GetXaxis().GetNbins()
    getxmax_num = num.GetXaxis().GetXmax();
    getxmin_num = num.GetXaxis().GetXmin();
    step_num = (getxmax_num - getxmin_num) / nBins_num;
    for bin in range( 1 , nBins_num+1 ):
        for entry in range( 0 , int(num.GetBinContent(bin)) ):
            num_varBin.Fill(step_num * bin)

    den_varBin = TH1F("den_varBin",
                      "den_varBin",
                      NbinTot - 1 , BinsFinal );
    nBins_den = den.GetXaxis().GetNbins()
    getxmax_den = den.GetXaxis().GetXmax();
    getxmin_den = den.GetXaxis().GetXmin();
    step_den = (getxmax_den - getxmin_den) / nBins_den;
    for bin in range( 1 , nBins_den+1 ):
        for entry in range( 0 , int(den.GetBinContent(bin)) ):
            den_varBin.Fill(step_den * bin)


    #--- create final graph
    GraphEffTemp = TGraphAsymmErrors( num_varBin , den_varBin )
    GraphEff = TGraphAsymmErrors()
    counter=0
    for point in range( 0 , GraphEffTemp.GetN() ):
        x = ROOT.Double(1)
        y = ROOT.Double(1)
        GraphEffTemp.GetPoint( point , x , y) 
        if( x > max or x < min ):
            continue
        GraphEff.SetPoint(counter,x,y);
        GraphEff.SetPointError(counter,
                               GraphEffTemp.GetErrorXlow(point),
                               GraphEffTemp.GetErrorXhigh(point),
                               GraphEffTemp.GetErrorYlow(point),
                               GraphEffTemp.GetErrorYhigh(point) )
        counter = counter + 1

    GraphEff.SetMarkerSize( m_size )
    GraphEff.SetMarkerStyle( m_style )
    GraphEff.SetMarkerColor( m_color )
    GraphEff.GetXaxis().SetTitle(xtitle)
    GraphEff.GetYaxis().SetTitle(ytitle)

    return GraphEff



def GetRatioEff( num , den , m_size , m_style , m_color , xtitle , ytitle ):

    ratio = TGraphAsymmErrors()
    npoint = 0;

    if(num.GetN() != den.GetN()):
        print "ERROR in GetRatioEff: num.GetN() != den.GetN()"
        print "exiting..."
        sys.exit()
        
    for point in range( 0 , den.GetN() ):
        
        x1, y1, x2, y2 = ROOT.Double(1), ROOT.Double(1), ROOT.Double(1), ROOT.Double(1)
        
        num.GetPoint(point, x1, y1)
        ehx1 = num.GetErrorXhigh(point)  
        ehy1 = num.GetErrorYhigh(point)  
        elx1 = num.GetErrorXlow(point)  
        ely1 = num.GetErrorYlow(point)  
        
        den.GetPoint(point, x2, y2)
        ehx2 = den.GetErrorXhigh(point)  
        ehy2 = den.GetErrorYhigh(point)  
        elx2 = den.GetErrorXlow(point)  
        ely2 = den.GetErrorYlow(point)  
        
        if( y2 == 0 or y1==0 ):
            ratio.SetPoint(npoint,x1, 0)
            ratio.SetPointEXhigh(npoint, 0)
            ratio.SetPointEXlow(npoint, 0)
            ratio.SetPointEYhigh(npoint, 0)
            ratio.SetPointEYlow(npoint, 0)
        else:
            r = y1/y2
            erelhy1 = ehy1 / y1
            erelhy2 = ehy2 / y2
            erelly1 = ely1 / y1
            erelly2 = ely2 / y2
            erelhr = sqrt(erelhy1 * erelhy1 + erelhy2 * erelhy2)
            erellr = sqrt(erelly1 * erelly1 + erelly2 * erelly2)
            ehr=erelhr*r
            elr=erellr*r
            
            ratio.SetPoint(npoint, x1, r)
            ratio.SetPointEXhigh(npoint, ehx1)
            ratio.SetPointEXlow(npoint, elx1)
            ratio.SetPointEYhigh(npoint, ehr)
            ratio.SetPointEYlow(npoint, elr)
        
        npoint = npoint + 1

    ratio.SetMarkerSize( m_size )
    ratio.SetMarkerStyle( m_style )
    ratio.SetMarkerColor( m_color )
    ratio.GetXaxis().SetTitle(xtitle)
    ratio.GetYaxis().SetTitle(ytitle)

    return ratio


# %%%%%%% END %%%%%%% 

                
#--- Define all the histograms

## Eta histograms
name_ele_eta_gen_all = "ele_Eta_Gen_etaCut"
h_ele_eta_gen_all = GetHisto( name_ele_eta_gen_all , File1)
name_ele_eta_gen_ID_ISO = "ele_Eta_Gen_matched_ID_ISO"
h_ele_eta_gen_ID_ISO = GetHisto( name_ele_eta_gen_ID_ISO , File1)

name_muon_eta_gen_all = "muon_Eta_Gen_etaCut"
h_muon_eta_gen_all = GetHisto( name_muon_eta_gen_all , File2)
name_muon_eta_gen_ID_ISO = "muon_Eta_Gen_matched_ID_ISO"
h_muon_eta_gen_ID_ISO = GetHisto( name_muon_eta_gen_ID_ISO , File2)

## Pt histograms
name_ele_pt_gen_all = "ele_Pt_Gen_etaCut"
h_ele_pt_gen_all = GetHisto( name_ele_pt_gen_all , File1)
name_ele_pt_gen_ID_ISO = "ele_Pt_Gen_matched_ID_ISO"
h_ele_pt_gen_ID_ISO = GetHisto( name_ele_pt_gen_ID_ISO , File1)

name_muon_pt_gen_all = "muon_Pt_Gen_etaCut"
h_muon_pt_gen_all = GetHisto( name_muon_pt_gen_all , File2)
name_muon_pt_gen_ID_ISO = "muon_Pt_Gen_matched_ID_ISO"
h_muon_pt_gen_ID_ISO = GetHisto( name_muon_pt_gen_ID_ISO , File2)


#--- Calculate Efficiency

eff_ele_eta_gen_ID_ISO = GetEffFixBinning( h_ele_eta_gen_ID_ISO , h_ele_eta_gen_all
                                    , 0.9 , 20 , 1
                                    , "electron \\eta_{gen}" , "reconstruction efficiency"
                                    , -2.5 , 2.5)

eff_muon_eta_gen_ID_ISO = GetEffFixBinning( h_muon_eta_gen_ID_ISO , h_muon_eta_gen_all
                                     , 0.9 , 20 , 1
                                     , "muon \\eta_{gen}" , "reconstruction efficiency"
                                     , -2.5 , 2.5)

eff_ele_pt_gen_ID_ISO = GetEffVarBinning( h_ele_pt_gen_ID_ISO , h_ele_pt_gen_all
                                          , 0.9 , 20 , 1
                                          , "electron pT_{gen} (GeV)" , "reconstruction efficiency"
                                          , 0 , 1000
                                          , 25,10 , 7,50 , 1,400)

eff_muon_pt_gen_ID_ISO = GetEffVarBinning( h_muon_pt_gen_ID_ISO , h_muon_pt_gen_all
                                           , 0.9 , 20 , 1
                                           , "muon pT_{gen} (GeV)" , "reconstruction efficiency"
                                           , 0 , 1000
                                           , 25,10 , 7,50 , 1,400)

##Variable bins
#LQ 250 --> (N_points,step): 25,10 , 7,50 , 1,400


#--- Calculate Ratio of Efficiency

ratioEff_eta_gen_ID_ISO = GetRatioEff( eff_ele_eta_gen_ID_ISO , eff_muon_eta_gen_ID_ISO
                                       , 0.9 , 20 , 1
                                       , "electron/muon \\eta_{gen}"
                                       , "efficiency ratio - \\varepsilon_{ele}/\\varepsilon_{muon}")

ratioEff_pt_gen_ID_ISO = GetRatioEff( eff_ele_pt_gen_ID_ISO , eff_muon_pt_gen_ID_ISO
                                      , 0.9 , 20 , 1
                                      , "electron/muon pT_{gen} (GeV)"
                                      , "efficiency ratio - \\varepsilon_{ele}/\\varepsilon_{muon}")

#--- Final plots

## Eff vs eta - electron
c1 = TCanvas()
c1.SetGridy();
c1.SetGridx();
eff_ele_eta_gen_ID_ISO.Draw("ap")
## Eff vs eta - muon
c2 = TCanvas()
c2.SetGridy();
c2.SetGridx();
eff_muon_eta_gen_ID_ISO.Draw("ap")
## Ratio Eff vs eta - ele / muon
c3 = TCanvas()
c3.SetGridy();
c3.SetGridx();
ratioEff_eta_gen_ID_ISO.Draw("ap")


## Eff vs pt - electron
c4 = TCanvas()
c4.SetGridy();
c4.SetGridx();
eff_ele_pt_gen_ID_ISO.Draw("ap")
## Eff vs pt - muon
c5 = TCanvas()
c5.SetGridy();
c5.SetGridx();
eff_muon_pt_gen_ID_ISO.Draw("ap")
## Ratio Eff vs pt - ele / muon
c6 = TCanvas()
c6.SetGridy();
c6.SetGridx();
ratioEff_pt_gen_ID_ISO.Draw("ap")


## Terminate the program
print "Press ENTER to terminate"
wait=raw_input()
