#!/bin/bash

g++ hw3_code.cpp -std=c++11

File="pmed37.txt"

for i in {1..50}
do
 ./a.out $File $RANDOM 1
done
