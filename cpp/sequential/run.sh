#!/bin/bash

echo "Compiling"
g++ -std=c++11 -g -O2 -lpthread sequential.cpp -o sequential 

echo "Starting"
time ./sequential
