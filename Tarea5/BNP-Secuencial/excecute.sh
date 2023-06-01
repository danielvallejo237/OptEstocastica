#!/bin/bash

cust_func()
{
    ./a.out N = $1 pc = $2 pm = 1 finalTime = 43200 seed = $RANDOM fileName = $3 outputFile = $4
    sleep 1
}

g++ main.cpp MA.cpp Problem.cpp utils.cpp Individual.cpp -std=c++11 

for i in {1..5}
do
    cust_func 50 0.95 "pmed1.txt" "bnp_pmed1_run_${i}" &
done

for i in {1..5}
do
    cust_func 50 0.95 "pmed9.txt" "bnp_pmed9_run_${i}" &
done

for i in {1..5}
do
    cust_func 50 0.95 "pmed15.txt" "bnp_pmed15_run_${i}" &
done 

wait
