package task1;

import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicInteger;

public class ComputeRouteTask implements Runnable {
	private final int[][] graph;
	private final ArrayList<Integer> partialPath;
	private final int destination;

	private final ExecutorService executorService;
	private final AtomicInteger inPool;

	public ComputeRouteTask(int[][] graph, ArrayList<Integer> partialPath, int destination,
			ExecutorService executorService, AtomicInteger inPool) {
		this.graph = graph;
		this.partialPath = partialPath;
		this.destination = destination;
		this.executorService = executorService;
		this.inPool = inPool;
	}

	@Override
	public void run() {
		if (partialPath.get(partialPath.size() - 1) == destination) {
			System.out.println(partialPath);
			inPool.decrementAndGet();
			if (inPool.get() == 0)
				executorService.shutdown();
			return;
		}

		int lastNodeInPath = partialPath.get(partialPath.size() - 1);
		for (int[] ints : graph) {
			if (ints[0] == lastNodeInPath) {
				if (partialPath.contains(ints[1]))
					continue;

				ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
				newPartialPath.add(ints[1]);

				executorService.submit(
						new ComputeRouteTask(graph, newPartialPath, destination, executorService, inPool));

				inPool.incrementAndGet();
			}
		}
		inPool.decrementAndGet();
		if (inPool.get() == 0)
			executorService.shutdown();
	}

}
