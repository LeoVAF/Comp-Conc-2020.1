#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define T_ESCRITA 3
#define T_LEITURA 3

pthread_mutex_t mutex, escr, leit;
int e = 0, l = 0;
int leitores = 2, escritores = 2; // Número de threads padrão

void* leitor(void* arg){
	printf("Leitor %i iniciou\n", *(int *)arg);
	
	pthread_mutex_lock(&leit);
	pthread_mutex_lock(&mutex); l++; // Lock()
	if(l == 1) pthread_mutex_lock(&escr);
	pthread_mutex_unlock(&mutex); // Unlock()
	pthread_mutex_unlock(&leit);
	printf("Leitor %i lendo\n", *(int *)arg);
	sleep(T_LEITURA); // Lendo
	pthread_mutex_lock(&mutex); l--; // Lock()
	printf("Leitor %i terminou de ler\n", *(int *)arg);
	if(l == 0) pthread_mutex_unlock(&escr);
	pthread_mutex_unlock(&mutex); // Unlock()
	
	pthread_exit(NULL);
}

void* escritor(void* arg){
	printf("Escritor %i iniciou\n", *(int *)arg);

	pthread_mutex_lock(&mutex); e++; // Lock()
	if(e == 1) pthread_mutex_lock(&leit);
	pthread_mutex_unlock(&mutex); // Unlock()
	pthread_mutex_lock(&escr);
	printf("Escritor %i escrevendo\n", *(int *)arg);
	sleep(T_ESCRITA); // Escrevendo
	pthread_mutex_unlock(&escr);
	pthread_mutex_lock(&mutex); e--; // Lock()
	printf("Escritor %i terminou de escrever\n", *(int *)arg);
	if(e == 0) pthread_mutex_unlock(&leit);
	pthread_mutex_unlock(&mutex); // Unlock()
	
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	if(argc != 3) puts("Formato de entrada: ./prog <número de leitores> <número de escritores>");
	else{
		if(atoi(argv[1]) > 0) leitores = atoi(argv[1]);
		if(atoi(argv[2]) > 0) escritores = atoi(argv[2]);
	}
	printf("\nLeitores: %i\nEscritores: %i\n\n", leitores, escritores);
	// Inicializa semáforos
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&escr, NULL);
	pthread_mutex_init(&leit, NULL);
	
	pthread_t threads[leitores + escritores];
	int id[leitores + escritores];
	
	for(int i = 0; i < leitores; i++){
		id[i+escritores] = i;
		if(pthread_create(&threads[escritores+i], NULL, leitor, (void*) &id[i+escritores])){
			puts("ERRO: Não foi possível criar thread leitora");
			exit(-1);
		}
	}
	
	for(int i = 0; i < escritores; i++){
		id[i] = i;
		if(pthread_create(&threads[i], NULL, escritor, (void*) &id[i])){
			puts("ERRO: Não foi possível criar thread escritora");
			exit(-1);
		}
	}
	
	for(int i = 0; i < leitores+escritores; i++){
		if(pthread_join(threads[i], NULL)){
			puts("ERRO: Não foi possível esperar uma das threads");
			exit(-1);
		}
	}
	
	// Destrói semáforos
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&escr);
	pthread_mutex_destroy(&leit);
	return 0;
}
