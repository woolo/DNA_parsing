#!/bin/bash

# set -x

# rm -f p2

# gcc -o p2 p2.c

# ./p2 chr1.fa > student_result.txt

# echo -n "=============================="
# echo -n " head statistic.txt "
# echo "=============================="

# head statistic.txt

# echo -n "=============================="
# echo -n " tail statistic.txt "
# echo "=============================="

# tail -n 15 statistic.txt

word1=ATCGATCGAT
word2=ATCGATCGAA
word3=ATCGATCAAA

echo -n "================"
echo -n " diff my result with student result: $word1"
echo  "================"

diff <(cat student_result.txt | grep $word1) <(cat my_result.txt | grep $word1)

echo

echo -n "================"
echo -n " diff my result with student result: $word2"
echo  "================"

diff <(cat student_result.txt | grep $word2) <(cat my_result.txt | grep $word2)

echo

echo -n "================"
echo -n " diff my result with student result: $word3"
echo  "================"

diff <(cat student_result.txt | grep $word3) <(cat my_result.txt | grep $word3)

echo
