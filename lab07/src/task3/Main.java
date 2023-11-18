package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static int N = 4;
    public static void main(String[] args) {
        int[] graph = new int[N];
        AtomicInteger inPool = new AtomicInteger(1);
        ExecutorService executorService = Executors.newFixedThreadPool(8);
        
        executorService.submit(new QueensTask(graph, 0, executorService, inPool));
    }
}
