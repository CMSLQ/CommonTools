#!/bin/bash

usage ()
{
        echo "Usage:   $0 -p filepath -t tagForQueries "
        echo "Example: $0 -p /castor/cern.ch/user/s/santanas/FullSim/LQ1stgenM650/RECO -t myfiles"
        exit 1;
}

if [ $# -ne 4 ]; then usage ; fi

while [ $# -gt 0 ]; # till there are parameters
do    
  case "$1" in
    -p) filepath="$2"; shift ;;
    -t) tag="$2"; shift ;;
    *) usage ;;
  esac
  shift  # get following parameters
done

#echo `rfdir $filepath | awk -v filepath=$filepath '{print "-M " filepath $9}'`

files=`rfdir $filepath | awk '{print $9}'`

for file in $files
do
  cmd="stager_get -M $filepath/$file -U $tag"
  echo "Executing $cmd ..."
  $cmd
done

echo
echo "To query status of staging use: stager_qry -U $tag"
echo

