import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

class Buffer{
    private final ArrayList<Integer> buffer;
    private int base = 0, topo = 0, quantidade = 0;

    Buffer(int tam){
        buffer = new ArrayList<>(tam);
        for(int i = 0; i < tam; i++) buffer.add( null);
    }

    public synchronized void imprime(){ System.out.println(buffer); }

    public void insere(Integer n) throws Exception{  // Coloca elemento na posição
        if(buffer.get(topo) != null) throw new Exception("ERRO: inseriu em posição ocupada");
        buffer.set(topo, n);
        topo = (++topo) % buffer.size();
        quantidade++;
    }
    public Integer retira() throws Exception{ // Retira elemento e faz a posição estar vazia
        Integer elemento = buffer.get(base);
        if(elemento == null) throw new Exception("ERRO: Retirou de posição vazia!");
        buffer.set(base, null);
        base = (++base) % buffer.size();
        quantidade--;
        return elemento;
    }
    public synchronized void consome(int id){ // Consome elemento ou se bloqueia
        boolean t = true;
        try{
            while(quantidade == 0){ // Se buffer vazio, se bloqueia
                if(t) {
                    System.out.println("Thread consumidora " + id + " foi bloqueada");
                    t = false;
                }
                wait();
            }
            if(!t) {
                System.out.println("Thread consumidora " + id + " foi descbloqueada");
            }
        }
        catch(InterruptedException e) {
            e = new InterruptedException("ERRO: thread consumidora não foi bloqueada devidamente!");
        }
        try {
            Integer n = retira();
            System.out.println("Thread consumidora " + id + " retirou: " + n);
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
        notifyAll();
    }

    public synchronized void produz(int id){ // Produz elemento ou se bloqueia
        Random r = new Random();
        boolean t = true;
        try{
            while(quantidade == buffer.size()){ // Se buffer cheio, se bloqueia
                if(t) {
                    System.out.println("Thread produtora " + id + " foi bloqueada");
                    t = false;
                }
                wait();
            }
            if(!t) {
                System.out.println("Thread produtora " + id + " foi desbloqueada");
            }
        }
        catch(InterruptedException e) {
            e = new InterruptedException("ERRO: thread produtora não foi bloqueada devidamente!");
        }
        int n = r.nextInt(10);
        System.out.println("Thread produtora " + id + " inseriu: " + n);
        try{
            insere(n);
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
        notifyAll();
    }
}

class Fluxo implements Runnable{
    // Variáveis compartilhadas
    private final Buffer buffer;
    private final int id;
    private final boolean consumidor;

    Fluxo(Buffer b, int id, boolean resposta){
        this.buffer = b;
        this.id = id;
        this.consumidor = resposta;
    }

    @Override
    public void run(){
        if(consumidor) {
            buffer.consome(id);
        }
        else {
            buffer.produz(id);
        }
    }
}

public class Atividade {
    public static void main(String[] args){
        // Dados de entrada
        Scanner teclado = new Scanner(System.in);
        System.out.println("Digite a quantidade de produtores:");
        int quantp = teclado.nextInt();
        if(quantp < 1) quantp = 2; // Valor padrão
        System.out.println("Digite a quantidade de consumidores:");
        int quantc = teclado.nextInt();
        if(quantc < 1) quantc = 2; // Valor padrão
        System.out.println("Digite o tamanho do buffer:");
        int tam = teclado.nextInt();
        if(tam < 1) tam = 10; // Valor padrão

        Buffer buffer = new Buffer(tam); // Constrói o buffer de inteiros
        Thread[] threads = new Thread[quantc + quantp]; // threads

        System.out.print("buffer = ");buffer.imprime();
        // Cria threads produtoras
        for(int i = 0; i < quantp; i++){
            threads[i] = new Thread(new Fluxo(buffer, i, false));
            threads[i].start();
        }
        // Cria threads consumidoras
        for(int i = quantp; i < quantp + quantc; i++){
            threads[i] = new Thread(new Fluxo(buffer, i-quantp, true));
            threads[i].start();
        }

        // Espera terminar as threads
        for(Thread t : threads) try{
            t.join();
        }catch(InterruptedException e){
            System.out.println("ERRO: espera das threads interrompida!");
            return;
        }
        System.out.print("buffer = ");buffer.imprime();
    }
}
