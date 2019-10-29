#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "timer.h"

// Variável global
double n = 2;

/*
	A função cuja integral será calculada. Aqui você pode descomentar a 
	função que deseja calcular a integral, salvar, compilar e executar à vontade!
*/
double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	//return sqrt(1 + pow(x,4));
	//return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}

/*
	Faz o cálculo da integral com base na Fórmula de Simpson para aproximações
	e nos retorna o resultado final. Aqui que fica o loop mais pesado na execução
	do código.
*/
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
		result += coef * Function(in + i*deltaX);
	}
	return result*(deltaX/3);

}

/**
 * Fluxo principal da aplicação. Por ser a versão sequencial, o principal do 
 * cálculo matemático é feito nessa função. Aqui, avaliamos a área dos subespaços,
 * vemos se ele está alinhado com nosso critério de avaliação e retornar
 * o resultado
 **/
int main(int argc, char const *argv[]) {
	
	// Variáveis de contagem de tempo criadas, contagem iniciada
	double start, startCrit,endCrit, end;
	GET_TIME(start);

	// Cria variáveis, corrige o input do usuário e passa o input dele
	// para as variáveis
	double a, b, erro; 
	int NTHREADS;

	if (argc != 5) {
		printf("Uso: ./sequencial <comeco> <fim> <erro> <numero de threads>\n");
		return 1;
	}
	
	a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]); NTHREADS = atoi(argv[4]);
	
	// Criamos as variáveis de cálculo, começamos a contagem do tempo e
	// começamos o loop de cálculo da integral
	double c, result, criteria;
	GET_TIME(startCrit);
	do {
		// Calculo os valores e critérios. Se passaram, o loop para, 
		// senão aumentamos N e repetimos o cálculo
		c = (b - a)/2.0;
		result = EfectiveSimpson(a,b);
		criteria = EfectiveSimpson(a,c) + EfectiveSimpson(c,b) - result;
		if (criteria < 0) { criteria *= -1; }
		printf("%lf < %lf | n = %lf\n", criteria ,erro*15, n);
		n++;
	} while (!(criteria < erro*15));
	GET_TIME(endCrit); GET_TIME(end);


	// Imprimimos os resultados
	printf("Resultado da integral: %lf\n", result);
	printf("Tempo de execução: %lf s\n", end - start);

	// Análise do tempo do cálculo
	double amdahl = (startCrit - start) + (endCrit - end) + (endCrit - startCrit)/NTHREADS;
	printf("Com %d threads, poderia ter sido feito em %lf s, com %lf de ganho!\n",
 		NTHREADS, amdahl,(end - start)/amdahl );
	return 0;
}
