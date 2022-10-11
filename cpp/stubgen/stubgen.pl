#!/usr/bin/perl
#
# stub generator for C/C++
#
use Time::HiRes 'time';
use Getopt::Long;
use constant USAGEMSG => <<USAGE;

Usage: stubgen.pl <options>
  -w  [output file] dump function list by `nm -u`
  -r  [read file] load function list
  -f  [file] same as -r file, -w file
  -g  [output file] generate stub code
  -i  [path] include path (default: "./")
  -a  [name] arg name for stub function (default: "__x")
  -m  [string] only matches are read/written
  -d  [str] dump debug info. str = inc|var|method|slist|dump1|ret|arg|all
      'all' is output all info
  -v  enable variable parser(default disable). for only global variable

1st step: generate function list from object file
   stubgen.pl -w func.txt hoge1.o hoge2.o ,,,,

2nd step: generate stub code by header file
   stubgen.pl -r func.txt -g hoge.c -d var,method

Do 1 and 2 at once
   stubgen.pl -f func.txt -g hoge.c hoge.o

USAGE

my $ARG_Include = "./";
my $ARG_GenStub = "";
my $ARG_Dump = "";
my $ARG_Write = "";
my $ARG_Read = "";
my $ARG_File = "";
my $ARG_ArgName = "__x";
#my $ARG_Cpp = 0; # -x  enable c++ mode (exec c++filt)
my $ARG_Match = "";
my $ARG_VariableParse = 0;
die USAGEMSG unless GetOptions
(
 'w=s' => \$ARG_Write,
 'r=s' => \$ARG_Read,
 'f=s' => \$ARG_File,
 'i=s' => \$ARG_Include,
 'g=s' => \$ARG_GenStub,
 'a=s' => \$ARG_ArgName,
 'm=s' => \$ARG_Match,
# 'x' => \$ARG_Cpp,
 'v' => \$ARG_VariableParse,
 'd=s' => \$ARG_Dump,
 'h' => \$ARG_Help,
);

if($ARG_Help) {
	die USAGEMSG;
}

my %ignore_list;
$ignore_list{"stdin"} = 1;
$ignore_list{"stdout"} = 1;
$ignore_list{"stderr"} = 1;
$ignore_list{"abort"} = 1;
$ignore_list{"abs"} = 1;
$ignore_list{"acos"} = 1;
$ignore_list{"asctime"} = 1;
$ignore_list{"asctime_r"} = 1;
$ignore_list{"asin"} = 1;
$ignore_list{"assert"} = 1;
$ignore_list{"atan"} = 1;
$ignore_list{"atan2"} = 1;
$ignore_list{"atexit"} = 1;
$ignore_list{"atof"} = 1;
$ignore_list{"atoi"} = 1;
$ignore_list{"atol"} = 1;
$ignore_list{"bsearch"} = 1;
$ignore_list{"btowc"} = 1;
$ignore_list{"calloc"} = 1;
$ignore_list{"catclose"} = 1;
$ignore_list{"catgets"} = 1;
$ignore_list{"catopen"} = 1;
$ignore_list{"ceil"} = 1;
$ignore_list{"clearerr"} = 1;
$ignore_list{"clock"} = 1;
$ignore_list{"cos"} = 1;
$ignore_list{"cosh"} = 1;
$ignore_list{"ctime"} = 1;
$ignore_list{"ctime64"} = 1;
$ignore_list{"ctime64_r"} = 1;
$ignore_list{"ctime_r"} = 1;
$ignore_list{"difftime"} = 1;
$ignore_list{"difftime64"} = 1;
$ignore_list{"div"} = 1;
$ignore_list{"erf"} = 1;
$ignore_list{"erfc"} = 1;
$ignore_list{"exit"} = 1;
$ignore_list{"exp"} = 1;
$ignore_list{"fabs"} = 1;
$ignore_list{"fclose"} = 1;
$ignore_list{"fdopen"} = 1;
$ignore_list{"feof"} = 1;
$ignore_list{"ferror"} = 1;
$ignore_list{"fflush"} = 1;
$ignore_list{"fgetc"} = 1;
$ignore_list{"fgetpos"} = 1;
$ignore_list{"fgets"} = 1;
$ignore_list{"fgetwc"} = 1;
$ignore_list{"fgetws"} = 1;
$ignore_list{"fileno"} = 1;
$ignore_list{"floor"} = 1;
$ignore_list{"fmod"} = 1;
$ignore_list{"fopen"} = 1;
$ignore_list{"fprintf"} = 1;
$ignore_list{"fputc"} = 1;
$ignore_list{"fputs"} = 1;
$ignore_list{"fputwc"} = 1;
$ignore_list{"fputws"} = 1;
$ignore_list{"fread"} = 1;
$ignore_list{"free"} = 1;
$ignore_list{"freopen"} = 1;
$ignore_list{"frexp"} = 1;
$ignore_list{"fscanf"} = 1;
$ignore_list{"fseek"} = 1;
$ignore_list{"fsetpos"} = 1;
$ignore_list{"ftell"} = 1;
$ignore_list{"fwide"} = 1;
$ignore_list{"fwprintf"} = 1;
$ignore_list{"fwrite"} = 1;
$ignore_list{"fwscanf"} = 1;
$ignore_list{"gamma"} = 1;
$ignore_list{"getc"} = 1;
$ignore_list{"getchar"} = 1;
$ignore_list{"getenv"} = 1;
$ignore_list{"gets"} = 1;
$ignore_list{"getwc"} = 1;
$ignore_list{"getwchar"} = 1;
$ignore_list{"gmtime"} = 1;
$ignore_list{"gmtime64"} = 1;
$ignore_list{"gmtime64_r"} = 1;
$ignore_list{"gmtime_r"} = 1;
$ignore_list{"hypot"} = 1;
$ignore_list{"isalnum"} = 1;
$ignore_list{"isalpha"} = 1;
$ignore_list{"isascii"} = 1;
$ignore_list{"isblank"} = 1;
$ignore_list{"iscntrl"} = 1;
$ignore_list{"isdigit"} = 1;
$ignore_list{"isgraph"} = 1;
$ignore_list{"islower"} = 1;
$ignore_list{"isprint"} = 1;
$ignore_list{"ispunct"} = 1;
$ignore_list{"isspace"} = 1;
$ignore_list{"isupper"} = 1;
$ignore_list{"iswalnum"} = 1;
$ignore_list{"iswalpha"} = 1;
$ignore_list{"iswblank"} = 1;
$ignore_list{"iswcntrl"} = 1;
$ignore_list{"iswctype"} = 1;
$ignore_list{"iswdigit"} = 1;
$ignore_list{"iswgraph"} = 1;
$ignore_list{"iswlower"} = 1;
$ignore_list{"iswprint"} = 1;
$ignore_list{"iswpunct"} = 1;
$ignore_list{"iswspace"} = 1;
$ignore_list{"iswupper"} = 1;
$ignore_list{"iswxdigit"} = 1;
$ignore_list{"isxdigit"} = 1;
$ignore_list{"j0"} = 1;
$ignore_list{"j1"} = 1;
$ignore_list{"jn"} = 1;
$ignore_list{"labs"} = 1;
$ignore_list{"ldexp"} = 1;
$ignore_list{"ldiv"} = 1;
$ignore_list{"localeconv"} = 1;
$ignore_list{"localtime"} = 1;
$ignore_list{"localtime64"} = 1;
$ignore_list{"localtime64_r"} = 1;
$ignore_list{"localtime_r"} = 1;
$ignore_list{"log"} = 1;
$ignore_list{"log10"} = 1;
$ignore_list{"longjmp"} = 1;
$ignore_list{"malloc"} = 1;
$ignore_list{"mblen"} = 1;
$ignore_list{"mbrlen"} = 1;
$ignore_list{"mbrtowc"} = 1;
$ignore_list{"mbsinit"} = 1;
$ignore_list{"mbsrtowcs"} = 1;
$ignore_list{"mbstowcs"} = 1;
$ignore_list{"mbtowc"} = 1;
$ignore_list{"memchr"} = 1;
$ignore_list{"memcmp"} = 1;
$ignore_list{"memcpy"} = 1;
$ignore_list{"memmove"} = 1;
$ignore_list{"memset"} = 1;
$ignore_list{"mktime"} = 1;
$ignore_list{"mktime64"} = 1;
$ignore_list{"modf"} = 1;
$ignore_list{"nextafter"} = 1;
$ignore_list{"nextafterl"} = 1;
$ignore_list{"nexttoward"} = 1;
$ignore_list{"nexttowardl"} = 1;
$ignore_list{"nl_langinfo"} = 1;
$ignore_list{"perror"} = 1;
$ignore_list{"pow"} = 1;
$ignore_list{"printf"} = 1;
$ignore_list{"putc"} = 1;
$ignore_list{"putchar"} = 1;
$ignore_list{"putenv"} = 1;
$ignore_list{"puts"} = 1;
$ignore_list{"putwc"} = 1;
$ignore_list{"putwchar"} = 1;
$ignore_list{"qsort"} = 1;
$ignore_list{"quantexpd128"} = 1;
$ignore_list{"quantexpd32"} = 1;
$ignore_list{"quantexpd64"} = 1;
$ignore_list{"quantized128"} = 1;
$ignore_list{"quantized32"} = 1;
$ignore_list{"quantized64"} = 1;
$ignore_list{"raise"} = 1;
$ignore_list{"rand"} = 1;
$ignore_list{"rand_r"} = 1;
$ignore_list{"realloc"} = 1;
$ignore_list{"regcomp"} = 1;
$ignore_list{"regerror"} = 1;
$ignore_list{"regexec"} = 1;
$ignore_list{"regfree"} = 1;
$ignore_list{"remove"} = 1;
$ignore_list{"rename"} = 1;
$ignore_list{"rewind"} = 1;
$ignore_list{"samequantumd128"} = 1;
$ignore_list{"samequantumd32"} = 1;
$ignore_list{"samequantumd64"} = 1;
$ignore_list{"scanf"} = 1;
$ignore_list{"setbuf"} = 1;
$ignore_list{"setjmp"} = 1;
$ignore_list{"setlocale"} = 1;
$ignore_list{"setvbuf"} = 1;
$ignore_list{"signal"} = 1;
$ignore_list{"sin"} = 1;
$ignore_list{"sinh"} = 1;
$ignore_list{"snprintf"} = 1;
$ignore_list{"sprintf"} = 1;
$ignore_list{"sqrt"} = 1;
$ignore_list{"srand"} = 1;
$ignore_list{"sscanf"} = 1;
$ignore_list{"strcasecmp"} = 1;
$ignore_list{"strcat"} = 1;
$ignore_list{"strchr"} = 1;
$ignore_list{"strcmp"} = 1;
$ignore_list{"strcoll"} = 1;
$ignore_list{"strcpy"} = 1;
$ignore_list{"strcspn"} = 1;
$ignore_list{"strerror"} = 1;
$ignore_list{"strfmon"} = 1;
$ignore_list{"strftime"} = 1;
$ignore_list{"strlen"} = 1;
$ignore_list{"strncasecmp"} = 1;
$ignore_list{"strncat"} = 1;
$ignore_list{"strncmp"} = 1;
$ignore_list{"strncpy"} = 1;
$ignore_list{"strpbrk"} = 1;
$ignore_list{"strptime"} = 1;
$ignore_list{"strrchr"} = 1;
$ignore_list{"strspn"} = 1;
$ignore_list{"strstr"} = 1;
$ignore_list{"strtod"} = 1;
$ignore_list{"strtod128"} = 1;
$ignore_list{"strtod32"} = 1;
$ignore_list{"strtod64"} = 1;
$ignore_list{"strtof"} = 1;
$ignore_list{"strtok"} = 1;
$ignore_list{"strtok_r"} = 1;
$ignore_list{"strtol"} = 1;
$ignore_list{"strtold"} = 1;
$ignore_list{"strtoul"} = 1;
$ignore_list{"strxfrm"} = 1;
$ignore_list{"swprintf"} = 1;
$ignore_list{"swscanf"} = 1;
$ignore_list{"system"} = 1;
$ignore_list{"tan"} = 1;
$ignore_list{"tanh"} = 1;
$ignore_list{"time"} = 1;
$ignore_list{"time64"} = 1;
$ignore_list{"tmpfile"} = 1;
$ignore_list{"tmpnam"} = 1;
$ignore_list{"toascii"} = 1;
$ignore_list{"tolower"} = 1;
$ignore_list{"toupper"} = 1;
$ignore_list{"towctrans"} = 1;
$ignore_list{"towlower"} = 1;
$ignore_list{"towupper"} = 1;
$ignore_list{"ungetc"} = 1;
$ignore_list{"ungetwc"} = 1;
$ignore_list{"va_arg"} = 1;
$ignore_list{"va_copy"} = 1;
$ignore_list{"va_end"} = 1;
$ignore_list{"va_start"} = 1;
$ignore_list{"vfprintf"} = 1;
$ignore_list{"vfscanf"} = 1;
$ignore_list{"vfwprintf"} = 1;
$ignore_list{"vfwscanf"} = 1;
$ignore_list{"vprintf"} = 1;
$ignore_list{"vscanf"} = 1;
$ignore_list{"vsnprintf"} = 1;
$ignore_list{"vsprintf"} = 1;
$ignore_list{"vsscanf"} = 1;
$ignore_list{"vswprintf"} = 1;
$ignore_list{"vswscanf"} = 1;
$ignore_list{"vwprintf"} = 1;
$ignore_list{"vwscanf"} = 1;
$ignore_list{"wcrtomb"} = 1;
$ignore_list{"wcscat"} = 1;
$ignore_list{"wcschr"} = 1;
$ignore_list{"wcscmp"} = 1;
$ignore_list{"wcscoll"} = 1;
$ignore_list{"wcscpy"} = 1;
$ignore_list{"wcscspn"} = 1;
$ignore_list{"wcsftime"} = 1;
$ignore_list{"wcslen"} = 1;
$ignore_list{"wcslocaleconv"} = 1;
$ignore_list{"wcsncat"} = 1;
$ignore_list{"wcsncmp"} = 1;
$ignore_list{"wcsncpy"} = 1;
$ignore_list{"wcspbrk"} = 1;
$ignore_list{"wcsptime"} = 1;
$ignore_list{"wcsrchr"} = 1;
$ignore_list{"wcsrtombs"} = 1;
$ignore_list{"wcsspn"} = 1;
$ignore_list{"wcsstr"} = 1;
$ignore_list{"wcstod"} = 1;
$ignore_list{"wcstod128"} = 1;
$ignore_list{"wcstod32"} = 1;
$ignore_list{"wcstod64"} = 1;
$ignore_list{"wcstof"} = 1;
$ignore_list{"wcstok"} = 1;
$ignore_list{"wcstol"} = 1;
$ignore_list{"wcstold"} = 1;
$ignore_list{"wcstombs"} = 1;
$ignore_list{"wcstoul"} = 1;
$ignore_list{"wcsxfrm"} = 1;
$ignore_list{"wctob"} = 1;
$ignore_list{"wctomb"} = 1;
$ignore_list{"wctrans"} = 1;
$ignore_list{"wctype"} = 1;
$ignore_list{"wcwidth"} = 1;
$ignore_list{"wmemchr"} = 1;
$ignore_list{"wmemcmp"} = 1;
$ignore_list{"wmemcpy"} = 1;
$ignore_list{"wmemmove"} = 1;
$ignore_list{"wmemset"} = 1;
$ignore_list{"wprintf"} = 1;
$ignore_list{"wscanf"} = 1;
$ignore_list{"y0"} = 1;
$ignore_list{"y1"} = 1;
$ignore_list{"yn"} = 1;
$ignore_list{"_GLOBAL_OFFSET_TABLE_"} = 1;

# c++
$ignore_list{""} = 1;
$ignore_list{"_Unwind_Resume"} = 1;
$ignore_list{"__dso_handle"} = 1;
$ignore_list{"__gxx_personality_v0"} = 1;
$ignore_list{"__dynamic_cast"} = 1;

my $ignore_list2 = "^operator delete|^operator new|^std::|^vtable for __cxx|^__cxa";

my @target_file = @ARGV;
my %symbol_list;
my %func_list;
my %local_include;
my @nm_scope;
my $scope_class; # FIXME class of class
my @scope_ns1;
my @scope_ns2;
my $current_file;

sub Dump
{
	my $pattern = shift;
	my $buf = shift;

	if($ARG_Dump =~/$pattern/ || $ARG_Dump eq "all") {
		print "$buf\n";
	}
}

sub Dump1
{
	if($ARG_Dump =~/dump1/ || $ARG_Dump eq "all") {
		print "\n===== symbol_list \n";
		foreach my $k (sort keys %symbol_list) {
			print "  ## $k = $symbol_list{$k}\n";
		}
    
		print "\n===== include search path = $ARG_Include\n";
		foreach my $k(sort keys %local_include) {
			print "  $k = $local_include{$k}\n";
		}
	}
}

sub open_include_file
{
	my $file = shift;
	my $ret = "";
	$current_file = $file;

	# delete preprocessor, comment //~~~~
	open(FH3, "< $file") or die;
	while(<FH3>) {
		my $str = $_;
		$str =~ s/\/\/.*//;
		$str =~ s/#\s*if.*//;
		$str =~ s/#\s*el.*//;
		$str =~ s/#\s*endif.*//;
		$str =~ s/#\s*define.*//;
		$str =~ s/#\s*include.*//;
		$ret .= $str;
	}
	close(FH3) or die;

	# delete comment /* ~~~ */ 
	$ret =~ s {
		/\*
		.*?
		\*/
	}[]gsx;

#	print ">>>>>>>>>>>>>>>>>>\n$ret\n<<<<<<<<<<<<<<<<<<<\n";
	Dump("inc", "$file: ", length($ret));
	return $ret;
}

my $arg_num = 0;
sub parse_args_tv
{
	my $buf = shift;
	my $num = 0;
	my $ret = "";
	my $arg_name = $ARG_ArgName . $arg_num;

	Dump("arg", "==> $buf <==");
	if($buf =~ /[\<\>]/) { # template arguments
		$arg_name = "";
	}

	$buf =~ s/^\s+//g;
	$buf =~ s/\s+$//g;
	my @str = reverse(split(/\s+/, $buf));
	my $len = @str;

	if($str[0] =~ /^\*+$/) { # omit arg name(int *)
		$ret = "$buf". "$arg_name";
	}
	elsif($len == 1) {
		if($str[0] eq "void") { # arg is (void), keep it
			$ret = "$buf";
		}
		else { # omit arg name (int)
			$ret = "$buf ". "$arg_name";
		}
	}
	elsif($len >= 2 &&
			$str[1] =~ /^(struct|union|unsigned|const)$/) { # omit arg name (const int)
		$ret = "$buf ". "$arg_name";
	}
	else { # not omit (int value), keep it
		$ret = "$buf";
	}

	if($arg_num > 0) {
		$ret = ", " . $ret;
	}
	$arg_num++;
#	print "# $ret#\n";
	return $ret;
}

sub parse_args
{
	my $buf = shift;
	$buf =~ s/\x0a/ /g;
	$buf =~ s/\x0d//g;
	$buf =~ s/\brestrict\b//g;
	$buf =~ s/\b__restrict\b//g;

	my $ret = "";
	my $depth = 0;
	my $arg = "";
	my $prev = "";
	$arg_num = 0;
	foreach my $s (split(//, $buf)) {
		if($s eq "\t") { $s = " "; }
		if($s eq " " && $prev eq $s) { next; }

		if($s eq "(") {
			$depth++;
			if($depth == 1) { next; } # ignore 1st '('
		}
		elsif($s eq ")") {
			$depth--;
			if($depth == 0) { # detect last ')'
				$ret .= parse_args_tv($arg);
				$arg = "";
				last;
			}
		}

		if($s eq "," && $depth == 1) {
			$ret .= parse_args_tv($arg);
			$arg = "";
		}
		elsif($depth > 0) {
			$arg .= $s
		}
		$prev = $s;
	}

	return "(" . $ret . ");";
}

sub parse_return
{
	my $buf = shift;
	my $num = 0;
	my $ret = "";
	$buf =~ s/\x0a/ /g;
	$buf =~ s/\x0d//g;

	my @str = split(/\s+/, $buf);
	foreach my $s (reverse @str) {
		if($num == 0) {
			if($s =~ /^\*+$/) {
				$ret = $s;
			}
			elsif($s eq '&') {
				$ret = $s;
			}
			else {
				$ret = "$s $ret";
				$num++;
			}
		}
		else {
			if($s =~ /^(struct|union|unsigned|signed|const)$/) {
				$ret = "$s $ret";
			}
			elsif($s =~ /[\<\>]/) {
				$ret = "$s $ret";
			}
			$num++;
		}
	}
	Dump("ret", "--> $ret <--");

	return $ret;
}

sub output_stub
{
	# stub function
	my $f_ret  = shift;
	my $f_name = shift;
	my $f_args = shift;
	my $static = "";
	my $val = $f_ret;

	print FH_CPP "$f_ret $scope_class$f_name$f_args\n";

	if($f_ret =~ /&/) {
		$static = "static ";
		$val =~ s/\s*&\s*//g;
	}

	if(($f_ret eq "void") || ($f_ret eq "")) {
		print FH_CPP "{\n\treturn;\n}\n\n";
	}
	elsif($f_ret =~ /\*/) {
		print FH_CPP "{\n\treturn NULL;\n}\n\n";
	}
	elsif($f_ret eq "bool") {
		print FH_CPP "{\n\treturn false;\n}\n\n";
	}
	else {
		print FH_CPP "{\n\t$static$val ret;\n\treturn ret;\n}\n\n";
	}
}

sub output_ns
{
	foreach my $ns1(@scope_ns1) {
		print FH_CPP "$ns1\n";
	}
	@scope_ns1 = ();
}

sub output_include
{
	if($local_include{$current_file} != 1) {
		my $f = $current_file;
		$f =~ s/.*\///;
		print FH_CPP "#include \"$f\"\n";
		$local_include{$current_file} = 1;
	}
}

my $prev_time = 0;
sub logging
{
	my $buf = shift;
	my $now = time;
	my $delta = $now - $prev_time;

	if($prev_time == 0) {
		$delta = 0;
	}

	printf "[%.6f] ", $delta;
	print "$buf\n";
	$prev_time = $now;
}

sub parse_method
{
	my $buf = shift;
	my $scope_str = shift;

	if($buf =~ /^([\w_\s:\<\>,]*[&\s\*]+)([\w_~]+)\s*(\([\w_,\s\*\(\)\[\]\<\>:=&]*\))$/) {
		Dump("method", "!!! $scope_str, $1| $2| $3");
		if($symbol_list{"$scope_str$2"} != 1) { next; }

		my $ret = parse_return($1);
		my $name = $2;
		my $args = parse_args($3);

		$args =~ s/;//;
		$args =~ s/\s*=\s[\w_:]+//g; # delete init value,  '= 100'
		$ret =~ s/\s*$//g;
		$ret =~ s/^\s*//g;

		output_include();
		output_ns();
		output_stub($ret, $name, $args);
	}
}

sub parse_variable
{
	my $buf = shift;
	my $scope_str = shift;

	if($buf =~ /^([\w_\s:]*[\s\*]+)([\w_]+)\s*([\w_\s\[\]:]*)$/) {
		Dump("var", "### $scope_str, $1| $2| $3");
		if($symbol_list{"$scope_str$2"} != 1) { next; }

		my $ret = parse_return($1);
		my $name = $2;
		my $array = $3;

		$array =~ s/;//;
		$ret =~ s/\s*$//g;
		$ret =~ s/^\s*//g;

		output_include();
		output_ns();
		print FH_CPP "$ret $name$array = {};\n";
	}
}

sub parse_mv
{
	my $buf = shift;
	my $num = shift;
	my $scope_str = "";
	Dump("mv", "??? $num\n$buf");

	$buf =~ s/public://g;
	$buf =~ s/^[\s\{]//g;
	foreach my $s(@nm_scope) {
		$scope_str .= "$s" ."::";
	}

	while($buf =~ /([^;]+);/g) {
		my $s = $1;
		$s =~ s/\s*$//g;
		parse_method($s, $scope_str);

		if($ARG_VariableParse == 1) {
			parse_variable($s, $scope_str);
		}
	}
}

sub parse_bracket
{
	my $buf = shift;
	my $depth = 0;
	my $len = 0;
	my $str = "";

	foreach my $s (split(//, $buf)) {
		$len++;
		$str .= $s;
		if($s eq "{") {
			$depth++;
		}
		elsif($s eq "}") {
			$depth--;
			if($depth == 0) { # detect last '}'
				last;
			}
		}
	}

	while(1) {
		$str = parse_class($str);
		if(length($str) == 0) {
			last;
		}
	}
	return substr($buf, $len);
}

sub parse_class
{
	my $buf = shift;
	my $nsc = "";

	if($buf =~ /\b(class|namespace)\s+([\w_]+)\s*\{/g) {
		my $index = pos($buf) - 1;
		my $str = substr($buf, $index); # skip until class/namespace

		parse_mv(substr($buf, 0, $index), "1");

		push(@nm_scope, "$2");
		if($1 eq "class") {
			$scope_class .= "$2::";
			$nsc = "class";
		}
		if($1 eq "namespace") {
			push(@scope_ns1, "namespace $2 {");
			push(@scope_ns2, "} // $2");
			$nsc = "ns";
		}
		$buf = parse_bracket($str);
	}
	else {
#		print "\n############\n##@scope_ns1\n##@scope_ns2\n$buf";
		parse_mv($buf, "2");
		return "";
	}

	pop(@nm_scope);
	if($nsc eq "class") {
		$scope_class = "";
	}
	elsif($nsc eq "ns") {
		my $ns1 = @scope_ns1;
		my $ns2 = pop(@scope_ns2);
		if(length($ns2) != 0 && $ns1 == 0) {
			print FH_CPP "$ns2\n";
		}
		pop(@scope_ns1);
	}

	return $buf;
}

sub output_func_list_cpp
{
	open(FH_CPP, "> $ARG_GenStub") or die;
	my $include_files = `find $ARG_Include -name "*.h"`;

	foreach my $file(split("\n", $include_files)) {
		my $buf = open_include_file($file);

		while(1) {
			$buf = parse_class($buf);
			if(length($buf) == 0) {
				last;
			}
		}
	}
	close(FH_CPP) or die;
	print "generate: $ARG_GenStub\n";
}

sub read_sym_list
{
	open(FH2, "< $ARG_Read") or die;

	while(<FH2>) {
		my $name = $_;
		$name =~ s/\x0d//g;
		$name =~ s/\x0a//g;
		$name =~ s/#.*//;
		if(length($name) == 0) { next; }

		if(length($ARG_Match) == 0 || $name =~ /$ARG_Match/) {
			$symbol_list{$name} = 1;
		}
	}

	close(FH2) or die;
}

sub write_sym_list
{
	my $opt = "";
#	if($ARG_Cpp == 1) {
		$opt = "| c++filt";
#	}

	open(FH1, "> $ARG_Write") or die;
	
	foreach my $o(@target_file) {
		# symbol list generated from Object File
		unless($o =~ /(\.o|\.so|\.a)$/) { next; }

		print FH1 "# nm -u $o $opt \n";
		my $list = `nm -u $o $opt | sort | uniq`;
		foreach my $line(split("\n", $list)) {
			Dump("slist", "$o: $line");
			if($line =~ /^\s+U.(.+)/) {
#			if($line =~ /^\s+U.([\w_:]+)/) {
				my $name = $1;
				$name =~ s/\[abi:cxx..\]//;
				if($ignore_list{$name} == 1) { next; }
				if($name =~ /$ignore_list2/) { next; }

				if(length($ARG_Match) == 0 || $name =~ /$ARG_Match/) {
					$name =~ s/(^[^\(]+)\(.*/$1/;
					print FH1 "$name\n";
				}
			}
		}
	}
	close(FH1) or die;
}



########################### main ###########################
if(length($ARG_File) != 0) {
	$ARG_Read = $ARG_File;
	$ARG_Write = $ARG_File;
}
elsif(length($ARG_Read) != 0) { }
elsif(length($ARG_Write) != 0) { }
else { die USAGEMSG; }

if(length($ARG_Write) != 0) {
	write_sym_list();
}

if(length($ARG_Read) != 0) {
	read_sym_list();
	output_func_list_cpp();
	Dump1();
}

