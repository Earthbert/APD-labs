package task4;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class PathRecursiveTask extends RecursiveTask<Void> {
	private final int[][] graph;
	private final ArrayList<Integer> partialPath;
	private final int destination;

	public PathRecursiveTask(int[][] graph, ArrayList<Integer> partialPath, int destination) {
		this.graph = graph;
		this.partialPath = partialPath;
		this.destination = destination;
	}

	@Override
	protected Void compute() {
		if (partialPath.get(partialPath.size() - 1) == destination) {
			System.out.println(partialPath);
			return null;
		}

		// se verifica nodurile pentru a evita ciclarea in graf
		int lastNodeInPath = partialPath.get(partialPath.size() - 1);

		List<PathRecursiveTask> tasks = new ArrayList<>();

		for (int[] ints : graph) {
			if (ints[0] == lastNodeInPath) {
				if (partialPath.contains(ints[1]))
					continue;
				ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
				newPartialPath.add(ints[1]);
				PathRecursiveTask task = new PathRecursiveTask(graph, newPartialPath, destination);
				tasks.add(task);
				task.fork();
			}
		}

		for (PathRecursiveTask task : tasks) {
			task.join();
		}

		return null;
	}

}
