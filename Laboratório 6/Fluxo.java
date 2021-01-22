public class Fluxo implements Runnable {
    int id, numThreads;
    Vetor a, b, c;
    Fluxo(int id, int t, Vetor a, Vetor b, Vetor c){
        this.id = id;
        this.numThreads = t;
        this.a = a;
        this.b = b;
        this.c = c;
    }
    @Override
    public void run() {
        System.out.println("Thread " + this.id + " inciou!");
        this.soma();
        System.out.println("Thread " + this.id + " terminou!");
    }

    // Somará dois vetores
    synchronized public void soma(){
        // Intervalo
        int x = id * this.a.vetor().length / numThreads;
        int y = (id+1) * this.a.vetor().length / numThreads - 1;
        // Soma as threads
        for (int i = x; i <= y; i++) {
            this.c.vetor()[i] = this.a.vetor()[i] + this.b.vetor()[i];
        }
    }
}
