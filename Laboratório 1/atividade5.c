#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#define NTHREADS 2

// Argumento para thread
typedef struct{
	int intervalo[NTHREADS];
	int* vet;
} t_struct;

void* inc_vetor(void* args){
	t_struct* argum = (t_struct*) args;
	for(int i = argum->intervalo[0]; i < argum->intervalo[1]; i++) argum->vet[i]++;
	free(args);
	pthread_exit(NULL);
}

void main(){
	int N; // Tamanho do vetor
	pthread_t tid[NTHREADS]; // Identificador de fluxo
	
	puts("Digite o tamanho do vetor:");
	scanf("%i",&N);
	int vetor[N]; // Vetor de inteiros
	
	// Testa se 10 < N < 100
	while(N < 11 || N > 99){
		puts("Digite o tamanho do vetor no intervalo (10,100):");
		scanf("%i",&N);
	}
	
	// Preenchendo o vetor aleatoriamente com valores até 100
	srand(time(NULL));
	puts("Vetor inicial:");
	for(int i = 0; i < N; i++){
		vetor[i] = rand()%100;
		printf("%i ",vetor[i]);
	}
	puts("");
	
	for(int i = 0; i < NTHREADS; i++){
		t_struct* args = malloc(sizeof(t_struct));
		args->intervalo[0] = N*i/NTHREADS;
		args->intervalo[1] = N*(i+1)/NTHREADS;
		args->vet = vetor;
		if(pthread_create(&tid[i], NULL, inc_vetor, (void*) args)) printf("ERRO: thread %li falhou\n", tid[i]);
	}
	
	// Espera bloqueante da thread
	for(int i = 0; i < NTHREADS; i++) if(pthread_join(tid[i],NULL)) printf("Erro ao esperar término da thread %li\n",tid[i]);
	// Impressão do vetor
	puts("Vetor final:");
	for(int i = 0; i < N; i++) printf("%i ",vetor[i]);
	puts("");
}






