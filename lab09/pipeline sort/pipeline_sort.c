#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int N;

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

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application with more processes
int main(int argc, char *argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	N = nProcesses - 1;

	if (rank == 0) { // This code is run by a single process
		int *v = (int *)malloc(sizeof(int) * (nProcesses - 1));
		int *vQSort = (int *)malloc(sizeof(int) * (nProcesses - 1));
		int val = 1 << 30;

		// generate the vector v with random values
		// DO NOT MODIFY
		srandom(42);
		for (int i = 0; i < N; i++)
			v[i] = random() % 200;
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for (int i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		// TODO sort the vector v

		for (int i = 0; i < N; i++) {
			MPI_Send(&v[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		}

		{
			int received;
			for (int i = 1; i < nProcesses; i++) {
				MPI_Recv(&received, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				v[i - 1] = received;
			};
		}


		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		// TODO sort the vector v
		int val = 1 << 30;

		for (int i = 0; i < N - (rank - 1); i++) {
			int received;
			MPI_Recv(&received, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (received < val) {
				if (rank + 1 < nProcesses && val != 1 << 30)
					MPI_Send(&val, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				val = received;

			} else if (rank) {
				if (rank + 1 < nProcesses)
					MPI_Send(&received, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			}
		}

		MPI_Send(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
