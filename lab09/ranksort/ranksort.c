#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

#define N 10
#define MASTER 0

void compareVectors(int *a, int *b) {
	// DO NOT MODIFY
	int i;
	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int *v) {
	// DO NOT MODIFY
	int i;
	for (i = 0; i < N; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int *)a;
	int B = *(int *)b;
	return A - B;
}

int main(int argc, char *argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);
	int pos[N];
	int sorted = 0;
	int *v = (int *)malloc(sizeof(int) * N);
	int *vQSort;
	int *result;

	for (i = 0; i < N; i++)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	if (rank == MASTER) {
		vQSort = (int *)malloc(sizeof(int) * N);
		result = (int *)malloc(sizeof(int) * N);

		// generate random vector
		srand(42);
		for (i = 0; i < N; i++)
			v[i] = rand() % 1000;

		// DO NOT MODIFY
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for (i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);
	}



	// send the vector to all processes
	MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

	int start = rank * (double)N / nProcesses;
	int end = min((rank + 1) * (double)N / nProcesses, N);

	for (i = start; i < end; i++) {
		for (j = 0; j < N; j++) {
			if (v[i] > v[j]) {
				pos[i]++;
			}
		}
	}


	MPI_Gather(pos + start, end - start, MPI_INT, pos, end - start, MPI_INT, MASTER, MPI_COMM_WORLD);

	if (rank == MASTER) {
		for (i = 0; i < N; i++) {
			result[pos[i]] = v[i];
		}
		displayVector(result);
		compareVectors(result, vQSort);
	}

	MPI_Finalize();
	return 0;
}
