#!/bin/bash

echo "Compiling"
g++ -std=c++11 -O2 sequential.cpp -o sequential 
./sequential

#echo "Starting time measurement"
#time for i in {1..10} ; do ./sequential ; done

#echo "Starting memory measurement"
#for i in {1..10}; do /usr/bin/time -f "%M" ./sequential; done

#perf record ./sequential 
