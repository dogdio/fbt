#!/usr/bin/perl

my $CMD = shift;
my $DIR = shift;

sub print_and_exec
{
	my $str = shift;
	print $str;
	system $str;
}

sub Find
{
	my $arg = shift;
	foreach my $f(`find $DIR -name $arg`) {
		chop $f;
		my $str = "";
		$str = "ln -sf $f .\n";
		print_and_exec($str);
	}
}

if($CMD eq "clean") {
	print_and_exec("rm -f *.cpp\n");
	print_and_exec("rm -f *.h\n");
	exit;
}
elsif(($CMD eq "set") && (length($DIR) > 0)) {
	Find("\"*.cpp\"");
	Find("\"*.h\"");
}
else {
	print "Usage\n";
	print "  sym.pl clean\n";
	print "  sym.pl set ../xxx/yyy\n";
}



