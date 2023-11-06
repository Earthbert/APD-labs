package multipleProducersMultipleConsumersNBuffer;

/**
 * DO NOT MODIFY
 */
public class Producer implements Runnable {
    private final Buffer buffer;
    private final int id;

    public Producer(Buffer buffer, int id) {
        this.buffer = buffer;
        this.id = id;
    }

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            try {
                Main.empty.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            buffer.put(i);
            Main.full.release();
        }
        System.out.println("Producer " + id + " finished Correctly");
    }

}
