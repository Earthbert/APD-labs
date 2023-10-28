#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*
	schelet pentru exercitiul 5
*/

#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct {
	int *arr;
	int end;
	int start;
} arg_t;


void *f(void *arg) {
	arg_t *arg_s = arg;
	int start = arg_s->start;
	int end = arg_s->end;

	for (int i = start; i <= end; i++) {
		arg_s->arr[i] += 100;
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	int *arr;
	int array_size;

	if (argc < 2) {
		perror("Specificati dimensiunea array-ului\n");
		exit(-1);
	}

	array_size = atoi(argv[1]);

	arr = malloc(array_size * sizeof(int));
	for (int i = 0; i < array_size; i++) {
		arr[i] = i;
	}

	// for (int i = 0; i < array_size; i++) {
	// 	printf("%d", arr[i]);
	// 	if (i != array_size - 1) {
	// 		printf(" ");
	// 	} else {
	// 		printf("\n");
	// 	}
	// }

	struct timespec start_time, finish_time;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	pthread_t threads[NUM_THREADS];
	int r;
	void *status;
	arg_t *args[NUM_THREADS];
	int start, end;

	for (long id = 0; id < NUM_THREADS; id++) {
		args[id] = malloc(sizeof(arg_t));

		args[id]->arr = arr;
		args[id]->start = id * (double)array_size / NUM_THREADS;
		args[id]->end = MIN((id + 1) * (double)array_size / NUM_THREADS, array_size);

		r = pthread_create(&threads[id], NULL, f, args[id]);

		if (r) {
			printf("Eroare la crearea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	for (long id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	elapsed = (finish_time.tv_sec - start_time.tv_sec);
	elapsed += (finish_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

	printf("ELAPSED: %lf\n", elapsed);

	// for (int i = 0; i < array_size; i++) {
	// 	printf("%d", arr[i]);
	// 	if (i != array_size - 1) {
	// 		printf(" ");
	// 	} else {
	// 		printf("\n");
	// 	}
	// }

	return 0;
}
