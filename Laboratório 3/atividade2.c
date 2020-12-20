#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<stdlib.h>
// Cálculo em segundos
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H
#include <sys/time.h>
#define BILLION 1000000000L
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif


int NTHREADS; // Número de threads
long int N; // Até onde irá o somatório para o cálculo de pi

void* seriePi(void* arg){
	int* thread = (int*) arg; // Identificador da thread
	double* serie_thread = malloc(sizeof(double)); // Soma por thread
	if(serie_thread == NULL){
		fprintf(stderr, "Erro ao alocar espaço para as threads\n");
		exit(1);
	}
	long int i = 2*(*thread);
	*serie_thread = 0;
	while(i < N-1){ // Vamos pegar os elementos da série par a par alternadamente entre as threads
		*serie_thread += 1.0/(1+2*i) - 1.0/(3+2*i);
		i += 2*NTHREADS;
	}
	if(i == N-1) *serie_thread += 1.0/(1+2*i);
	pthread_exit(serie_thread);
}

int main(int argc, char* argv[]){
	// Dados inseridos pelo usuário
	N = atoll(argv[1]);
	NTHREADS = atoi(argv[2]);
	// Teste de validade dos dados
	if(argc != 3){
		fprintf(stderr,"O programa deve ter o seguinte formato:\n<Programa> <Elementos da série> <Número de threads>\n");
		exit(1);
	}
	else if( N < 1){
		fprintf(stderr, "Parâmetro inválido: %li",N);
		exit(1);
	}
	else if(NTHREADS < 1){
		fprintf(stderr,"Parâmetro inválido: %i",NTHREADS);
		exit(1);
	}
	
	double ti, tf, tempo_seq, tempo_conc; // Marcadores de tempo
	double serie_seq = 0, serie_conc = 0; // Somatório da série sequencial e concorrente
	double s = 1; // Sinal dos elementos
	// Soma sequencial
	GET_TIME(ti);
	long int i = 0;
	while(i < N-1){
		serie_seq += 1.0/(1+2*i) - 1.0/(3+2*i);
		i += 2;
	} 
	if(i == N-1) serie_seq += 1.0/(1+2*i); // Caso N ímpar, devemos somar o último elemento
	serie_seq *= 4;
	GET_TIME(tf);
	tempo_seq = tf-ti; // Tempo de soma sequencial
	
	double **ret = (double **) malloc(sizeof(double));
	if(ret == NULL){
		fprintf(stderr, "Erro ao alocar espaço para retorno das threads\n");
		exit(1);
	}
	// Criação das threads
	GET_TIME(ti)
	long int tid[NTHREADS];
	int thread[NTHREADS];
	for(int i = 0; i < NTHREADS; i++){
		thread[i] = i;
		if(pthread_create(tid+i, NULL, seriePi, (void *) &thread[i])){
			fprintf(stderr, "Não foi possível criar thread: %i\n", i);
			exit(1);
		}
	}
	// Espera bloqueante
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], (void **) ret)){
			fprintf(stderr, "Erro ao esperar término da thread: %i\n", i);
			exit(1);
		}
		serie_conc += **ret; // Soma concorrente com o retorno de pthread_exit
		free(*ret); // libera memória alocada nas threads
	}
	serie_conc *= 4;
	GET_TIME(tf);
	tempo_conc = tf-ti;
	printf("Série sequencial: %.15f\nSérie concorrente: %.15f\nTempo sequencial: %.15f s\nTempo concorrente: %.15f s\n",serie_seq,serie_conc,tempo_seq,tempo_conc);
	puts("");
	printf("Erro sequencial =  %.15f\nErro concorrente = %.15f\nDiferença erro_seq - erro_conc = %.15f\n", M_PI-serie_seq, M_PI-serie_conc,serie_conc-serie_seq);
	printf("Aceleração = %.15f\n", tempo_seq/tempo_conc);
	free(ret);
	return 0;
}

