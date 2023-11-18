package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class QueensTask implements Runnable {
	private final int[] graph;
	private final int step;
	private final ExecutorService executorService;
	private final AtomicInteger inPool;

	public QueensTask(int[] graph, int step, ExecutorService executorService, AtomicInteger inPool) {
		this.graph = graph;
		this.step = step;
		this.executorService = executorService;
		this.inPool = inPool;
	}

	@Override
	public void run() {
		if (Main.N == step) {
			if (inPool.decrementAndGet() == 0) {
				executorService.shutdown();
			}
            printQueens(graph);
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
				executorService.submit(new QueensTask(newGraph, step + 1, executorService, inPool));
				inPool.incrementAndGet();
            }
        }
		if (inPool.decrementAndGet() == 0) {
			executorService.shutdown();
		}
	}
	
	private static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }
}
