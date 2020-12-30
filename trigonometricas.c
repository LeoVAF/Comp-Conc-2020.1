#include<stdio.h>
#include<stdlib.h>
#include<math.h> // Compilar com -lm
#include<pthread.h> // Compilar com -lpthread

long int NTHREADS; // Número de threads
long int N; // Número de iterações
double* sen; // Ponteiro para guardar os retornos das threads
double x; // Parâmetro da função trigonométrica seno

// Calcula o fatorial
double fatorial(long int n){
	if(n < 2) return 1;
	double fat = 1;
	for(long int i = 1; i <= n; i++){
		fat *= i;
	}
	return fat;
}

// Limita x no intervalo [-2pi, 2pi]
double normaliza(double y){
	double norma = y, n = 2*M_PI, temp = y;
	while(temp > n){
		temp -= n;
		norma = temp;
	}
	while(temp < -n){
		temp += n;
		norma = temp;
	}
	return norma;
}

// Série para o cálculo de seno
void* seno(void* arg){
	long int id = *(long int *)arg;
	 
	// Intervalo para as threads
	long int inic, fim;
	inic = N*id/NTHREADS;
	fim = N*(id+1)/NTHREADS-1;
	// Somatório resultante
	double soma = 0;
	for(long int i = inic; i <= fim; i++){ soma += pow(x, 4*i + 1)/fatorial(4*i + 1) - pow(x, 4*i + 3)/fatorial(4*i + 3);}
	// Armazena o somatório
	sen[id] = soma;
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	if(argc != 3){
		puts("FORMATO: ./prog <número de iterações> <número de threads>");
		return -1;
	}
	
	// Dados iniciais
	N = atoll(argv[1]); // Número de iterações
	NTHREADS = atoll(argv[2]); // Número de threads
	if(NTHREADS > N) NTHREADS = N; // Limita o número de threads
	pthread_t tid[NTHREADS]; // Identificador da thread
	long int id[NTHREADS]; // Identificador da thread no programa
	sen = (double*) malloc(NTHREADS*sizeof(double)); // Valor de retorno das threads
	
	char resposta[32];
	while(1){
		puts("Digite o ponto (em graus) para cáculo da função seno: (digite s para sair)");
		scanf("%s", resposta);
		fflush(stdin); // Limpa o buffer do teclado
		if(!strcmp(resposta, "s") || !strcmp(resposta, "S")) break;
		
		x = normaliza((M_PI/180)*atof(resposta)); // Normaliza x devido limitação no fatorial
		// Criação das threads
		for(long int i = 0; i < NTHREADS; i++){
			id[i] = i;
			if(pthread_create(tid+i, NULL, seno, (void *) (id+i))){
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
	
		double resultado = 0;
		for(long int i = 0; i < NTHREADS; i++) resultado += sen[i];
		printf("\nSérie = %.20lf\nERRO = %.20lf\n\n", resultado, resultado - sin(x));
	}
	free(sen);
	return 0;
}
