#!/bin/bash

cust_func()
{
    ./a.out $1 $2 $3 $4 $RANDOM $5
    sleep 1
}

g++ differential_evolution.cpp

for i in {1..30} 	
do
    cust_func $pop_size $dim $lb $ub $opt &
done

wait