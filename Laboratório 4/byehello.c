/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  3

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread B */
void *B (void *t) {
  int boba1, boba2;

  printf("B: Comecei\n");
  
  /* faz alguma coisa para gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("HELLO\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  printf("B: x = %d, vai sinalizar a condicao \n", x);
  pthread_cond_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread A */
void *A (void *t) {
  printf("A: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  if (x == 0) { 
     printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }
  pthread_mutex_unlock(&x_mutex); 
  printf("BYE\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[2], NULL, A, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
