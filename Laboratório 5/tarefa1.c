#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>


// Variáveis globais
int N; // Quantidade de threads leitoras
int M; // Quantidade de threads escritoras
int TAM; // Tamanho do vetor
int leitores = 0, escritores = 0;
int *vetor; // Vetor de inteiros e vetor de médias
pthread_mutex_t mutex;
pthread_cond_t lcond, econd;

// Thread para leitura
void* leitor(void* arg){
	int id = *(int *)arg; // identificador da thread no programa
	int teste = 0, media = 0; // Testa se a thread foi bloqueada e guarda a média
	pthread_mutex_lock(&mutex); // Início da seção crítica
	printf("Leitor %i começou\n", id);
	while(escritores > 0){ // Testa se há escritores
		if(!teste) printf("Leitor %i foi bloqueado\n", id);
		pthread_cond_wait(&lcond, &mutex);
		teste = 1;
	}
	if(teste) printf("Leitor %i foi desbloqueado\n", id);
	leitores++;
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	for(int i = 0; i < TAM; i++){
		media += vetor[i];
		printf("Leitor %i imprimiu => conteúdo = %i e média = %f\n", id, vetor[i], (float) media/(i+1));
	}
	pthread_mutex_lock(&mutex);
	leitores--;
	pthread_cond_broadcast(&econd);
	pthread_mutex_unlock(&mutex);
	
	printf("Leitor %i terminou\n", id);
	pthread_exit(NULL);
}

// Thread para escrita
void* escritor(void* arg){
	int id = *(int *)arg; // identificador da thread no programa
	int teste = 0; // Testa se houve bloqueio
	
	pthread_mutex_lock(&mutex); // Início da seção crítica
	printf("Escritor %i começou\n", id);
	while(escritores > 0 || leitores > 0){ // Verifica se é a única thread para utilizar o vetor
		if(!teste) printf("Escritor %i foi bloqueado\n", id);
		pthread_cond_wait(&econd, &mutex);
		teste = 1;
	}
	
	if(teste) printf("Escritor %i foi desbloqueado\n", id);
	
	escritores++; // Escritor começa a escrever
	// Início da seção de escrita
	vetor[0] = id;
	for(int i = 1; i < TAM-1; i++){
		vetor[i] = 2*id;
	}
	vetor[TAM-1] = id; // Fim da seção de escrita
	escritores--;
	pthread_cond_broadcast(&econd); pthread_cond_broadcast(&lcond); // Desbloqueia os leitores e escritores
	pthread_mutex_unlock(&mutex); // Fim da seção crítica
	
	printf("Escritor %i terminou\n", id);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	// Inicializa as variáveis de controle
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&lcond, NULL);
	pthread_cond_init(&econd, NULL);

	srand(time(NULL)); // Semente para vetor aleatório
	
	// Teste inicial dos dados
	if(argc != 4){
		puts("\nTamanho do vetor = 10, N = 2 e M = 2");
		puts("Formato para modificação: <programa> <tamanho do vetor> <número de leitores> <número de escritores>");
		N = 2;
		M = 2;
		TAM = 10;
	}
	else{
		TAM = atoi(argv[1]);
		N = atoi(argv[2]);
		M = atoi(argv[3]);
		if(TAM < 2) TAM = 10;
		if(N < 2) N = 2;
		if(M < 2) M = 2;
		printf("\nTamanho do vetor = %i, N = %i e M = %i\n",N, M, TAM);
	}
	
	pthread_t tid[N]; // Armazena o identificador do sistema para as threads de leitura e escritura
	int id[N+M]; // Armazena o identificador do programa para as threads de leitura e escritura
	vetor = (int *) malloc(TAM*sizeof(int)); // Aloca espaço para o vetor
	int a = -5, b = -1; // Intervalo para o qual os valores devem variar [a, b]
	
	// Preenchimento de vetor com valores aleatórios
	printf("Vetor = [");
	for(int i = 0; i < TAM; i++){
		vetor[i] = ceil((double) rand()/RAND_MAX*(b-a)) + a;
		printf(" %i",vetor[i]);
	}
	printf(" ]\n\n");
	
	// Criação alternada das threads
	if(N > M){
		for(int i = 0; i < M; i++){
			id[i] = i;
			id[i+N] = i;
			if(pthread_create(tid+i, NULL, leitor, (void *) &id[i])){
				printf("ERRO: Não foi possível criar leitor %i\n", i);
				return -1;
			}
			printf("Criou leitor %i\n", i);
			if(pthread_create(tid+N+i, NULL, escritor, (void *) &id[N+i])){
				printf("ERRO: Não foi possível criar escritor %i\n", i);
				return -1;
			}
			printf("Criou escritor %i\n", i);
		}
		for(int i = M; i < N; i++){
			id[i] = i;
			if(pthread_create(tid+i, NULL, leitor, (void *) &id[i])){
				printf("ERRO: Não foi possível criar leitor %i\n", i);
				return -1;
			}
			printf("Criou leitor %i\n", i);
		}
	}
	else{
		for(int i = 0; i < N; i++){
			id[i] = i;
			id[i+N] = i;
			if(pthread_create(tid+i, NULL, leitor, (void *) &id[i])){
				printf("ERRO: Não foi possível criar leitor %i\n", i);
				return -1;
			}
			printf("Criou leitor %i\n", i);
			if(pthread_create(tid+N+i, NULL, escritor, (void *) &id[N+i])){
				printf("ERRO: Não foi possível criar escritor %i\n", i);
				return -1;
			}
			printf("Criou escritor %i\n", i);
		}
		for(int i = N; i < M; i++){
			id[N+i] = i;
			if(pthread_create(tid+N+i, NULL, escritor, (void *) &id[N+i])){
				printf("ERRO: Não foi possível criar escritor %i\n", i);
				return -1;
			}
			printf("Criou escritor %i\n", i);
		}
	}
	
	
	// Espera bloqueante
	for(int i = 0; i < N+M; i++){
		pthread_join(tid[i], NULL);
	}
	
	// Destrói as variáveis de controle
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&lcond);
	pthread_cond_destroy(&econd);
	free(vetor);
	
	return 0;
}
