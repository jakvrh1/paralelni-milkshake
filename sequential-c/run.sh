#!/bin/bash

echo "Compiling"
g++ -std=c++11 -lpthread sequential.cpp -o sequential 

echo "Starting"
./sequential
