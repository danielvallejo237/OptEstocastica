#!/usr/bin/env bash

FILE="pmed37.txt"

g++ tarea1.cpp -fopenmp

for seed in {1..100}
do
 ./a.out $FILE $seed
done
