#!/usr/bin/env python

##############################################################################
## DONT'T MODIFY WITHIN "# %%%%%%% BEGIN %%%%%%%"  and "# %%%%%%% END %%%%%%%"
##############################################################################

###########################################
#Important!
#The input file must be in the followinf format
#
# KEEP THE ORDERING OF THE TABLES
#
#Zjets-madgraph__Winter09_FastSim AtZ
#ZJets-madgraph__Fall08_FullSim   AtZ
#Zjets-madgraph__Winter09_FastSim OffZ
#ZJets-madgraph__Fall08_FullSim   OffZ
#
#
###########################################

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

#--- Define functions

# %%%%%%% BEGIN %%%%%%%     

        
# %%%%%%% END %%%%%%% 
                
#--- Define all the histograms

FileCombined = "combinedFile_14points.dat"

#---Read .dat table for current dataset
dataFastAtZ={}  #sectionCounter 1
dataFullAtZ={}  #sectionCounter 2
dataFastOffZ={} #sectionCounter 3
dataFullOffZ={} #sectionCounter 4
column=[]
lineCounter = int(0)
sectionCounter = int(0)

for j,line in enumerate( open( FileCombined ) ):
    
    #if( re.search("Zjets-madgraph__Winter09_FastSim",line)
    #    or re.search("ZJets-madgraph__Fall08_FullSim",line) ):
    #    sectionCounter = sectionCounter+1
    #    continue

    if( len(line.split())==1 ):
        sectionCounter = sectionCounter + 1
        continue

    if( len(line.split())==0 ):
        continue
    
    line = string.strip(line,"\n")
    #print "---> lineCounter: " , lineCounter
    #print line


    #FastAtZ
    if sectionCounter == 1:

        if lineCounter == 0:
            for i,piece in enumerate(line.split()):
                column.append(piece)
        else:
            for i,piece in enumerate(line.split()):
                if i == 0:
                    dataFastAtZ[piece] = {}
                    row = piece
                else:
                    dataFastAtZ[row][ column[i] ] = piece
                #print dataFastAtZ[row][ column[i] ] 
                
        lineCounter = lineCounter+1


    #FullAtZ
    if sectionCounter == 2:

        if lineCounter == 0:
            for i,piece in enumerate(line.split()):
                column.append(piece)
        else:
            for i,piece in enumerate(line.split()):
                if i == 0:
                    dataFullAtZ[piece] = {}
                    row = piece
                else:
                    dataFullAtZ[row][ column[i] ] = piece
                #print dataFullAtZ[row][ column[i] ] 
                
        lineCounter = lineCounter+1


    #FastOffZ
    if sectionCounter == 3:

        if lineCounter == 0:
            for i,piece in enumerate(line.split()):
                column.append(piece)
        else:
            for i,piece in enumerate(line.split()):
                if i == 0:
                    dataFastOffZ[piece] = {}
                    row = piece
                else:
                    dataFastOffZ[row][ column[i] ] = piece
                #print dataFastOffZ[row][ column[i] ] 
                
        lineCounter = lineCounter+1


    #FullOffZ
    if sectionCounter == 4:

        if lineCounter == 0:
            for i,piece in enumerate(line.split()):
                column.append(piece)
        else:
            for i,piece in enumerate(line.split()):
                if i == 0:
                    dataFullOffZ[piece] = {}
                    row = piece
                else:
                    dataFullOffZ[row][ column[i] ] = piece
                #print dataFullOffZ[row][ column[i] ] 
                
        lineCounter = lineCounter+1


summaryTable={}

for counter in range(0, 15):

    thisST = "sT"+str(counter)

    ## RATIO
    RatioAtZOffZFast = float(dataFastAtZ[thisST]["Npass"]) / float(dataFastOffZ[thisST]["Npass"])

    errRelNeeAtZFast2 = float(dataFastAtZ[thisST]["errNpass"]) / float(dataFastAtZ[thisST]["Npass"])
    errRelNeeAtZFast2 = errRelNeeAtZFast2*errRelNeeAtZFast2
    errRelNeeOffZFast2 = float(dataFastOffZ[thisST]["errNpass"]) / float(dataFastOffZ[thisST]["Npass"])
    errRelNeeOffZFast2 = errRelNeeOffZFast2*errRelNeeAtZFast2

    errRatioAtZOffZFast = sqrt(errRelNeeAtZFast2 + errRelNeeOffZFast2) * RatioAtZOffZFast

    errRelRatioAtZOffZFast2 = errRatioAtZOffZFast / RatioAtZOffZFast
    errRelRatioAtZOffZFast2 = errRelRatioAtZOffZFast2 * errRelRatioAtZOffZFast2

    ## Neejj Atz (DATA)
    NeeAtZFull = int(float(dataFullAtZ[thisST]["Npass"]))
    #TOTerrNeeAtZFull = sqrt( int(float(dataFullAtZ[thisST]["errNpass"])) * int(float(dataFullAtZ[thisST]["errNpass"])) + NeeAtZFull )
    TOTerrNeeAtZFull = sqrt( NeeAtZFull )

    errRelNeeAtZFull2 = TOTerrNeeAtZFull / NeeAtZFull
    errRelNeeAtZFull2 = errRelNeeAtZFull2 * errRelNeeAtZFull2

    ## NeeOffZEstimate = RATIO * Neejj Atz (DATA)
    NeeOffZEstimate = NeeAtZFull / RatioAtZOffZFast 
    
    errNeeOffZEstimate = sqrt(errRelRatioAtZOffZFast2 + errRelNeeAtZFull2) * NeeOffZEstimate

    summaryTable[thisST]={
        'STcut': dataFastAtZ[thisST]["min1"],
        'NeeAtZFast': dataFastAtZ[thisST]["Npass"],
        'errNeeAtZFast': dataFastAtZ[thisST]["errNpass"],
        'NeeOffZFast': dataFastOffZ[thisST]["Npass"],
        'errNeeOffZFast': dataFastOffZ[thisST]["errNpass"],
        'NeeAtZFull': int(float(dataFullAtZ[thisST]["Npass"])),
        'errNeeAtZFull': dataFullAtZ[thisST]["errNpass"],
        'errStatNeeAtZFull': sqrt(int(float(dataFullAtZ[thisST]["Npass"]))),
        'RatioAtZOffZFast': RatioAtZOffZFast,
        'errRatioAtZOffZFast': errRatioAtZOffZFast,
        'NeeOffZEstimate': NeeOffZEstimate,
        'errNeeOffZEstimate': errNeeOffZEstimate,
        'NeeOffZFull': dataFullOffZ[thisST]["Npass"],
        'errNeeOffZFull': dataFullOffZ[thisST]["errNpass"]
        }

    
#for i,piece in enumerate( summaryTable ):
#    print summaryTable[piece]


## PRINT ON FILE (.TEX TABLE) ##

file = open("GetZbkg.tex",'w')

#--> begin document
print >>file, "\\documentclass[a4paper,11pt]{article}" 
print >>file, "\\usepackage[latin1]{inputenc}" 
print >>file, "\\usepackage[italian]{babel}"
print >>file, "\\begin{document}"
print >>file, "\n"

#--> table header
print >>file, "\\begin{table}[htbp]"
print >>file, "\\begin{center}"
print >>file, "\\begin{tabular}{||c||c|c|c||c||}"
print >>file, "\hline\hline"

#--> first line
print >>file, "$S_{T}$ cut (GeV)".rjust(6),
print >>file, " & ", 
print >>file, "$N_{eejjAtZ}$".rjust(10),
print >>file, " & ", 
print >>file, "$R_{OffZ/AtZ}^{-1}$".rjust(20),
print >>file, " & ", 
print >>file, "$N_{eejj}^{Z}$".rjust(17),
print >>file, " & ", 
print >>file, "$N_{eejjOffZ}$".rjust(15),
print >>file, " \\\ "

#--> second line
print >>file, " ".rjust(6),
print >>file, " & ", 
print >>file, "(``data'')".rjust(10),
print >>file, " & ", 
print >>file, "(MC - FastSim)".rjust(20),
print >>file, " & ", 
print >>file, "(``data'' \& MC)".rjust(17),
print >>file, " & ", 
print >>file, "(MC - FullSim)".rjust(15),
print >>file, " \\\ "


#--> hline
print >>file, "\hline\hline"


for j, line in enumerate(summaryTable):

    thisST = "sT"+str(j)
    
    print >>file, ("%.i" % float(summaryTable[thisST]['STcut'])).rjust(6),
    print >>file, " & ", 
    #    print >>file, ("%.02f" % float(summaryTable[thisST]['NeeAtZFast'])).rjust(10),
    #    print >>file, " $\pm$ ", 
    #    print >>file, ("%.02f" % float(summaryTable[thisST]['errNeeAtZFast'])).rjust(14),
    #    print >>file, " & ", 
    #    print >>file, ("%.02f" % float(summaryTable[thisST]['NeeOffZFast'])).rjust(10),
    #    print >>file, " $\pm$ ", 
    #    print >>file, ("%.02f" % float(summaryTable[thisST]['errNeeOffZFast'])).rjust(10),
    #    print >>file, " & ", 
    print >>file, ("%.i" % float(summaryTable[thisST]['NeeAtZFull'])).rjust(6),
    #   print >>file, " $\pm$ ", 
    #    print >>file, ("%.02f" % float(summaryTable[thisST]['errNeeAtZFull'])).rjust(15),
    print >>file, " $\pm$ ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['errStatNeeAtZFull'])).rjust(6),
    print >>file, " & ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['RatioAtZOffZFast'])).rjust(6),
    print >>file, " $\pm$ ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['errRatioAtZOffZFast'])).rjust(6),
    print >>file, " & ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['NeeOffZEstimate'])).rjust(6),
    print >>file, " $\pm$ ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['errNeeOffZEstimate'])).rjust(6),
    print >>file, " & ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['NeeOffZFull'])).rjust(6),
    print >>file, " $\pm$ ", 
    print >>file, ("%.02f" % float(summaryTable[thisST]['errNeeOffZFull'])).rjust(6),
    print >>file, " \\\ "

#--> hline
print >>file, "\hline\hline"

#--> trailer
print >>file, "\end{tabular}"
print >>file, "\end{center}"
print >>file, "\caption{ADD TEXT IN THE CAPTION}"
print >>file, "\label{tab:XXXX}"
print >>file, "\end{table}"

#--> end document
print >>file, "\n"
print >>file, "\end{document}"
print >>file, "\n"

    
## PRINT ON SCREEN ##

print "\n"
print "STcut".rjust(6),
#print "NeeAtZFast".rjust(10),
#print "errNeeAtZFast".rjust(14),
print "NeeOffZFast".rjust(10),
print "errNeeOffZFast".rjust(10),
print "NeeAtZFull".rjust(10),
#print "errNeeAtZFull".rjust(15),
print "errStatNeeAtZFull".rjust(18),
print "RatioAtZOffZFast".rjust(20),
print "errRatioAtZOffZFast".rjust(23),
print "NeeOffZEstimate".rjust(17),
print "errNeeOffZEstimate".rjust(20),
print "NeeOffZFull".rjust(15),
print "errNeeOffZFull".rjust(15)

invRatio = TGraphErrors()
invRatio.SetMarkerStyle(20)
invRatio.SetMarkerColor(1)
invRatio.SetMarkerSize(1)

for j, line in enumerate(summaryTable):

    thisST = "sT"+str(j)
    
    print ("%.i" % float(summaryTable[thisST]['STcut'])).rjust(6),
#    print ("%.02f" % float(summaryTable[thisST]['NeeAtZFast'])).rjust(10),
#    print ("%.02f" % float(summaryTable[thisST]['errNeeAtZFast'])).rjust(14),
    print ("%.02f" % float(summaryTable[thisST]['NeeOffZFast'])).rjust(10),
    print ("%.02f" % float(summaryTable[thisST]['errNeeOffZFast'])).rjust(10),
    print ("%.i" % float(summaryTable[thisST]['NeeAtZFull'])).rjust(10),
#   print ("%.02f" % float(summaryTable[thisST]['errNeeAtZFull'])).rjust(15),
    print ("%.02f" % float(summaryTable[thisST]['errStatNeeAtZFull'])).rjust(18),
    print ("%.02f" % float(summaryTable[thisST]['RatioAtZOffZFast'])).rjust(20),
    print ("%.02f" % float(summaryTable[thisST]['errRatioAtZOffZFast'])).rjust(23),
    print ("%.02f" % float(summaryTable[thisST]['NeeOffZEstimate'])).rjust(17),
    print ("%.02f" % float(summaryTable[thisST]['errNeeOffZEstimate'])).rjust(20),
    print ("%.02f" % float(summaryTable[thisST]['NeeOffZFull'])).rjust(15),
    print ("%.02f" % float(summaryTable[thisST]['errNeeOffZFull'])).rjust(15)

    invRatio.SetPoint(j ,float(summaryTable[thisST]['STcut']) , float(summaryTable[thisST]['RatioAtZOffZFast']))
    invRatio.SetPointError(j , float(0) , float(summaryTable[thisST]['errRatioAtZOffZFast']))

c1 = TCanvas()
c1.SetGridx()
c1.SetGridy()

invRatio.GetXaxis().SetTitle("S_{T} cut (GeV)")
invRatio.GetYaxis().SetTitle("R_{OffZAtZ}^{-1}")
invRatio.GetXaxis().SetTitleOffset(1.1)
invRatio.GetYaxis().SetTitleOffset(1.2)
invRatio.RemovePoint(7)
invRatio.RemovePoint(8)
invRatio.Draw("ap")



## Terminate the program
print "Press ENTER to terminate"
wait=raw_input()
