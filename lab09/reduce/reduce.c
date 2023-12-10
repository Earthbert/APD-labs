#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main(int argc, char *argv[]) {
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if ((procs & (procs - 1)) != 0) {
    //     if (rank == MASTER)
    //         printf("Number of processes must be a power of 2\n");
    //     MPI_Finalize();
    //     exit(0);
    // }

    int value = rank;

    int pow_2 = 1;
    while (pow_2 <= procs) {
        pow_2 <<= 1;
    }
    pow_2 >>= 1;

    if (rank < pow_2) {
        for (int i = 2; i <= procs; i <<= 1) {
            if (rank % i == 0) {
                if (rank + i / 2 >= procs)
                    break;
                int received;
                MPI_Recv(&received, 1, MPI_INT, rank + i / 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                value += received;
            } else {
                MPI_Send(&value, 1, MPI_INT, rank - i / 2, 0, MPI_COMM_WORLD);
                break;
            }
        }
    } else {
        MPI_Send(&value, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    }

    if (rank == MASTER) {
        for (int i = pow_2; i < procs; i++) {
            int received;
            MPI_Recv(&received, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += received;
        }
        printf("Result = %d\n", value);
    }

    MPI_Finalize();

}

