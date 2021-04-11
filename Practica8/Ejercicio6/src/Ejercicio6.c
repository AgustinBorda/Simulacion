#include <stdlib.h>
#include <stdio.h>
#include "euler.h"

double ba;
double m;
double g;
double k;
double b;
/**
 * Derivate function for speed
 * params:
 	speed: the current speed.
	height: the current height.
*/
double derSpeed(double speed,double height) {
	if (height>0) {
		return -(ba/m)*speed -g;
	}
	else {
		return -(k/m) * height - (b/m) * speed -g;
	}
}

int main(int argc, char* argv[]) {
	/*Param initialization*/
	double initSpeed = atof(argv[2]);
	double initHeight = atof(argv[3]);
	ba = atof(argv[4]);
	m = atof(argv[5]);
	g = atof(argv[6]);
	k = atof(argv[7]);
	b = atof(argv[8]);
	double tInit = atof(argv[9]);
	double tEnd = atof(argv[10]);
	double h = atof(argv[11]);
	/*call to euler*/
	eulerEj6(initSpeed,initHeight,tInit,tEnd,h,derSpeed,argv[12]);
	return 0;
}

