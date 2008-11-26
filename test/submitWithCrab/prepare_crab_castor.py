#!/usr/bin/env python

import string
import re
import imp
import os

# parsing the input parameters
import sys
args = sys.argv

debug = True


sample_filename = args[1]
#root_dir = args[2]

samplefile = open ( sample_filename, 'r')
lines = samplefile.readlines()


## root dir for output and log
#rootdir =  "/u1/santanas/Leptoquark/RootNtuples/CSA07Soups"
rootdircastor =  "/castor/cern.ch/user/s/santanas/RootNtuples/Leptoquarks/CSA07Electron_CMSSW_1_6_7-CSA07-Chowder-A3-PDElectron-ReReco-100pb-Skims7-susyHLTElectronPhoton-skimElectron"

#if not os.access(rootdir,os.F_OK) :
#  print "bad directory %s ! exiting..." % (rootdir)
#  sys.exit()

## template cfg file
cfg_name = "template_rtuple.cfg"
cfg_templ = open(cfg_name,'r')
cfg_lines = cfg_templ.readlines()

### template crab cfg file
crab_name = "template_crab_rtuple_castor.cfg"
crab_templ = open(crab_name,'r')
crab_lines = crab_templ.readlines()



# expr to change number of events in dumper
eventsExp = re.compile(r'untracked int32 numEvents = \d+')
outputExp = re.compile(r'untracked string rootfile = \".*\"')

sizeof = { }
fullname = { }

slashExp = re.compile(r'/(.*)/.*')

for line in lines :
  v1 = line.split()
  if len(v1) != 2 :
    print "bad line! skipping"
    continue


  # replace / with _ to prepare actual cfg and crab files
  v2 = slashExp.split(v1[0])
  #print v2[1]
  sample = re.sub(r'/','_', v2[1])

  fullname[sample] = v1[0]

  # assign size
  sizeof[sample] = v1[1]

  print "sample: %s\t events: %i" % (sample,int(v1[1]))
  print "---------------------------------------------------------------------------------------"


  # prepare rtuple cfg file
  cfgfile_name =  "rtuple_castor_" + sample + ".cfg"
  rootfile_name = "rtuple_" + sample + ".root"
  #outputdir = rootdir+"/"+sample+"/output"
  #logdir = rootdir+"/"+sample+"/log"

  # create cfg file for each sample
  cfgfile = open( cfgfile_name, 'w')

  for l in cfg_lines :

    # replace name of root tuple
    if  outputExp.search(l) :
      newl = re.sub(r'".+"', "\""+rootfile_name+"\"", l)
      if(debug) : print "--> %s" % (newl)


    # replace number of events
    elif eventsExp.search(l) :
      newl = re.sub(r'=\s+\d+', "= " + sizeof[sample], l)
      if(debug) : print "--> %s" % (newl)

    else :
      newl = l

    cfgfile.write(newl)

  cfgfile.close()


  # create crab cfg files
  #
  crabfile = open( "crab_rtuple_castor_" + sample + ".cfg", 'w')

  for l in crab_lines :

    if re.match(r'^datasetpath=.*',l) :
      newl = re.sub(r'^datasetpath\s*=.*',"datasetpath="+fullname[sample],l)
      if(debug) : print "--> %s" % (newl)

    elif re.match(r'^pset=.*',l) :
      newl = re.sub(r'^pset\s*=.*',"pset="+cfgfile_name,l)
      if(debug) : print "--> %s" % (newl)

    elif re.match(r'^output_file\s*=.*',l) :
      newl = re.sub( r'^output_file\s*=.*', "output_file="+rootfile_name, l )
      if(debug) : print "--> %s" % (newl)

#    elif re.match(r'^outputdir\s*=.*',l) :
#      newl = re.sub( r'^outputdir\s*=.*', "outputdir="+rootdir+"/"+sample+"/output", l )
#      if(debug) : print "--> %s" % (newl)

#    elif re.match(r'^logdir\s*=.*',l) :
#      newl = re.sub( r'^logdir\s*=.*', "logdir="+rootdir+"/"+sample+"/log", l )
#      if(debug) : print "--> %s" % (newl)

    elif re.match(r'^storage_path\s*=.*',l) :
      newl = re.sub( r'^storage_path\s*=.*', "storage_path=/srm/managerv2?SFN="+rootdircastor, l )
      if(debug) : print "--> %s" % (newl)


    else :
      newl = l

    crabfile.write(newl)
  crabfile.close()


  # create dirs
  #os.makedirs(outputdir)
  #os.makedirs(logdir)


