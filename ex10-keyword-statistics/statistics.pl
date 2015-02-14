#!/usr/bin/perl
use strict;
use warnings;
use feature 'say';

sub percent { return sprintf "%4.1f%%", ($_[0] / $_[1]) * 100; }

sub display_keyword_data {
    my ($occur, $total, $word) = @_;
    say "$occur\t", percent($occur, $total), "\t", $word // '';
}

chomp(my $alphabet = <>);
my @keywords = split /\s+/, <>;
shift @keywords;
@keywords = sort @keywords;
my @pre_keyword = map { 0 } @keywords;
my %kw_counter = map { $_ => 0 } @keywords;

my ($words_total, $keywords_total) = (0, 0);
while (<>) {
    my @words = split /[^$alphabet]+|\s+/;
    $words_total += @words;
    foreach my $word (@words) {
        if (exists $kw_counter{$word}) {
            ++$kw_counter{$word};
            ++$keywords_total;
        } else {
            my $index = 0;
            ++$index while ($index < @keywords and $word gt $keywords[$index]);
            ++$pre_keyword[$index];
        }
    }
}

say 'Wersja AK';
say scalar @keywords, "\t\tróżnych słów kluczowych";
say $words_total, "\t\twystąpień wszystkich słów";
say $keywords_total, "\t", percent($keywords_total, $words_total),
  "\twystąpień słów kluczowych";
my $non_keywords = $words_total - $keywords_total;
say $non_keywords, "\t", percent($non_keywords, $words_total),
  "\twystąpień innych słów";
foreach (keys @keywords) {
    display_keyword_data $pre_keyword[$_], $words_total;
    display_keyword_data $kw_counter{ $keywords[$_] }, $words_total,
      $keywords[$_];
}
display_keyword_data $pre_keyword[$#pre_keyword], $words_total;
