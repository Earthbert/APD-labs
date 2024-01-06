#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int recv_num;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        // Send the number to the next process.
        srand(time(NULL));
        int num = rand() % 100;
        printf("Process %d generated number %d\n", rank, num);

        MPI_Request request;
        MPI_Isend(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);

        MPI_Irecv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Process %d received number %d\n", rank, recv_num);
    } else {
        // Receives the number from the previous process.
        // Increments the number.
        // Sends the number to the next process.
        MPI_Request request;
        MPI_Irecv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Process %d received number %d\n", rank, recv_num);

        recv_num += 2;

        MPI_Isend(&recv_num, 1, MPI_INT, (rank + 1) % numtasks, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();

}

