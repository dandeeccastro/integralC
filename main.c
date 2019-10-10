#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include <pthread.h>

#define NTHREADS 4

int threadCount = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void barreira() {
	pthread_mutex_lock(&mutex);
	threadCount++;
	if (threadCount < NTHREADS) {
		printf("Ok, eu espero\n");
		pthread_cond_wait(&cond,&mutex);
	} else {
		printf("Vou liberar vocẽs!\n");
		threadCount = 0;
		pthread_cond_broadcast(&cond);
	} printf("Saíndo da barreira\n");
	pthread_mutex_unlock(&mutex);
}

void *t(void *arg) {
	printf("Fazendo coisas super importantes!\n");
	barreira();
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
	int i;
	pthread_t threads[NTHREADS];
	for (i = 0; i < NTHREADS; i++) {
		if (pthread_create(&threads[i],NULL,t,NULL)) {
			printf("Erro na criação de threads!\n");
		}
	} for (i = 0; i < NTHREADS; i++) {
		if (pthread_join(threads[i],NULL)) {
			printf("Erro na espera das threads terminarem!\n");
		}
	} pthread_exit(NULL);
}