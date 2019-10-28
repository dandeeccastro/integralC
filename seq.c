#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "timer.h"

double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	//return sqrt(1 + pow(x,4));
	//return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}

double n = 2;


double EfectiveSimpson(double in, double end){
	 // numero de subintervalos
	int i; int nInt = n;
	double result = 0;

	double coef; // coeficiente da soma
	double deltaX = (end - in)/n;
	if (deltaX < 0) { deltaX *= -1;}
	for (i = 0; i <= nInt; i++){ // iterando pelos subespaços
		if (i == 0 || i == nInt){
			coef = 1.;
		} else if (i%2) {
			coef = 4.;
		} else { coef = 2.;}

		//printf("coef = %lf; xi = %lf no passo %d\n",coef,in + i*deltaX,i+1 );
		result += coef * Function(in + i*deltaX);
	}
	return result*(deltaX/3);

}

double Simpson(double a, double b) {
	double c, h3;
	c = (a+b)/2.0;
	h3 = (b-a)/6.0;
	if (h3 < 0) { h3 *= -1; }
	return h3 * (Function(a) + 4.0*Function(c) + Function(b));
}

int main(int argc, char const *argv[]) {
	
	double start, startCrit,endCrit, end;
	GET_TIME(start);
	if (argc != 5) {
		printf("Uso: ./sequencial <comeco> <fim> <erro> <numero de threads>\n");
		return 1;
	}

	double a, b, erro; 
	int NTHREADS;

	a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]); NTHREADS = atoi(argv[4]);
	
	double c, result, criteria;

	GET_TIME(startCrit);
	do {
		c = (b - a)/2.0;
		result = EfectiveSimpson(a,b);
		criteria = EfectiveSimpson(a,c) + EfectiveSimpson(c,b) - result;
		if (criteria < 0) { criteria *= -1; }
		printf("%lf < %lf | n = %lf\n", criteria ,erro*15, n);
		n++;
	} while (!(criteria < erro*15));
	GET_TIME(endCrit); GET_TIME(end);
	
	printf("Resultado da integral: %lf\n", result);
	printf("Subintervalos: %lf\n", n);
	printf("Tempo de execução: %lf s\n", end - start);

	double amdahl = (startCrit - start) + (endCrit - end) + (endCrit - startCrit)/NTHREADS;
	printf("Com %d threads, poderia ter sido feito em %lf s, com %lf de ganho!\n",
 		NTHREADS, amdahl,(end - start)/amdahl );
	return 0;
}
