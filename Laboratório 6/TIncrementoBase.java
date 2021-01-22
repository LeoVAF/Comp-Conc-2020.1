/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Codigo: Acessando variável compartilhada em um programa multithreading Java */
/* -------------------------------------------------------------------*/

//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
  //recurso compartilhado
  private int r;
  //construtor
  public S() { 
     this.r = 0; 
  }

  //operacao de escrita sobre o recurso compartilhado
 /*public void inc() {
     this.r++; 
  }
  //operacao de leitura sobre o recurso compartilhado
  public int get() {
     return this.r;
  }*/
  
  // ou...

  public synchronized void inc() {
     this.r++; 
  }

  public synchronized int get() { 
      return this.r; 
  }
  
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   private final int id;
   //objeto compartilhado com outras threads
   S s;
  
   //construtor
   public T(int tid, S s) { 
      this.id = tid; 
      this.s = s;
   }

   //metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int i=0; i<100000; i++) {
         this.s.inc();  
      }
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class TIncrementoBase {
   static final int N = 2;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s);
      }

      //inicia as threads
       for (Thread thread : threads) {
           thread.start();
       }

      //espera pelo termino de todas as threads
       for (Thread thread : threads) {
           try {
               thread.join();
           } catch (InterruptedException e) {
               return;
           }
       }

      System.out.println("Valor de s = " + s.get()); 
   }
}
