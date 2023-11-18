package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class ColorGraphTask implements Runnable {
	private final int[] colors;
	private final int step;
	private final ExecutorService executorService;
	private AtomicInteger inPool;

	public ColorGraphTask(int[] colors, int step, ExecutorService executorService, AtomicInteger inPool) {
		this.colors = colors;
		this.step = step;
		this.executorService = executorService;
		this.inPool = inPool;
	}

	@Override
	public void run() {
		if (step == Main.N) {
			printColors(colors);
			if (inPool.decrementAndGet() == 0) {
				executorService.shutdown();
			}
			return;
		}

		// for the node at position step try all possible colors
		for (int i = 0; i < Main.COLORS; i++) {
			int[] newColors = colors.clone();
			newColors[step] = i;
			if (verifyColors(newColors, step)) {
				executorService.submit(new ColorGraphTask(newColors, step + 1, executorService, inPool));
				inPool.incrementAndGet();
			}
		}

		if (inPool.decrementAndGet() == 0) {
			executorService.shutdown();
		}
	}

	private static boolean verifyColors(int[] colors, int step) {
		for (int i = 0; i < step; i++) {
			if (colors[i] == colors[step] && isEdge(i, step))
				return false;
		}
		return true;
	}

	private static boolean isEdge(int a, int b) {
		for (int[] ints : Main.graph) {
			if (ints[0] == a && ints[1] == b)
				return true;
		}
		return false;
	}

	private static void printColors(int[] colors) {
		StringBuilder aux = new StringBuilder();
		for (int color : colors) {
			aux.append(color).append(" ");
		}
		System.out.println(aux);
	}
}
