package oneProducerOneConsumer;

/**
 * DO NOT MODIFY
 */
public class Producer implements Runnable {
    private final Buffer buffer;

    public Producer(Buffer buffer) {
        this.buffer = buffer;
    }

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            try {
                Main.s1.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            buffer.put(i);

            Main.s2.release();
        }

    }

}
