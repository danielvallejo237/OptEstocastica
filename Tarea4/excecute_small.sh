#!/bin/bash

file="pmed1.txt"

cust_func()
{
    ./a.out $1 $RANDOM
    sleep 1
}

g++ tarea4_recocidosimpmed.cpp -std=c++11

for i in {1..50}
do
    cust_func $file &
done

wait