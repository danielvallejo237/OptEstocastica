#!/bin/bash

file=pmed9.txt
g++ -std=c++11 -g localsearch.cpp

for i in {1..50}
do
	./a.out $file $RANDOM
done