#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define NTHREADS 4

/* Variáveis Globais e Compartilhadas */
double n = 2;
double a, b, erro;
pthread_mutex_t mutex;
int checkIfFinished = 0;

double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	//return sqrt(1 + pow(x,4));
	//return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}

double EfectiveSimpson(double in, double end, double nDouble){

	int i; int nInt = nDouble;
	double result = 0;

	double coef; // coeficiente da soma
	double deltaX = (end - in)/nDouble;
	if (deltaX < 0) { deltaX *= -1;}
	for (i = 0; i <= nInt; i++){ // iterando pelos subespaços
		if (i == 0 || i == nInt){
			coef = 1.;
		} else if (i%2) {
			coef = 4.;
		} else { coef = 2.;}
		if (checkIfFinished) return 1000;
		//printf("coef = %lf; xi = %lf no passo %d\n",coef,in + i*deltaX,i+1 );
		result += coef * Function(in + i*deltaX);
	}
	return result*(deltaX/3);

}

void *t() {

	// Pegando dados de variáveis globais uma vez! Mutex lock!
	pthread_mutex_lock(&mutex);
	double localN = n; n *= 2;
	pthread_mutex_unlock(&mutex);

	double c = (a + b)/2.0;
	double localResult = EfectiveSimpson(a,b,localN);
	double h = (b - a)/localN;
	double criteria = - (pow(h,4)/180) * (b - a) * Function(c);
	if (criteria < 0) { criteria *= -1; }

	while (!(criteria < erro*15) && !checkIfFinished) {
		localResult = EfectiveSimpson(a,b,localN);
		criteria = EfectiveSimpson(a,c,localN) + EfectiveSimpson(c,b,localN) - localResult;
		if (criteria < 0) { criteria *= -1; }
		printf("%lf < %lf\n", criteria ,erro*15);

		pthread_mutex_lock(&mutex);
		localN = n;
		n *= 2;
		pthread_mutex_unlock(&mutex);
	}
	if (!checkIfFinished){
		checkIfFinished = 1;
	} else {
		pthread_exit(NULL);
	} printf("Deu %lf pra mim! Tchau!\n", localResult);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

	// Inicializa as variáveis necessárias e começa a contagem de tempo
	double start, startCrit,endCrit, end;
	pthread_t threads[NTHREADS];
	pthread_mutex_init(&mutex,NULL);
	GET_TIME(start);

	// Lê input do terminal e valida, colocando na variável
	if (argc != 4) {
		printf("Uso: ./sequencial <comeco> <fim> <erro>\n");
		return 1;
	}	a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]);

	// Cria os threads, espera eles terminarem e faz a contagem de tempo concorrente
	GET_TIME(startCrit);
	int i;
	for (i = 0; i < NTHREADS; i++) {
		if (pthread_create(&threads[i],NULL,t,NULL)){
			printf("Erro no pthread_create!\n");
		}
	}
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i],NULL);
	}
	GET_TIME(endCrit); GET_TIME(end);

	// Imprime o resultado final
//	printf("Resultado da integral: %lf\n", result);
	printf("Subintervalos: %lf\n", n);
	printf("Tempo de execução: %lf s\n", end - start);

	// Análise de tempo
	printf("Rodei com %lf s, e %lf s deles eram concorrentes! \n", (end - start), (endCrit - startCrit) );
	return 0;
}
