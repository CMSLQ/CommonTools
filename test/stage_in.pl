#!/usr/local/bin/perl -w
use strict;

my $castorHome = $ENV{CASTOR_HOME};

my $inputdir = "$castorHome/FullSim/LQ1stgenM650/DIGI2RAW";
my $listStagedFiles = "stagedFiles.txt";


my $tmpOutputFile = "out.txt";
system("rm -f $tmpOutputFile");

open(LISTSTAGEDFILES,">$listStagedFiles");
print LISTSTAGEDFILES "replace PoolSource.fileNames = {\n";

open(CASTORFILE, "rfdir $inputdir |");
while(<CASTORFILE>)
{
    my $line = $_; chomp $line;
    my @array = split(/\s+/, $line);
    my $currentfile="$inputdir/$array[8]";
    system("stager_qry -M $currentfile \> $tmpOutputFile \n");

    my $isStaged=0;
    my $isStagin=0;
    my $NeedStageget=0;

    open(FILE,"<$tmpOutputFile");
    while(<FILE>)
    {
	my $x=$_; 
	chomp $x;
	#print("$x\n");
	
	if($x=~/STAGED/)
	{
	    $isStaged=1;
	}
	if($x=~/STAGEIN/)
	{
	    $isStagin=1;
	}
	if($x=~/Error/)
	{
	    $NeedStageget=1;
	    print "### stager_qry output ###\n";
	    print "$x\n";
	    print "#########################\n";

            ## stager_get
	    print("stager_get -M $currentfile \n");
	    system("stager_get -M $currentfile");
	    print ("\n");
	}
	
    }
    close(FILE);

    if($isStagin==1)
    {
	print ("$currentfile is still STAGEIN\n");
    }
    
    if($isStaged==1)
    {
	print ("$currentfile is STAGED\n");
	print LISTSTAGEDFILES "\'file:rfio:$currentfile\' \,\n";
    }

 }
close (CASTORFILE);

system("rm -f $tmpOutputFile");

print LISTSTAGEDFILES "}\n";
close (LISTSTAGEDFILES);

print ("\n");
print ("$listStagedFiles  created\n");
print ("ATTENTION - REMOVE THE \, FROM THE LAST LINE OF $listStagedFiles \n");
