#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

/**
*	Escolha a função que quer integrar!
*/
double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	return sqrt(1 + pow(x,4));
	//return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	// return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}
/**
* 	Dado o inicio e o final do intervalo de integração, essa função
* 	calcula a área do retângulo aproximado, chegando numa aproximação da
*	integral
*
*	Sn = deltaX/n * (f(x0) + 4f(x1) + 2f(x2) + 4f(x3) + ... + f(xn))
*
*	TODO: - aprender o porquê disso funcionar!
* 		- Resultados estão dando errado!
*/ 
double Simpson(double in, double end){
	double n = 32.; // numero de subintervalos
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

		printf("coef = %lf; xi = %lf no passo %d\n",coef,in + i*deltaX/n,i+1 );
		result += coef * Function(in + i*deltaX);
	}	
	return result*(deltaX/3);

}

int main(int argc, char **args){
	double start, end;
	GET_TIME(start);
	if (argc < 4) {
		printf("Uso: ./integralDefinida.out <inicio> <fim> <erro>\n");
		return 0;
	}
	double a, b, erro;
	a = atof(args[1]); b = atof(args[2]); erro = atof(args[3]);
	printf("%lf até %lf com erro %lf\n",a,b,erro);

	printf("A área de f(x) = sqrt(1 + x^4) entre %lf e %lf é %lf\n",a,b,Simpson(a,b));

	GET_TIME(end);
	printf("Tempo de execução: %lf\n",end - start);

	return 0;
}