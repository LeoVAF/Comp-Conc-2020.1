/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C com passagem de um argumento */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10 //total de threads a serem criadas

//funcao executada pelas threads
void *PrintHello (void* arg) {
  int idThread = * (int*) arg;

  printf("Hello World da thread: %d\n", idThread);

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  int tid_local[NTHREADS]; //identificadores locais das threads

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Cria a thread %d\n", thread);
    tid_local[thread] = thread;
    if (pthread_create(&tid_sistema[thread], NULL, PrintHello, (void*) &tid_local[thread])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);
}
