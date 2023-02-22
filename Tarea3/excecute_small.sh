#!/bin/bash

g++ hw3_code.cpp -std=c++11

File="pmed1.txt"

for i in {1..50}
do
 ./a.out $File $RANDOM 0
done
