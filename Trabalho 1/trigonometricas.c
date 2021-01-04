#include<stdio.h>
#include<stdlib.h>
#include<math.h> // Compilar com -lm
#include<pthread.h> // Compilar com -lpthread
#include<time.h>

long int NTHREADS; // Número de threads
long int N; // Número de iterações
double* trigonometrica; // Ponteiro para guardar os retornos das threads
double x; // Parâmetro da função trigonométrica

// Função que calcula os termos das séries seno e cosseno
double termo(double x ,long int n){
	double a = 1;
	for(long int i = 1; i <= n; i++) a *= x/i;
	double b = a*x/(n+1)*x/(n+2);
	return a-b;
}

// Limita x no intervalo [-2pi, 2pi]
double normaliza(double y){
	double norma = y, n = 360, temp = y;
	while(temp >= n){
		temp -= n;
		norma = temp;
	}
	while(temp <= -n){
		temp += n;
		norma = temp;
	}
	return norma;
}

// Retorna o valor absoluto
double absoluto(double x){
	return (x < 0)? -x : x;
}

// Série para o cálculo de seno
void* seno(void* arg){
	long int id = *(long int *)arg;
	 
	// Intervalo para as threads
	long int inic, fim;
	inic = N*id/NTHREADS;
	fim = N*(id+1)/NTHREADS - 1;
	
	// Somatório resultante
	double soma = 0;
	for(long int i = inic; i <= fim; i++){
		soma += termo(x, 4*i + 1); // termo_seno da série
	}
	
	// Armazena o somatório
	trigonometrica[id] = soma;
	pthread_exit(NULL);
}

// Série para o cálculo do cosseno
void* cosseno(void* arg){
	long int id = *(long int *)arg;
	 
	// Intervalo para as threads
	long int inic, fim;
	inic = N*id/NTHREADS;
	fim = N*(id+1)/NTHREADS-1;
	
	// Somatório resultante
	double soma = 0;
	for(long int i = inic; i <= fim; i++){
		soma += termo(x, 4*i+4);
	}
	// Armazena o somatório
	trigonometrica[id] = soma;
	pthread_exit(NULL);
}



int main(int argc, char* argv[]){
	if(argc != 3){
	puts("FORMATO: ./prog <número de iterações> <número de threads>");
		return -1;
	}
	
	// Dados iniciais
	typedef void* (*Func)(void*);
	Func f; // Ponteiro para threads
	char resposta[32]; // resposta do usuário
	int t = 0; // Variável de teste para a resposta
	clock_t ti, tf; // Variáveis de marcação de tempo
	N = atoll(argv[1]); // Número de iterações
	NTHREADS = atoll(argv[2]); // Número de threads
	if(NTHREADS > N) NTHREADS = N; // Limita o número de threads
	pthread_t tid[NTHREADS]; // Identificador da thread
	long int id[NTHREADS]; // Identificador da thread no programa
	trigonometrica = (double*) malloc(NTHREADS*sizeof(double)); // Valor de retorno das threads
	double resultado, tempo; // Resultado da série e duração das operações
	
	// Escolha da função
	puts("Digite \"sen\" ou \"cos\" para escolher uma função:");
	scanf("%s", resposta);
	if(!strcmp(resposta, "cos")){
		puts("Função cosseno selecionada");
		f = &cosseno;
	}
	else{
		puts("Função seno selecionada");
		f = &seno;
		t = 1;
	}
	
	// Ciclo para cálculo da função escolhida
	while(1){
		ti = clock();// Início da contagem
		puts("Digite o ponto (em graus) para cáculo da função: (digite \"s\" para sair, \"sen\" ou \"cos\" para trocar de função)");
		scanf("%s", resposta);
		fflush(stdin); // Limpa o buffer do teclado
		if(!strcmp(resposta, "s") || !strcmp(resposta, "S")) break;
		else if(!strcmp(resposta, "cos")){
			puts("Função cosseno selecionada");
			f = &cosseno;
			t = 0;
			continue;
		}
		else if(!strcmp(resposta, "sen")){
			puts("Função seno selecionada");
			f = &seno;
			t = 1;
			continue;
		}
			
		x = normaliza(atof(resposta)); // Normaliza x devido limitação no fatorial
		x *= M_PI/180;
		// Criação das threads
		for(long int i = 0; i < NTHREADS; i++){
			id[i] = i;
			if(pthread_create(tid+i, NULL, f, (void *) (id+i))){
				fprintf(stderr, "Não foi possível criar thread: %li\n", i);
				exit(1);
			}
		}
		
		// Espera bloqueante
		for(long int i = 0; i < NTHREADS; i++){
			if(pthread_join(tid[i], NULL)){
				fprintf(stderr, "Erro ao esperar término da thread: %li\n", i);
				exit(1);
			}
		}
		
		resultado = 0;
		for(long int i = 0; i < NTHREADS; i++) resultado += trigonometrica[i];
		tf = clock();
		tempo = (double) (tf - ti)/CLOCKS_PER_SEC;
		if(t) printf("\nSérie = %.20lf\nERRO = %.20lf\nTempo = %lf s\n\n", resultado, absoluto(resultado - sin(x)), tempo);
		else{
			resultado += 1 - x*x/2;
			printf("\nSérie = %.20lf\nERRO = %.20lf\nTempo = %lf s\n\n", resultado, absoluto(resultado - cos(x)), tempo);
		}
	}
	free(trigonometrica);
	return 0;
}
