#!/usr/bin/perl
use strict;
use warnings;

# How to use:
# ./datagen.pl [output file] [museum height] [museum width] [no of paths]
#
# All arguments are mandatory. This script generates a museum of given size
# and [no of paths] requests for paths in a format required by the 
# exercise specification.

my $range = 100;

die "Not enough arguments" if @ARGV < 4;
my ($filename, $height, $width, $tasks) = @ARGV;
open my $OUT, '>', $filename or die $!;

print $OUT $height, ' ', $width, "\n";
for (1..$height) {
	for (1..$width) {
		print $OUT int rand $range, ' ';
	}
	print $OUT "\n";
}

for (1 .. $tasks) {
	my $htest = int rand $height;
	my $wtest = int rand $width;
	if ($htest != 0 and $wtest != 0) {
		print $OUT "$htest $wtest\n";
	}
}

print $OUT "0 0\n";
