#!/bin/bash

echo "Compiling"
g++ -std=c++11 -O2 -lpthread pipesquare.cpp -o pipesquare.o

echo "Starting time measurement"
time for i in {1..10} ; do ./pipesquare.o ; done

echo "Starting memory measurement"
for i in {1..10}; do /usr/bin/time -f "%M" ./pipesquare.o; done

#perf record ./pipesquare.o 
