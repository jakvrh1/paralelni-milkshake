#!/bin/bash

echo "Compiling"
g++ -std=c++11 -O2 -lpthread pipeline.cpp -o pipeline 

echo "Starting time measurement"
time for i in {1..10} ; do ./pipeline ; done

echo "Starting memory measurement"
for i in {1..10}; do /usr/bin/time -f "%M" ./pipeline; done

#perf record ./pipeline 
