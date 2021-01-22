import java.util.Scanner;

public class Atividade5 {
    public static void main(String[] args){
        System.out.println("Digite o número de threads:");
        Scanner entrada = new Scanner(System.in);
        int numT = entrada.nextInt();
        System.out.println("Digite o tamanho do vetor:");
        int numN = entrada.nextInt();
        // Verificação da entrada
        if(numN < 1) numN = 1;
        if(numT > numN || numT < 1) numT = numN;

        Thread[] threads = new Thread[numT]; // Aloca as threads
        Vetor a = new Vetor(numN), b = new Vetor(numN), c = new Vetor(numN); // Inicializa os vetores com valores aleatórios
        a.inicializa(); b.inicializa();

        System.out.println("Thread: " + numT + "\nVetor: " + numN);

        // Cria as threads
        for(int i = 0; i < numT; i++){
            threads[i] = new Thread(new Fluxo(i, numT, a, b, c));
            threads[i].start();
        }

        // Espera as threads terminarem
        for(Thread t : threads) try{
            t.join();
        } catch(InterruptedException e){
            System.out.println("Erro ao esperar threads!");
        }

        // Imprime os vetores
        System.out.print("a = ");a.imprime();
        System.out.print("b = ");b.imprime();
        System.out.print("c = ");c.imprime();
    }
}
