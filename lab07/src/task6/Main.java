package task6;

import java.util.concurrent.ForkJoinPool;

public class Main {
    public static int N = 4;

    public static void main(String[] args) {
        int[] graph = new int[N];
        ForkJoinPool pool = new ForkJoinPool(8);
        pool.invoke(new QueensRecursiveTask(graph, 0));
        pool.shutdown();
        pool.close();
    }
}
