#!/bin/bash
for filename in *.cpp; do
	echo "Compile ${filename} ..."
	rm $(basename "$filename" .cpp)
	g++ -fopenmp -O3 "$filename" -o $(basename "$filename" .cpp) -Wall
	echo "\n"
done
