#!/usr/bin/env python

##############################################################################
## DONT'T MODIFY WITHIN "# %%%%%%% BEGIN %%%%%%%"  and "# %%%%%%% END %%%%%%%"
##############################################################################

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

# %%%%%%% BEGIN %%%%%%%
def GetFile(filename):
    file = TFile(filename)
    if( not file):
        print "ERROR: file " + filename + " not found"
        print "exiting..."
        sys.exit()
    return file
# %%%%%%% END %%%%%%%

#output from /rootNtupleMacros/src/analysisClass_elecStudies2.C
#File1 = GetFile("eleEffLQUE250.root")
File1 = GetFile("eleEffZJet_new2.root")
#output from /rootNtupleMacros/src/analysisClass_muonStudies2.C
#File1 = GetFile("muonEffLQUE250.root")
File2 = GetFile("muonEffZJet_new2.root")
#this rootfile contains the histogram/s to rescale with the ratio of efficiency
File3 = GetFile("analysisClass_emujjSample_plots.root")

#--- Define functions

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



def GetEffVarBinning( num , den , m_size , m_style , m_color , xtitle , ytitle , min , max, Bins):

    NbinTot = len(Bins) 
    print Bins
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
            ratio.SetPointEXhigh(npoint, ehx1)
            ratio.SetPointEXlow(npoint, elx1)
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


def GetHistoRescaled( histo , function ):

    Bins = []
    BinsContent = []
    BinsContentErrMax = []
    for point in range( 0 , function.GetN() ):
        x, y = ROOT.Double(1), ROOT.Double(1)
        function.GetPoint(point, x, y)
        ehx = function.GetErrorXhigh(point)  
        elx = function.GetErrorXlow(point)
        ehy = function.GetErrorYhigh(point)
        ely = function.GetErrorYlow(point)  
        up = x + ehx
        low = x - elx
        if(point == 0):
            Bins.append( low )
        Bins.append( up )
        BinsContent.append( y )
        BinsContentErrMax.append( max(ehy,ely) )

    BinsFinal = array( 'f', Bins ) 

    ##define the tmp-histo
    histoFinal = TH1F("histoFinal",
                      histo.GetName()+"_rescaled",
                      len(Bins) - 1 , BinsFinal )
    histoFinal.Sumw2()

    ##fill the tmp-histo with the content of the histo passed as argument to the function
    for bin1 in range( 1 , histoFinal.GetNbinsX()+1 ):
        center = histoFinal.GetBinCenter(bin1)
        halfwidth = histoFinal.GetBinWidth(bin1) / 2
        #print center
        #print halfwidth
        newValue = 0
        newErr = 0
        for bin2 in range( 1 , histo.GetNbinsX()+1 ):
            if(histo.GetBinCenter(bin2) > (center - halfwidth)
               and
               histo.GetBinCenter(bin2) < (center + halfwidth) ):
                newValue = newValue + histo.GetBinContent(bin2)
                newErr = newErr + histo.GetBinError(bin2)*histo.GetBinError(bin2)

        histoFinal.SetBinContent(bin1,newValue)
        histoFinal.SetBinError(bin1,sqrt(newErr))

#    for bin in range( 1 , histo.GetNbinsX()+1 ):
#        for entry in range( 0 , int(histo.GetBinContent(bin)) ):
#            histoFinal.SetBinContent( histo.GetBinCenter(bin) , float(histo.GetBinContent(bin) )

    ##rescale tmp-histo with the function
    for index, rescaleFactor in enumerate( BinsContent ):
        #print index
        #print "histoFinal.GetBinContent(index+1): " + str(histoFinal.GetBinContent(index+1))
        #print "histoFinal.GetBinError(index+1): " + str(histoFinal.GetBinError(index+1))
        #print "rescaleFactor: " + str(rescaleFactor)
        #print "BinsContentErrMax[index]: " + str(BinsContentErrMax[index])
        rescaledBinContent = rescaleFactor * histoFinal.GetBinContent(index+1)
        if( histoFinal.GetBinContent(index+1) >0 and rescaleFactor >0 ):
            erelh = histoFinal.GetBinError(index+1) / histoFinal.GetBinContent(index+1)
            erelf = BinsContentErrMax[index] / rescaleFactor
            erelhfin = sqrt(erelh*erelh + erelf*erelf)
            rescaledBinError = erelhfin * rescaledBinContent
            #print "rescaledBinContent: " + str(rescaledBinContent)
            #print "rescaledBinError: " + str(rescaledBinError)
            histoFinal.SetBinContent(index+1 , rescaledBinContent )
            histoFinal.SetBinError(index+1 , rescaledBinError )
        else:
            histoFinal.SetBinContent(index+1 , 0 )
            histoFinal.SetBinError(index+1 , 0 )
        #print ""

    return histoFinal

def GetIntegral(histo):
    return histo.Integral()

def GetIntegralError(histo):
    integralError = 0
    for bin in range( 1 , histo.GetNbinsX()+1 ):
        integralError = integralError + (histoFinal.GetBinError(bin)*histoFinal.GetBinError(bin))
    integralError = sqrt(integralError)
    return integralError
        
# %%%%%%% END %%%%%%% 

                
#--- Define all the histograms

## Eta histograms
h_ele_eta_gen_all = GetHisto( "ele_Eta_Gen_etaCut" , File1)
h_ele_eta_gen_ID_ISO = GetHisto( "ele_Eta_Gen_matched_ID_ISO" , File1)

h_muon_eta_gen_all = GetHisto( "muon_Eta_Gen_etaCut" , File2)
h_muon_eta_gen_ID_ISO = GetHisto( "muon_Eta_Gen_matched_ID_ISO" , File2)

## Pt histograms
h_ele_pt_gen_all = GetHisto( "ele_Pt_Gen_etaCut" , File1)
h_ele_pt_gen_ID_ISO = GetHisto( "ele_Pt_Gen_matched_ID_ISO" , File1)

h_muon_pt_gen_all = GetHisto( "muon_Pt_Gen_etaCut" , File2)
h_muon_pt_gen_ID_ISO = GetHisto( "muon_Pt_Gen_matched_ID_ISO" , File2)

## Pt histograms to be rescaled
h_ptmuon_allcuts = GetHisto( "histo1D__TTBAR__cutHisto_allCuts________________Pt1stMuIDISO_DIS" , File3 )

#--- Calculate Efficiency

eff_ele_eta_gen_ID_ISO = GetEffFixBinning( h_ele_eta_gen_ID_ISO , h_ele_eta_gen_all
                                    , 0.9 , 20 , 1
                                    , "electron \\eta_{gen}" , "reconstruction efficiency"
                                    , -2.5 , 2.5)

eff_muon_eta_gen_ID_ISO = GetEffFixBinning( h_muon_eta_gen_ID_ISO , h_muon_eta_gen_all
                                     , 0.9 , 20 , 1
                                     , "muon \\eta_{gen}" , "reconstruction efficiency"
                                     , -2.5 , 2.5)

############################
#variable binning
#LQ 250
#MyBins = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210,
#          220, 230, 240, 250,
#          300, 350, 400, 450, 500, 550, 600, 1000]
#Z+jets
MyBins = [30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 400]
############################

eff_ele_pt_gen_ID_ISO = GetEffVarBinning( h_ele_pt_gen_ID_ISO , h_ele_pt_gen_all
                                          , 0.9 , 20 , 1
                                          , "electron pT_{gen} (GeV)" , "reconstruction efficiency"
                                          , 0 , 1000
                                          , MyBins)

eff_muon_pt_gen_ID_ISO = GetEffVarBinning( h_muon_pt_gen_ID_ISO , h_muon_pt_gen_all
                                           , 0.9 , 20 , 1
                                           , "muon pT_{gen} (GeV)" , "reconstruction efficiency"
                                           , 0 , 1000
                                           , MyBins)

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

#--- Rescale histogram with ratio of efficiency

h_ptmuon_allcuts_rescaled = GetHistoRescaled( h_ptmuon_allcuts , ratioEff_pt_gen_ID_ISO )

print "integral = " + str( GetIntegral(h_ptmuon_allcuts_rescaled) )
print "error = " + str( GetIntegralError(h_ptmuon_allcuts_rescaled) )

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

## pT muon rescaled by Ratio Eff
c7 = TCanvas()
h_ptmuon_allcuts_rescaled.Draw()

## Terminate the program
print "Press ENTER to terminate"
wait=raw_input()
