#!/usr/local/bin/perl

open (FILE, "ls crab_rtuple_* |");
while(<FILE>){
    my $file = $_; chomp $file;
    #print "$file\n";

    system ("crab -create -submit -cfg $file");
    print ("crab -create -submit -cfg $file\n");
}
close(FILE);
