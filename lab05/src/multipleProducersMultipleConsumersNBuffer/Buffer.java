package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;

public class Buffer {
    
    Queue<Integer> queue;
    
    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
    }

	synchronized public void put(int value) {
        queue.add(value);        
	}

	synchronized public int get() {
        int a = -1;
        Integer result = queue.poll();
        if (result != null) {
            a = result;
        }
        return a;
	}
}
