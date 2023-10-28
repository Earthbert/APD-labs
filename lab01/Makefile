build: example add_serial add_parallel

example: example.c
	gcc example.c -o example -lpthread

add_serial: add_serial.c
	gcc add_serial.c -o add_serial
	gcc add_paralel.c -o add_paralel
	gcc example_two_threads.c -o example_two_threads -lpthread
clean:
	rm -f example add_serial add_parallel