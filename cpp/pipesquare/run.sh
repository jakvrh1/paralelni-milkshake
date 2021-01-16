#!/bin/bash

echo "Compiling"
g++ -std=c++11 -O2 -lpthread pipesquare.cpp -o pipesquare 

echo "Starting"
time ./pipesquare
# perf record ./pipesquare 
