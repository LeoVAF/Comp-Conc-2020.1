#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif

// Dados
int N;
int NTHREADS;

// Matrizes
double* A;
double* B;
double* C;

void* multiplicaMatriz(void* a){ 
	int* numt = (int*) a;
	int passo = N*NTHREADS;
	double prod_int;
	// Iteração por linha de C (cada for desse constrói uma linha de C)
	for(int i = *(numt)*N; i < N*N; i += passo){
		// Iteração pela coluna de B (cada for desse constrói um elemento da linha de C)
		for(int j = 0; j < N; j++){
			prod_int = 0;
			// Faz o produto interno (linha de A, coluna de B)
			for(int k = 0; k < N; k++){
				prod_int += A[i + k]*B[N*k + j];
			}
			C[i + j] = prod_int;
		}
	}
	pthread_exit(NULL);
}

void imprimeMatriz(double* M, int N){
	if(N > 0){
		printf("[ %.3f",M[0]);
		for(int i = 1; i < N*N; i++){
			if(i % N == 0) printf("]\n[ %.3f",M[i]);
			else printf(", %.3f",M[i]);
		}
		puts("]");
	}
}


int main(int argc, char* argv[]){
	srand(time(NULL)); // Semente para matrizes aleatórias
	if(argc == 1){
		// Dados iniciais
		puts("Digite a dimensão da matriz:");
		scanf("%i", &N);
		puts("Digite a quantidade de threads:");
		scanf("%i",&NTHREADS);
		/* Trata caso de invalidez na dimensão */
		while(N < 1){
			printf("Dimensão %i inválida, digite novamente:\n",N);
			scanf("%i",&N);
		}
	
		/* Trata caso de invalidez no número de threads */
		while(NTHREADS < 1){
			printf("Quantidade de threads inválida: %i, digite novamente:\n",NTHREADS);
			scanf("%i",&NTHREADS);
		}
	}
	else if(argc == 3){
		N = atoi(argv[1]);
		NTHREADS = atoi(argv[2]);
	}
	else{
		puts("ERRO: Parâmetros inválidos!");
		exit(1);
	}
	printf("Threads: %i\nDimensão das matrizes: %i\n\n", NTHREADS,N);
	pthread_t tid[NTHREADS];
	int thread[NTHREADS];
	// Alocação de memória
	int tam = N*N;
	A = (double*) malloc(tam*sizeof(double));
	B = (double*) malloc(tam*sizeof(double));
	C = (double*) malloc(tam*sizeof(double));
	
	/* Início da marcação temporal */
	double ti,tf, tempo_a, tempo_b, tempo_c;
	GET_TIME(ti);
	
	// Define um intervalo onde os elementos das matrizes podem variar
	double a = -5;
	double b = 5;
	// Inicialização das matrizes A e B
	for(int i = 0; i < tam; i++){
		// Para valores em um intervalo [a,b]
		*(A+i) = (double) rand()/RAND_MAX*(b-a) + a;
		*(B+i) = (double) rand()/RAND_MAX*(b-a) + a;
	}
	GET_TIME(tf);
	tempo_a = tf - ti; // Tempo de inicialização das matrizes
	ti = tf;
	
	// Multiplicação (chamada das threads)
	for(int i = 0; i < NTHREADS; i++){
		thread[i] = i;
		if(pthread_create(&tid[i], NULL, multiplicaMatriz, (void*) &thread[i])) printf("ERRO: numt %li falhou\n", tid[i]);
	}
	// Espera bloqueante de main
	for(int i = 0; i < NTHREADS; i++) if(pthread_join(tid[i],NULL)) printf("Erro ao esperar término da thread %li\n",tid[i]);
	GET_TIME(tf);
	tempo_b = tf - ti; // Tempo de multiplicação de matrizes
	ti = tf;
	// Impressão das matrizes 
	/*
	printf("\nA:\n");imprimeMatriz(A, N);puts("");
	printf("B:\n");imprimeMatriz(B, N);puts("");
	printf("C:\n");imprimeMatriz(C, N);puts(""); // Impressão da matriz resultante
	*/
	
	free(A);free(B);free(C); // Liberação de memória;
	GET_TIME(tf); // Tempo de finalização do programa
	tempo_c = tf - ti + tempo_b + tempo_a;
	// Impressão dos tempos
	printf("Medidas de tempo:\nInicialização das estruturas = %.10f s\nMultiplicação de matrizes = %.10f s\nFinalização do programa = %.10f s\n",tempo_a,tempo_b,tempo_c);
	return 0;
}
