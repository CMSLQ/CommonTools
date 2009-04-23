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
#File1 = GetFile("ElePlotsNoteJune09/analysisClass_elecStudies2___Exotica_LQtoUE_M400__Summer08_IDEAL_V9_v1__GEN-SIM-RECO.root")
#File1 = GetFile("analysisClass_elecStudies2_LQtoUE_M400_barrel.root")
File1 = GetFile("analysisClass_elecStudies2_LQtoUE_M400.root")

#--- Define functions

# %%%%%%% BEGIN %%%%%%%     

def GetHisto( histoName , file ):
    histo = file.Get( histoName )
    if( not histo):
        print "ERROR: histo " + histoName + " not found in " + file.GetName()
        print "exiting..."
        sys.exit()
    return histo



def GetEffFixBinning( num , den , m_size , m_style , m_color , xtitle , ytitle , min , max, EffMin, EffMax):
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

    GraphEff.GetYaxis().SetRangeUser(EffMin,EffMax)

    return GraphEff



def GetEffVarBinning( num , den , m_size , m_style , m_color , xtitle , ytitle , min , max, Bins, EffMin, EffMax):

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

    GraphEff.GetYaxis().SetRangeUser(EffMin,EffMax)

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

#-- reconstruction efficiencies

## Eta histograms
h_ele_eta_gen_all = GetHisto( "ele_Eta_Gen" , File1)
h_ele_eta_gen_cuts = GetHisto( "ele_Eta_Gen_etaCut" , File1)
h_ele_eta_gen_RECO = GetHisto( "ele_Eta_Gen_matched" , File1)
h_ele_eta_gen_RECO_ID = GetHisto( "ele_Eta_Gen_matched_ID" , File1)
h_ele_eta_gen_RECO_ID_ISO = GetHisto( "ele_Eta_Gen_matched_ID_ISO" , File1)

## Pt histograms
h_ele_pt_gen_all = GetHisto( "ele_Pt_Gen" , File1)
h_ele_pt_gen_cuts = GetHisto( "ele_Pt_Gen_etaCut" , File1)
h_ele_pt_gen_RECO = GetHisto( "ele_Pt_Gen_matched" , File1)
h_ele_pt_gen_RECO_ID = GetHisto( "ele_Pt_Gen_matched_ID" , File1)
h_ele_pt_gen_RECO_ID_ISO = GetHisto( "ele_Pt_Gen_matched_ID_ISO" , File1)


############################
#variable binning
#LQ 250
MyBins = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 900, 1200]
#Z+jets
#MyBins = [30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 400]
############################


#--- Calculate Acceptance
accept_ele_eta_gen = GetEffFixBinning( h_ele_eta_gen_cuts , h_ele_eta_gen_all
                                       , 0.9 , 20 , 1
                                       , "electron \\eta_{gen}" , "acceptance"
                                       , -2.5 , 2.5, 0, 1)

accept_ele_pt_gen = GetEffVarBinning( h_ele_pt_gen_cuts , h_ele_pt_gen_all
                                      , 0.9 , 20 , 1
                                      , "electron pT_{gen} (GeV)" , "acceptance"
                                      , 0 , 1200
                                      , MyBins, 0.5, 1)

#--- Calculate Absolute Efficiency
eff_ele_eta_gen_RECO = GetEffFixBinning( h_ele_eta_gen_RECO , h_ele_eta_gen_cuts
                                           , 0.9 , 20 , 1
                                           , "electron \\eta_{gen}" , "relative efficiency wrt to acceptance"
                                           , -2.5 , 2.5, 0, 1)

eff_ele_eta_gen_RECO_ID = GetEffFixBinning( h_ele_eta_gen_RECO_ID , h_ele_eta_gen_cuts
                                            , 0.9 , 20 , 2
                                            , "electron \\eta_{gen}" , "relative efficiency wrt to acceptance"
                                            , -2.5 , 2.5, 0, 1)

eff_ele_eta_gen_RECO_ID_ISO = GetEffFixBinning( h_ele_eta_gen_RECO_ID_ISO , h_ele_eta_gen_cuts
                                                , 0.9 , 20 , 4
                                                , "electron \\eta_{gen}" , "relative efficiency wrt to acceptance"
                                                , -2.5 , 2.5, 0, 1)


eff_ele_pt_gen_RECO = GetEffVarBinning( h_ele_pt_gen_RECO , h_ele_pt_gen_cuts
                                        , 0.9 , 20 , 1
                                        , "electron pT_{gen} (GeV)" , "relative efficiency wrt to acceptance"
                                        , 0 , 1200
                                        , MyBins, 0.5, 1)

eff_ele_pt_gen_RECO_ID = GetEffVarBinning( h_ele_pt_gen_RECO_ID , h_ele_pt_gen_cuts
                                           , 0.9 , 20 , 2
                                           , "electron pT_{gen} (GeV)" , "relative efficiency wrt to acceptance"
                                           , 0 , 1200
                                           , MyBins, 0.5, 1)

eff_ele_pt_gen_RECO_ID_ISO = GetEffVarBinning( h_ele_pt_gen_RECO_ID_ISO , h_ele_pt_gen_cuts
                                               , 0.9 , 20 , 4
                                               , "electron pT_{gen} (GeV)" , "relative efficiency wrt to acceptance"
                                               , 0 , 1200
                                               , MyBins, 0.5, 1)


#--- Calculate Relative Efficiency


eff_ele_pt_gen_RECO_ID_rel = GetEffVarBinning( h_ele_pt_gen_RECO_ID , h_ele_pt_gen_RECO
                                               , 0.9 , 20 , 2
                                               , "electron pT_{gen} (GeV)" , "relative efficiency"
                                               , 0 , 1200
                                               , MyBins, 0.5, 1)

eff_ele_pt_gen_RECO_ID_ISO_rel = GetEffVarBinning( h_ele_pt_gen_RECO_ID_ISO , h_ele_pt_gen_RECO_ID
                                               , 0.9 , 20 , 4
                                               , "electron pT_{gen} (GeV)" , "relative efficiency"
                                               , 0 , 1200
                                               , MyBins, 0.5, 1)


#--- Final plots

## Eta distributions - electron
c0 = TCanvas()
h_ele_eta_gen_all.SetMarkerStyle(20)
h_ele_eta_gen_all.SetMarkerColor(kBlack)
h_ele_eta_gen_all.SetTitle("")
h_ele_eta_gen_all.GetXaxis().SetTitle("electron \\eta_{gen}")
h_ele_eta_gen_all.GetYaxis().SetTitle("arbitrary unit")
h_ele_eta_gen_all.Draw("ap")
h_ele_eta_gen_cuts.SetMarkerStyle(22)
h_ele_eta_gen_cuts.SetMarkerColor(kRed)
h_ele_eta_gen_cuts.Draw("psame")


#---Create legend
globals()['legend0'] = TLegend(0.564648,0.6618,0.869067,0.851582)
legend0.SetFillColor(kWhite)
legend0.SetMargin(0.3)
legend0.AddEntry(h_ele_eta_gen_all,"no cuts","p")
legend0.AddEntry(h_ele_eta_gen_cuts,"acceptance cuts","p")
legend0.Draw()
c0.Update()
gPad.RedrawAxis()
gPad.Modified()


## pt distributions - electron
c00 = TCanvas()
h_ele_pt_gen_all.SetMarkerStyle(20)
h_ele_pt_gen_all.SetMarkerColor(kBlack)
h_ele_pt_gen_all.SetTitle("")
h_ele_pt_gen_all.GetXaxis().SetTitle("electron pt_{gen} (GeV)")
h_ele_pt_gen_all.GetYaxis().SetTitle("arbitrary unit")
h_ele_pt_gen_all.Draw("ap")
h_ele_pt_gen_cuts.SetMarkerStyle(22)
h_ele_pt_gen_cuts.SetMarkerColor(kRed)
h_ele_pt_gen_cuts.Draw("psame")

#---Create legend
globals()['legend00'] = TLegend(0.564648,0.6618,0.869067,0.851582)
legend00.SetFillColor(kWhite)
legend00.SetMargin(0.3)
legend00.AddEntry(h_ele_pt_gen_all,"no cuts","p")
legend00.AddEntry(h_ele_pt_gen_cuts,"acceptance cuts","p")
legend00.Draw()
c00.Update()
gPad.RedrawAxis()
gPad.Modified()



## Acceptance vs eta - electron
c1 = TCanvas()
c1.SetGridy();
c1.SetGridx();
accept_ele_eta_gen.Draw("ap")


## Acceptance vs pt - electron
c2 = TCanvas()
c2.SetGridy();
c2.SetGridx();
accept_ele_pt_gen.Draw("ap")

## Eff vs eta - electron
c3 = TCanvas()
c3.SetGridy();
c3.SetGridx();
eff_ele_eta_gen_RECO.Draw("ap")
eff_ele_eta_gen_RECO_ID.Draw("psame")
eff_ele_eta_gen_RECO_ID_ISO.Draw("psame")

#---Create legend
globals()['legend3'] = TLegend(0.348609,0.184915,0.653028,0.377129)
legend3.SetFillColor(kWhite)
legend3.SetMargin(0.3)
legend3.AddEntry(eff_ele_eta_gen_RECO,"only reco","p")
legend3.AddEntry(eff_ele_eta_gen_RECO_ID,"reco+ID","p")
legend3.AddEntry(eff_ele_eta_gen_RECO_ID_ISO,"reco+ID+ISO","p")
c3.Update()
legend3.DrawClone()
gPad.RedrawAxis()
gPad.Modified()

## Eff vs pt - electron
c4 = TCanvas()
c4.SetGridy();
c4.SetGridx();
eff_ele_pt_gen_RECO.Draw("ap")
eff_ele_pt_gen_RECO_ID.Draw("psame")
eff_ele_pt_gen_RECO_ID_ISO.Draw("psame")

#---Create legend
globals()['legend4'] = TLegend(0.348609,0.184915,0.653028,0.377129)
legend4.SetFillColor(kWhite)
legend4.SetMargin(0.3)
legend4.AddEntry(eff_ele_pt_gen_RECO,"only reco","p")
legend4.AddEntry(eff_ele_pt_gen_RECO_ID,"reco+ID","p")
legend4.AddEntry(eff_ele_pt_gen_RECO_ID_ISO,"reco+ID+ISO","p")
c4.Update()
legend4.DrawClone()
gPad.RedrawAxis()
gPad.Modified()

## Relative Eff vs pt - electron
c5 = TCanvas()
c5.SetGridy();
c5.SetGridx();
eff_ele_pt_gen_RECO_ID_rel.Draw("ap")
eff_ele_pt_gen_RECO_ID_ISO_rel.Draw("psame")

#---Create legend
globals()['legend5'] = TLegend(0.348609,0.184915,0.653028,0.377129)
legend5.SetFillColor(kWhite)
legend5.SetMargin(0.3)
legend5.AddEntry(eff_ele_pt_gen_RECO_ID_rel,"ID eff wrt to RECO","p")
legend5.AddEntry(eff_ele_pt_gen_RECO_ID_ISO_rel,"ISO eff wrt to ID+RECO","p")
c5.Update()
legend5.DrawClone()
gPad.RedrawAxis()
gPad.Modified()



#Print out
print "Acceptance = " + str(h_ele_eta_gen_cuts.Integral()/h_ele_eta_gen_all.Integral())
print "Reco eff wrt to acceptance = " + str(h_ele_eta_gen_RECO.Integral()/h_ele_eta_gen_cuts.Integral())
print "Reco+ID eff wrt to acceptance = " + str(h_ele_eta_gen_RECO_ID.Integral()/h_ele_eta_gen_cuts.Integral())
print "Reco+ID+ISO eff wrt to acceptance = " + str(h_ele_eta_gen_RECO_ID_ISO.Integral()/h_ele_eta_gen_cuts.Integral())


## Terminate the program
print "Press ENTER to terminate"
wait=raw_input()
