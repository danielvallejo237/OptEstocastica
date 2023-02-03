#!/usr/bin/env bash

FILE="pmed9.txt"

g++ tarea1.cpp

for seed in {1..100}
do
  ./a.out $FILE $seed
done
