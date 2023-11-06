package shortestPathsFloyd_Warshall;

import java.util.Random;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;

public class Main {
    public static Integer NEG_INF = Integer.MAX_VALUE >> 1;

    public static void main(String[] args) {
        int M = 9;
        int P = 5;
        int length = 2000;
        int[][] graphSeq = new int[length][length];
        int[][] graphPar = new int[length][length];
        CyclicBarrier barrier = new CyclicBarrier(P);

        Random rand = new Random();
        for (int i = 0; i < graphSeq.length; i++) {
            for (int j = 0; j < graphSeq[i].length; j++) {
                int num = rand.nextInt(3);
                if (num == 2) {
                    graphSeq[i][j] = M;
                    graphPar[i][j] = M;
                } else if (num == 1) {
                    graphSeq[i][j] = 1;
                    graphPar[i][j] = 1;
                } else {
                    graphSeq[i][j] = NEG_INF;
                    graphPar[i][j] = NEG_INF;
                }
            }
        }

        long startTime = System.nanoTime();
        // Parallelize me (You might want to keep the original code in order to compare)
        for (int k = 0; k < graphSeq.length; k++) {
            for (int i = 0; i < graphSeq.length; i++) {
                for (int j = 0; j < graphSeq[i].length; j++) {
                    graphSeq[i][j] = Math.min(graphSeq[i][k] + graphSeq[k][j], graphSeq[i][j]);
                }
            }
        }

        long endTime = System.nanoTime();
        System.out.println("Sequential time: " + TimeUnit.NANOSECONDS.toMillis(endTime - startTime));

        startTime = System.nanoTime();
        Thread[] threads = new Thread[P];

        for (int i = 0; i < P; i++) {
            threads[i] = new MyThread(graphPar, i, P, barrier);
            threads[i].start();
        }
        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        endTime = System.nanoTime();
        System.out.println("Parallel time: " + TimeUnit.NANOSECONDS.toMillis(endTime - startTime));

        Boolean equal = true;
        for (int i = 0; i < graphPar.length; i++) {
            for (int j = 0; j < graphPar[i].length; j++) {
                if (graphPar[i][j] != graphSeq[i][j]) {
                    equal = false;
                    break;
                }
            }
        }

        if (equal)
            System.out.println("Correct");
        else
            System.out.println("Incorrect");

    }
}
