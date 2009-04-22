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

# %%%%%%% BEGIN %%%%%%%

#--- ROOT general options
gStyle.SetOptStat(0)
gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetPadTickX(1);
gStyle.SetPadTickY(1);
#--- TODO: WHY IT DOES NOT LOAD THE DEFAULT ROOTLOGON.C ? ---#


#--- Functions
def GetHisto( histoName , file ):
    histo = file.Get( histoName )
    if( not histo):
        print "ERROR: histo " + histoName + " not found in " + file.GetName()
        print "exiting..."
        sys.exit()
    return histo


def CompareHisto( histoSig, histoBkg, labelSig, labelBkg, xmin, xmax, xtitle, ytitle, canvas, logy, rebin, outputFileName):

    canvas.cd()

    if(logy == 1):
        canvas.SetLogy(1)

    histoSig.Rebin(rebin)
    histoBkg.Rebin(rebin)

    histoSig.GetXaxis().SetRangeUser(xmin, xmax)
    histoSig.GetXaxis().SetTitle(xtitle)
    histoSig.GetYaxis().SetTitle(ytitle)

    histoSig.SetFillColor(kWhite)
    histoSig.SetFillStyle(1001)
    histoSig.SetLineColor(kBlue)
    histoSig.SetLineWidth(2)

    histoBkg.SetFillColor(kRed)
    histoBkg.SetFillStyle(3002)
    histoBkg.SetLineColor(kRed)
    histoBkg.SetLineWidth(2)

    histoSig.DrawNormalized("HISTE")
    histoBkg.DrawNormalized("HISTEsame")

    #---Create legend
    globals()['legend'] = TLegend(0.561375,0.6691,0.865794,0.863747)
    legend.SetFillColor(kWhite)
    legend.SetMargin(0.3)
    legend.AddEntry(histoSig,labelSig,"f")
    legend.AddEntry(histoBkg,labelBkg,"f")
    canvas.Update()
    legend.DrawClone()
    gPad.RedrawAxis()
    gPad.Modified()

    if(savePlots==true):
        canvas.SaveAs(outputFileName + "_LQ400vsQCD.eps")
        #canvas.SaveAs(outputFileName + "_LQ400vsQCD.gif")

# %%%%%%% END %%%%%%%

#--- Load input file

gROOT.Reset()
gStyle.SetHistMinimumZero(1)

inputRootFile = "/home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer_26Jan2009/ElePlotsNoteJune09//analysisClass_elecStudies2_plots.root"

file = TFile(inputRootFile)
file.ls()


######
savePlots = false
######

#--- Compare sig vs bkg histograms

#CompareHisto( histoSig, histoBkg,
#              labelSig, labelBkg,
#              xmin, xmax, xtitle, ytitle,
#              canvas, logy, rebin):

#EcalRecHit Isolation - barrel
c1 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleEcalRecHitIso_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleEcalRecHitIso_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "EcalRecHit Isolation - barrel (GeV)", "arbitrary unit", 
             c1, 0, 1, "eleEcalRecHitIso_barrel")

#EcalRecHit Isolation - endcap
c2 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleEcalRecHitIso_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleEcalRecHitIso_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "EcalRecHit Isolation - endcap (GeV)", "arbitrary unit", 
             c2, 0, 1, "eleEcalRecHitIso_endcap")

#HcalRecHit Isolation - barrel
c3 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleHcalRecHitIso_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleHcalRecHitIso_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "HcalRecHit Isolation - barrel (GeV)", "arbitrary unit", 
             c3, 0, 1, "eleHcalRecHitIso_barrel")

#HcalRecHit Isolation - endcap
c4 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleHcalRecHitIso_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleHcalRecHitIso_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "HcalRecHit Isolation - endcap (GeV)", "arbitrary unit", 
             c4, 0, 1, "eleHcalRecHitIso_endcap")

#Trk Isolation - barrel
c5 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleTrkIso_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleTrkIso_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "Trk Isolation - barrel (GeV)", "arbitrary unit", 
             c5, 1, 1, "eleTrkIso_barrel")

#Trk Isolation - endcap
c6 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleTrkIso_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleTrkIso_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 20, "Trk Isolation - endcap (GeV)", "arbitrary unit", 
             c6, 1, 1, "eleTrkIso_endcap")

#NumTrk Isolation - barrel
c7 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleNumTrkIso_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleNumTrkIso_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 10, "NumTrk Isolation - barrel (GeV)", "arbitrary unit", 
             c7, 0, 1, "eleNumTrkIso_barrel")

#NumTrk Isolation - endcap
c8 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleNumTrkIso_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleNumTrkIso_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 10, "NumTrk Isolation - endcap (GeV)", "arbitrary unit", 
             c8, 0, 1, "eleNumTrkIso_endcap")

#DeltaPhiTrkSC - barrel
c9 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleDeltaPhiTrkSC_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleDeltaPhiTrkSC_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             -0.02, 0.02, "\\Delta_{\\phi} trk/SC - barrel", "arbitrary unit", 
             c9, 0, 1, "eleDeltaPhiTrkSC_barrel")

#DeltaPhiTrkSC - endcap
c10 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleDeltaPhiTrkSC_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleDeltaPhiTrkSC_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             -0.02, 0.02, "\\Delta_{\\phi} trk/SC - endcap", "arbitrary unit", 
             c10, 0, 1, "eleDeltaPhiTrkSC_endcap")


#DeltaEtaTrkSC - barrel
c11 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleDeltaEtaTrkSC_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleDeltaEtaTrkSC_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             -0.02, 0.02, "\\Delta_{\\eta} trk/SC - barrel", "arbitrary unit", 
             c11, 0, 1, "eleDeltaEtaTrkSC_barrel")

#DeltaEtaTrkSC - endcap
c12 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleDeltaEtaTrkSC_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleDeltaEtaTrkSC_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             -0.02, 0.02, "\\Delta_{\\eta} trk/SC - endcap", "arbitrary unit", 
             c12, 0, 1, "eleDeltaEtaTrkSC_endcap")

#HoE - barrel
c13 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleHoE_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleHoE_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 0.1, "H/E - barrel", "arbitrary unit", 
             c13, 1, 1, "eleHoE_barrel")

#HoE - endcap
c14 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleHoE_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleHoE_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 0.1, "H/E - endcap", "arbitrary unit", 
             c14, 1, 1, "eleHoE_endcap")

#SigmaEE - barrel
c15 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleSigmaEE_barrel_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleSigmaEE_barrel_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 0.03, "\\sigma_{\\eta\\eta} - barrel", "arbitrary unit", 
             c15, 0, 1, "eleSigmaEE_barrel")

#SigmaEE - endcap
c16 = TCanvas()
histoSig = GetHisto( "histo1D__LQtoUE_M400__eleSigmaEE_endcap_matched" , file)
histoBkg = GetHisto( "histo1D__QCD__eleSigmaEE_endcap_unmatched" , file)

CompareHisto(histoSig, histoBkg,
             "real electrons - LQ400", "fake electrons - QCD",
             0, 0.06, "\\sigma_{\\eta\\eta} - endcap", "arbitrary unit", 
             c16, 0, 1, "eleSigmaEE_endcap")


## Terminate the program
print "Press ENTER to terminate"
wait=raw_input()
