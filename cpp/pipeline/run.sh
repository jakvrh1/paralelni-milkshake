#!/bin/bash

mkdir -p ../../out

echo "Compiling"
g++ -std=c++11 -O2 -lpthread pipeline.cpp -o pipeline.o 

echo "Starting time measurement"
time for i in {1..10} ; do ./pipeline.o ; done

echo "Starting memory measurement"
for i in {1..10}; do /usr/bin/time -f "%M" ./pipeline.o; done

#perf record ./pipeline.o
