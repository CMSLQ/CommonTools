#!/usr/local/bin/perl

##input dir with files on castor
my $castorDir = "/castor/cern.ch/user/s/santanas/RootNtuples/Leptoquarks/CSA07Electron_CMSSW_1_6_12-CSA07-Gumbo-B1-PDElectron-ReReco-100pb-Skims7-susyHLTElectronPhoton-skimElectron";
 

##name of output dir
my $outputDirName="/home/santanas/Data/Leptoquarks/RootNtuples/CSA07Electron_CMSSW_1_6_12-CSA07-Gumbo-B1-PDElectron-ReReco-100pb-Skims7-susyHLTElectronPhoton-skimElectron";


##create output dir
system("mkdir $outputDirName");

##copy the files from castor to output dir
open (FILE, "rfdir $castorDir |");
while(<FILE>){
    my $file = $_; chomp $file;
    #print "$file\n";

    @array = split(/\s+/, $file);
    #print "$array[8]\n";

    my $currentFileName = $array[8]; 
    my $currentFile = $castorDir."/".$currentFileName;

    print ("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print ("copy of: $currentFileName\n");
    print ("from:    $castorDir\n");
    print ("to:      $outputDirName\n");
    print ("IN PROGRESS ......\n");

    #print  ("rfcp $currentFile $outputDirName\n");
    system ("rfcp $currentFile $outputDirName");
    
}

close(FILE);
