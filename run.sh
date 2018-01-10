#!/bin/bash

set -x

rm -f p2

gcc -o p2 p2.c

./p2 chr1.fa > statistic.txt

echo -n "=============================="
echo -n " head statistic.txt "
echo "=============================="

head statistic.txt

echo -n "=============================="
echo -n " tail statistic.txt "
echo "=============================="

tail -n 15 statistic.txt
