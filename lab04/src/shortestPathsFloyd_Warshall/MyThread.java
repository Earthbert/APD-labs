package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class MyThread extends Thread {
	int[][] graph;
	Integer threadId;
	Integer numThreads;
	CyclicBarrier barrier;

	public MyThread(int[][] graph, int threadId, int numThreads, CyclicBarrier barrier) {
		this.graph = graph;
		this.threadId = threadId;
		this.numThreads = numThreads;
		this.barrier = barrier;
	}

	@Override
	public void run() {
		super.run();

		int start = (int) (threadId * (double) graph.length / numThreads);
		int end = (int) Math.min((threadId + 1) * (double) graph.length / numThreads, graph.length);

		for (int k = 0; k < graph.length; k++) {
			for (int i = start; i < end; i++) {
				for (int j = 0; j < graph[i].length; j++) {
					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
				}
			}

			try {
				barrier.await();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (BrokenBarrierException e) {
				e.printStackTrace();
			}
		}
	}
}
