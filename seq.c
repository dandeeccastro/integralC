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
	if (argc != 4) {
		printf("Uso: ./sequencial <comeco> <fim> <erro>\n");
		return 1;
	}
	double a, b, erro;
	a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]);
	double c = (a + b)/2.0;
	double result = EfectiveSimpson(a,b);
	double criteria = Simpson(a,c) + Simpson(c,b) - result;
	if (criteria < 0) { criteria *= -1; }
	while (!(criteria < erro*15)) {
		n *= 2;
		criteria = EfectiveSimpson(a,c) + EfectiveSimpson(c,b) - result;
		result = EfectiveSimpson(a,b);
		if (criteria < 0) { criteria *= -1; }
		printf("%lf < %lf\n", criteria ,erro*15);
	}
	printf("%lf vs %lf\n",Simpson(a,b),result);
	printf("%lf < %lf\n", criteria ,erro*15);
	return 0;
}
