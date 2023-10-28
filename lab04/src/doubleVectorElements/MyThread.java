package doubleVectorElements;

public class MyThread extends Thread {
	int[] v;
	Integer threadId;
	Integer numThreads;

	public MyThread(int[] v, int threadId, int numThreads) {
		this.v = v;
		this.threadId = threadId;
		this.numThreads = numThreads;
	}

	@Override
	public void run() {
		int start = (int) (threadId * (double) v.length / numThreads);
		int end = (int) Math.min((threadId + 1) * (double) v.length / numThreads, v.length);

		super.run();
		for (int i = start; i < end; i++) {
			v[i] *= 2;
		}
	}
}
