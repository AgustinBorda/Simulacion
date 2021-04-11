#include "euler.h"
#include <stdio.h>
#include <stdlib.h>

double der(double n) {
	return -n;
}

int main(int argc, char* argv[]) {
	double initVal = atof(argv[2]);
	double tInit = atof(argv[3]);
	double tFin = atof(argv[4]);
	double h = atof(argv[5]);
	euler(initVal,tInit,tFin,h,der,argv[6]);
	return 0;
}
