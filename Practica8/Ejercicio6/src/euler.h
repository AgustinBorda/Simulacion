#include<stdio.h>
/**
 * Provides a continuous function approach with Euler's method.
 *params:
	initVal: The value of the function in the starting point.
	tInit: The initial time value.
	tEnd: The final time value.
	h: The variation of the Euler's method.
	(*der)(double): The derivate function of the function.
	output[]: The external name of the file that we will use to store output the function.
 * Author: Agustin Borda
 * Date: 28/5/2020
 */
void euler(double initVal,double tInit, double tEnd, double h, double (*der)(double), char output[]) {
	FILE *outputFile = fopen(output,"w");
	while(tInit <= tEnd) {
		fprintf(outputFile,"%lf %lf\n",tInit,initVal);
		tInit += h; 
		initVal = initVal + h * der(initVal);
	}
	fclose(outputFile);
	return;
}

/**
 * Euler's Method modified for exercise 6, will stop when the rebound is 
 * insignificant or reaches the end time.
 * params:
	initSpeed: The initial speed of the ball.
	initHeight: The initial Height of the ball.
        tInit: The initial time value.
        tEnd: The final time value.
        h: The variation of the Euler's method.
        (*derSpeed)(double,double): The derivate function of the speed function.
        output[]: The external name of the file that we will use to store output the function.
 * Author: Agustin Borda
 * Date 4/6/2020
 */
void eulerEj6(double initSpeed, double initHeight, double tInit, double tEnd, double h, double (*derSpeed)(double,double), char output[]) {
	FILE *outputFile = fopen(output, "w");
	double maxHeight = initHeight;
	double lastSpeed;
	int isMaxHeight = 0;
	while (tInit <= tEnd && !(initHeight < 0.00001 && isMaxHeight)) {
		fprintf(outputFile, "%.12f %.12f %.12f\n",tInit,initHeight,initSpeed); //put the values in the file
		tInit += h;
		lastSpeed = initSpeed;
		initSpeed = initSpeed + h*derSpeed(initSpeed,initHeight); 
		initHeight = initHeight + h*initSpeed; //The speed is the derivate of the height
		if(lastSpeed >=0 && initSpeed < 0) { //calculates if the current height is a maximun
			isMaxHeight = 1;
		}
		else {
			isMaxHeight = 0;
		}
	}
	fclose(outputFile);
	return;
}
