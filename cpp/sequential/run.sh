#!/bin/bash

mkdir -p ../../out 
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

#echo "Executing sequential"
#g++ -std=c++11 -O2 sequential.cpp -o sequential.o 
#"$DIR/sequential"

echo "Starting time measurement"
time for i in {1..10} ; do "$DIR/sequential" ; done

echo "Starting memory measurement"
for i in {1..10}; do /usr/bin/time -f "%M" "$DIR/sequential"; done

#perf record "$DIR/sequential"
