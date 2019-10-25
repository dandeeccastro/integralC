#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

double n = 2;
double a,b,erro;

double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	//return sqrt(1 + pow(x,4));
	//return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}

double MetodoDeSimpson(double comeco, double final){
	double deltaX = (final - comeco)/n; 
	double resultado = 0; double xi, coef;
	int i;
	for (i = 0; i <= n; i++){
		xi = comeco + i * deltaX;
		if (i == 0 || i == n)
			coef = 1;
		else if (i%2)
			coef = 4;
		else if (!i%2)
			coef = 2;
		resultado += Function (xi) * coef;
	}
	return resultado * (deltaX/3);
}

void Calculador(){
	double m = (b - a) / 2;
	double criteria, localResult;
	while (true){
		localResult = MetodoDeSimpson(a,b);
		criteria = MetodoDeSimpson(a,m) + MetodoDeSimpson(m,b) - localResult;
		if (criteria < 0) criteria *= -1;
		if (criteria < erro * 15)
			break;
		else
			n *= 2;
	} printf("Resultado calculado foi %lf\n", localResult);
}

int main(int argc, char *argv[]){
	if (argc != 4) {
		printf("Uso: ./sequencial <comeco> <fim> <erro>\n");
		return 1;
	}
	a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]);
	Calculador();
	return 0;
}
