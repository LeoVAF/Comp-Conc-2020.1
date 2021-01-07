/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so podera imprimir a sentenca X=... quando x for multiplo de 10, e devera fazer isso uma unica vez e depois terminar sua execucao  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  3
#define TCOUNT  20

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Threads que incrementam x */
void *A (void *t) {
  int *my_id = (int*)t, i;
  int boba1, boba2;

  printf("A: Comecei: thread %d\n", *my_id);
  
  for (i=0; i < TCOUNT; i++) {
    pthread_mutex_lock(&x_mutex);
    x++;
    if ((x%10) == 0) {
      printf("A: thread %d, x = %d, encontrou padrao \n", *my_id, x);
      pthread_cond_signal(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    /* faz alguma coisa inutil pra gastar tempo... */
    boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
  }
  free(my_id);
  pthread_exit(NULL);
}

/* Thread que imprime x */
void *B (void *t) {
  int *my_id = (int*)t;

  printf("B: Comecei: thread %d\n", *my_id);

  pthread_mutex_lock(&x_mutex);
  while ((x % 10) != 0) { //para o caso de B ser sinalizada e antes de retornar a execucao o valor de x mudar!
  //if ((x % 10) != 0) { /*** pode gerar resultados incorretos, por que??? ***/
     printf("B: thread %d x = %d, vai se bloquear...\n", *my_id, x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("B: thread %d, sinal recebido e mutex realocado. x = %d\n", *my_id, x);
  }
  /*...faz alguma coisa com x */
  printf("X=%d\n", x);
  pthread_mutex_unlock(&x_mutex); 
  free(my_id);
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3;
  pthread_t threads[NTHREADS];
  //aloca espaco para os identificadores das threads
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  t3=malloc(sizeof(int)); 
  *t1=1, *t2=2, *t3=3;

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[2], NULL, B, (void *)t3);
  pthread_create(&threads[0], NULL, A, (void *)t1);
  pthread_create(&threads[1], NULL, A, (void *)t2);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("FIM.\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
