#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)

void *f(void *arg) {
	long id = *(long *)arg;
	for (int i = 0; i < 100; i++)
		printf("Hello World din thread-ul %ld!\n", id);
	pthread_exit(NULL);
}

void *g(void *arg) {
	printf("NO!!!!!!!!!!!!!!!!!!\n");
}

int main(int argc, char *argv[]) {
	int r;
	long id;
	void *status;

	pthread_t thread_f, thread_g;
	long f_arg = 69;

	r = pthread_create(&thread_f, NULL, f, &f_arg);

	if (r) {
		printf("Eroare la crearea thread-ului %ld\n", id);
		exit(-1);
	}

	r = pthread_create(&thread_g, NULL, g, NULL);

	if (r) {
		printf("Eroare la crearea thread-ului %ld\n", id);
		exit(-1);
	}

	r = pthread_join(thread_f, &status);

	if (r) {
		printf("Eroare la asteptarea thread-ului %ld\n", id);
		exit(-1);
	}

	r = pthread_join(thread_g, &status);

	if (r) {
		printf("Eroare la asteptarea thread-ului %ld\n", id);
		exit(-1);
	}

	return 0;
}
