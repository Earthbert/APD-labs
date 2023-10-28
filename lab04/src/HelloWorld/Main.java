package HelloWorld;

public class Main {
	public static void main(String[] args) {
		Integer numCores = Runtime.getRuntime().availableProcessors();
		MyThread[] myThreads = new MyThread[numCores];

		for (int i = 0; i < myThreads.length; i++) {
			myThreads[i] = new MyThread(i);
			myThreads[i].start();
		}
	}
}
