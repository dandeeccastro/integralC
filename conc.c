#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "timer.h"

/* Variáveis Globais e Compartilhadas referentes ao lock com calculus */
double n = 2;
double a, b, erro;
int NTHREADS;
double result;

/* Variáveis Globais e Compartilhadas referentes ao lock com check */
int threadCount = 0;
bool hasFinished = false;
bool uniqueHolder = false;

/* Variáveis para sincronização */
pthread_mutex_t calculus, check;
pthread_cond_t cond;

/**
 * Método de sincronização baseado em barreira, adaptado para a necessidade
 * do problema.
 * 
 * A ideia é: recebo o resultado do critério de erro e o resultado do cálculo da integral.
 * Caso a variável hasFinished, que define se o resultado foi encontrado, seja falsa ao mesmo
 * tempo que essa não seja a última thread, ela se para e espera o broadcast.
 * A última thread, se não tiver o resultado correto, libera todos para a próxima iteração.
 * Mas se ela tiver o resultado, ela atualiza hasFinished e permite que ela printe o resultado
 * na tela do terminal
 **/
bool ImprovedCriteriaCheck(bool comparisonResult, double calculusResult){
	pthread_mutex_lock(&check);
	bool returnValue = false;
	threadCount++;
	if (!hasFinished && threadCount < NTHREADS ){
		pthread_cond_wait(&cond,&check);
	} else {
		// Ou foi atualizado o hasFinished ou é a ultima thread a chegar na barreira
		threadCount = 0;
		if (comparisonResult && !uniqueHolder) {
			hasFinished = comparisonResult;
			uniqueHolder = true;
			returnValue = true;
		}
		pthread_cond_broadcast(&cond);
	}  
	pthread_mutex_unlock(&check);
	return returnValue;
}

/*
	A função cuja integral será calculada. Aqui você pode descomentar a 
	função que deseja calcular a integral, salvar, compilar e executar à vontade!
*/
double Function(double x) {
	//return 1 + x;
	//return sqrt(1 + x*x);
	//return sqrt(1 + pow(x,4));
	return sin(pow(x,2));
	// return cos(pow(M_E,-x));
	// return cos(pow(M_E,-x))*x;
	//return cos(pow(M_E,-x))*(0.005*pow(x,3) + 1);
}

/*
	Faz o cálculo da integral com base na Fórmula de Simpson para aproximações
	e nos retorna o resultado final. Aqui que fica o loop mais pesado na execução
	do código.
*/
double EfectiveSimpson(double in, double end, double nDouble){

	int i; int nInt = nDouble;
	double resultado = 0;

	double coef; // coeficiente da soma
	double deltaX = (end - in)/nDouble;
	if (deltaX < 0) { deltaX *= -1;}
	for (i = 0; i <= nInt; i++){ // iterando pelos subespaços
		if (i == 0 || i == nInt){
			coef = 1.;
		} else if (i%2) {
			coef = 4.;
		} else { coef = 2.;}
		resultado += coef * Function(in + i*deltaX);
	}
	return resultado*(deltaX/3);

}

/*
	Código executado pelas threads. Usa uma cópia local de n, assim como
	outras variáveis de cálculo,  para fazer o processamento da integral

	O loop principal consiste em atualizar o resultado local e o critério de
	avaliação. Uma vez feitos esses cálculos, valido com a função
	ImprovedCriteriaCheck se o erro calculado é menor que o especificado pelo
	usuário. Se for, a variável hasFinished é setada para true, e todas as
	threads retorna. Senão, o loop continua
*/
void *t() {

	double localN, c, criteria, localResult;
	bool resultHolder = false;
	// Pegando dados de variáveis globais uma vez! Mutex lock!
	pthread_mutex_lock(&calculus);
	localN = n; n++;
	pthread_mutex_unlock(&calculus);

	// Loop baseado na checagem de erro e na condicional estabelecida
	do {
		c = (b - a)/2.0;
		localResult = EfectiveSimpson(a,b,localN);
		criteria = EfectiveSimpson(a,c,localN) + EfectiveSimpson(c,b,localN) - localResult;
		if (criteria < 0) { criteria *= -1; }

		pthread_mutex_lock(&calculus);
		localN = n;
		n++;
		pthread_mutex_unlock(&calculus);
		resultHolder = ImprovedCriteriaCheck((criteria < erro * 15), localResult);
	} while (!hasFinished);
	if (resultHolder) {
		printf("Resultado: %lf\n",localResult);
	}
	pthread_exit(NULL);
}

/**
*	Fluxo principal da aplicação, começa inicializando as variáveis necessárias
*	e depois parte para o threading. No final, retorna o resultado da integral
* 	no terminal e o tempo de execução
*/
int main(int argc, char const *argv[]) {

	// Inicializa as variáveis necessárias e começa a contagem de tempo
	double start, startCrit,endCrit, end;
	pthread_mutex_init(&calculus,NULL);
	GET_TIME(start);

	// Lê input do terminal e valida, colocando nas respectivas variáveis
	if (argc != 5) {
		printf("Uso: ./sequencial <comeco> <fim> <erro> <numero de threads>\n");
		return 1;
	} a = atof(argv[1]); b = atof(argv[2]); erro = atof(argv[3]); NTHREADS = atoi(argv[4]);

	pthread_t threads[NTHREADS];
	// Cria os threads, espera eles terminarem e faz a contagem de tempo concorrente
	GET_TIME(startCrit);
	int i;
	for (i = 0; i < NTHREADS; i++) {
		if (pthread_create(&threads[i],NULL,t,NULL)){
			printf("Erro no pthread_create!\n");
		}
	}
	for (i = 0; i < NTHREADS; i++) {
		if(pthread_join(threads[i],NULL)){
			printf("Erro no pthread_join!\n");
		}
	}
	GET_TIME(endCrit); GET_TIME(end);

	// Imprime o resultado final
	printf("Tempo de execução: %lf s\n", end - start);

	// Análise de tempo
	printf("Rodei com %lf s, e %lf s deles eram concorrentes! \n", (end - start), (endCrit - startCrit) );
	return 0;
}
