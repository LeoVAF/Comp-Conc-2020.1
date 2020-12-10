/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10 //total de threads a serem criadas

//--funcao executada pelas threads
void *PrintHello (void *arg) {
  printf("Hello World\n");

  pthread_exit(NULL);
}

//--funcao principal do programa
int main(void) {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  
  int thread; //variavel auxiliar

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, PrintHello, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou\n");

  pthread_exit(NULL);
}
