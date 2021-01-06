#!/bin/bash

echo "Compiling"
g++ -std=c++11 -O2 -lpthread pipeline.cpp -o pipeline 

echo "Starting"
time ./pipeline
