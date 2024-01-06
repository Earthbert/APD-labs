#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

typedef struct {
    int size;
    int arr[1000];
} queue_t;

int main(int argc, char *argv[]) {
    int numtasks, rank;

    queue_t q;
    MPI_Datatype mpi_queue_t;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    {
        int blocklengths[] = { 1, 1000 };
        MPI_Aint displacements[2];
        displacements[0] = offsetof(queue_t, size);
        displacements[1] = offsetof(queue_t, arr);
        MPI_Datatype types[] = { MPI_INT, MPI_INT };
        MPI_Type_create_struct(2, blocklengths, displacements, types, &mpi_queue_t);
        MPI_Type_commit(&mpi_queue_t);
    }

    srand(rank * time(NULL));
    q.size = 0;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number and add it in queue.
        // Sends the queue to the next process.
        q.arr[q.size] = rand();
        q.size++;
        MPI_Send(&q, 1, mpi_queue_t, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&q, 1, mpi_queue_t, numtasks - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < q.size; i++) {
            printf("%d ", q.arr[i]);
        }
    } else {
        // Receives the queue from the previous process.
        // Generate a random number and add it in queue.
        // Sends the queue to the next process.
        MPI_Recv(&q, 1, mpi_queue_t, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        q.arr[q.size] = rand();
        q.size++;
        MPI_Send(&q, 1, mpi_queue_t, (rank + 1) % numtasks, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}