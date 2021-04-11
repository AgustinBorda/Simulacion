#!/bin/bash

mkdir ../build > /dev/null 2>&1
gcc -o ../build/euler  ../src/euler.h
gcc -o ../build/Ej6 ../src/Ejercicio6.c

