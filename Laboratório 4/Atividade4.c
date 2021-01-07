#include<stdio.h>
#include<pthread.h>

// Variáveis globais
int ordem = 0; // Variável de controle para ordem de impressão
pthread_mutex_t mutex; // Variável de exclusão mútua
pthread_cond_t cond; // Pilha de bloqueio condicional

void* t1(void* arg){
	puts("tudo bem?");
	
	pthread_mutex_lock(&mutex); // Início da seção crítica
	ordem++;
	if(ordem == 2){
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	
	pthread_exit(NULL);
}

void* t2(void* arg){
	puts("bom dia!");
	
	pthread_mutex_lock(&mutex); // Início da seção crítica
	ordem++;
	if(ordem == 2){
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	
	pthread_exit(NULL);
}

void* t3(void* arg){
	pthread_mutex_lock(&mutex); // Início da seção crítica
	while(ordem < 2){
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	
	puts("até mais!");
	pthread_exit(NULL);
}

void* t4(void* arg){
	pthread_mutex_lock(&mutex); // Início da seção crítica
	while(ordem < 2){
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	
	puts("boa tarde!");
	pthread_exit(NULL);
}

int main(){
	pthread_t tid[4]; // id das threads
	pthread_mutex_init(&mutex, NULL); // Inicializa a variável mutex
	pthread_cond_init(&cond, NULL); // Inicializa a variável cond
	
	// Chama thread 1
	if(pthread_create(tid, NULL, t1, NULL)){
		puts("Não foi possível criar thread 1");
		return -1;
	}
	// Chama thread 2
	if(pthread_create(tid+1, NULL, t2, NULL)){
		puts("Não foi possível criar thread 2");
		return -1;
	}
	// Chama thread 3
	if(pthread_create(tid+2, NULL, t3, NULL)){
		puts("Não foi possível criar thread 3");
		return -1;
	}
	// Chama thread 4
	if(pthread_create(tid+3, NULL, t4, NULL)){
		puts("Não foi possível criar thread 4");
		return -1;
	}
	
	// Espera as threads terminarem para destruir as variáveis
	for(int i = 0; i < 4; i++) pthread_join(tid[i], NULL);
	
	pthread_mutex_destroy(&mutex); // Destrói a variável mutex
	pthread_cond_destroy(&cond); // Destrói a variável cond
	
	pthread_exit(NULL);
}
