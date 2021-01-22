/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Codigo: "Hello World" usando threads em Java */


//--PASSO 1: criar uma classe que implementa a interface Runnable 
class Hello implements Runnable {
   private final String msg;
   
   //--construtor
   public Hello(String m) { 
      this.msg = m; 
   }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg); 
   }
}

//--classe do metodo main
class HelloThread {
   static final int N = 10;

   public static void main (String[] args) {
      //--reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //--PASSO 2: transformar o objeto Runnable em Thread
      for (int i=0; i<threads.length; i++) {
         final String m = "Hello World da thread: " + i;
         System.out.println("--Cria a thread " + i);
         threads[i] = new Thread(new Hello(m));
      }

      //--PASSO 3: iniciar a thread
      for (Thread thread : threads) {
         thread.start();
      }

      //--PASSO 4: esperar pelo termino das threads
      for (Thread thread : threads) {
         try {
            thread.join();
         } catch (InterruptedException e) {
            return;
         }
      }

      System.out.println("Terminou"); 
   }
}
