#!/bin/bash

echo "Compiling ..."
g++ -fopenmp main.cpp -o main -O3 -Wall
echo "Done!"
