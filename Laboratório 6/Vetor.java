import java.util.Random;

public class Vetor {
    private final int[] vet;
    Vetor(int n){
        this.vet = new int[n];
    }

    public int[] vetor(){ return vet; }

    public void inicializa(){
        Random r = new Random();
        for(int i = 0; i < this.vet.length; i++ ) vet[i] = r.nextInt(10);
    }

    public void imprime(){
        System.out.print("[");
        for(int i : vet) System.out.print(" " + i);
        System.out.println(" ]");
    }
}
