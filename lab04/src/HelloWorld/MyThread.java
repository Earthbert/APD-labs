package HelloWorld;

/**
 * MyThread
 */
public class MyThread extends Thread{
	private Integer threadId;

	public MyThread(Integer threadId) {
		this.threadId = threadId;
	}

	@Override
	public void run() {
		System.err.println("Hello from thread" + threadId);
		super.run();
	}
}