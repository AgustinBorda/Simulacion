#!/bin/bash
mkdir ../output > /dev/null 2>&1
touch ../output/output.txt
./../build/Ej6 11 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} "../output/output.txt"
gnuplot -p ../src/plotting_Ej6

