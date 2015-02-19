# Exercise 10 - Keyword statistics
## Problem description
Given an alphabet and a list of keywords, count all occurrences of
both keywords and non-keywords, grouping the latter into groups
positioned lexicographically between the keywords.

###Example:

**keywords**: carrot, mushroom, train  
**non-keywords**: animation, ball, cartoon, crate, graph, mouse, name, park, zebra  

####Groups of non-keywords:  
words before **carrot:**  
- animation
- ball

words between **carrot** and **mushroom**  
- cartoon
- crate
- graph
- mouse

words between **mushroom** and **train**  
- name
- park

words after **train**  
- zebra  

## Technical requirements
The input is a file with the following structure:
````
[alphabet]
[number of keywords] [keywords]...
[text to process]
````

The following information should be printed to the standard output:
* number of distinct keywords
* all words total
* number of keyword occurrences
* percentage of keywords in the text
* number of non-keyword occurrences
* percentage of non-keywords in the text
* number of occurrences and percentage of all keywords
* number of occurrences and percentage of all non-keyword groups

### Sample input
````
abcdefghijklmnopqrstuvwxyz
12 begin end if then else while do repeat until array case of
program name;
    // lines of Pascal source code
end.
````

### Sample output
````
12              distinct keywords
1000            words total
272     27.2%   keywords total
728     72.8%   non-keywords total
45      4.5%
7       0.7%    array
14      1.4%
36      3.6%    begin
23      2.3%
4       0.4%    case
41      4.1%
27      2.7%    do
24      2.4%
14      1.4%    else
2       0.2%
40      4.0%    end
100     10.0%
30      3.0%    if
162     16.2%
11      1.1%    of
80      8.0%
23      2.3%    repeat
57      5.7%
30      3.0%    then
33      3.3%
23      2.3%    until
48      4.8%
27      2.7%    while
99      9.9%
````
