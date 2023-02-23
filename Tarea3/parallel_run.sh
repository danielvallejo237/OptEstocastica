#!/bin/bash

g++ hw3_code.cpp -std=c++11 -g

cust_func()
{
    ./a.out $1 $RANDOM 1
    sleep 1
}

for i in {1..50}
do
    cust_func $file &
done

wait
