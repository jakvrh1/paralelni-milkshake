#!/bin/bash

mkdir -p ../../out
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

#echo "Compiling"
#g++ -std=c++11 -O2 -lpthread pipeline.cpp -o pipeline.o 

echo "Starting time measurement"
time for i in {1..10} ; do "$DIR/pipeline" ; done

echo "Starting memory measurement"
for i in {1..10}; do /usr/bin/time -f "%M" "$DIR/pipeline"; done

#perf record "$DIR/pipeline"
