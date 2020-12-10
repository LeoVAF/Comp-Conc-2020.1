#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#define T 1000 // Unidade de tempo (1 para segundos, milissegundos para 1000, ...)

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


int main(){
	srand(time(NULL)); // Semente para matrizes aleatórias
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
	pthread_t tid[NTHREADS];
	int thread[NTHREADS];
	// Alocação de memória
	int tam = N*N;
	A = (double*) malloc(tam*sizeof(double));
	B = (double*) malloc(tam*sizeof(double));
	C = (double*) malloc(tam*sizeof(double));
	
	/* Início da marcação temporal */
	double tempoinic = clock(), tempo_a, tempo_b, tempo_c;
	
	// Define um intervalo onde os elementos das matrizes podem variar
	double a = -5;
	double b = 5;
	// Inicialização das matrizes A e B
	for(int i = 0; i < tam; i++){
		// Para valores em um intervalo [a,b]
		*(A+i) = (double) rand()/RAND_MAX*(b-a) + a;
		*(B+i) = (double) rand()/RAND_MAX*(b-a) + a;
	}
	tempo_a = (clock() - tempoinic)*T/CLOCKS_PER_SEC; // Tempo de inicialização das matrizes
	
	// Multiplicação (chamada das threads)
	for(int i = 0; i < NTHREADS; i++){
		thread[i] = i;
		if(pthread_create(&tid[i], NULL, multiplicaMatriz, (void*) &thread[i])) printf("ERRO: numt %li falhou\n", tid[i]);
	}
	// Espera bloqueante de main
	for(int i = 0; i < NTHREADS; i++) if(pthread_join(tid[i],NULL)) printf("Erro ao esperar término da thread %li\n",tid[i]);
	tempo_b = (clock() - tempoinic)*T/CLOCKS_PER_SEC; // Tempo de multiplicação de matrizes

	// Impressão das matrizes 
	/*printf("\nA:\n");imprimeMatriz(A, N);puts("");
	printf("B:\n");imprimeMatriz(B, N);puts("");
	printf("C:\n");imprimeMatriz(C, N);puts(""); // Impressão da matriz resultante*/
	
	free(A);free(B);free(C); // Liberação de memória;
	tempo_c = (clock() - tempoinic)*T/CLOCKS_PER_SEC; // Tempo de finalização do programa
	
	// Impressão dos tempos
	if(T == 1) printf("Medidas de tempo:\nInicialização das estruturas = %.10f s\nMultiplicação de matrizes = %.10f s\nFinalização do programa = %.10f s\n",tempo_a,tempo_b,tempo_c);
	else if(T == 1000) printf("Medidas de tempo:\nInicialização das estruturas = %.10f ms\nMultiplicação de matrizes = %.10f ms\nFinalização do programa = %.10f ms\n",tempo_a,tempo_b,tempo_c);
	else if(T == 1000000) printf("Medidas de tempo:\nInicialização das estruturas = %.10f μs\nMultiplicação de matrizes = %.10f μs\nFinalização do programa = %.10f μs\n",tempo_a,tempo_b,tempo_c);
	else if(T == 1000000000) printf("Medidas de tempo:\nInicialização das estruturas = %.10f ns\nMultiplicação de matrizes = %.10f ns\nFinalização do programa = %.10f ns\n",tempo_a,tempo_b,tempo_c);
	else printf("Medidas de tempo:\nInicialização das estruturas = %.10f\nMultiplicação de matrizes = %.10f\nFinalização do programa = %.10f\n",tempo_a,tempo_b,tempo_c);
	return 0;
}








