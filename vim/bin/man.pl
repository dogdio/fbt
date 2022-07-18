#!/usr/bin/perl

my $arg = shift;
my $buf = `man 2 $arg`;
if(length($buf) == 0) {
	$buf = `man 3 $arg`;
}
open(FH, "> $ENV{HOME}/.vim/mmm") or die;
print FH $buf;

close(FH);

