#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MASTER 0

typedef struct {
	float val;
	float sum;
} polynomial;

int main(int argc, char *argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	MPI_Datatype mpi_polynomial;
	{
		int blocklen[2] = { 1, 1 };
		MPI_Datatype types[2] = { MPI_FLOAT, MPI_FLOAT };
		MPI_Aint disp[2];
		disp[0] = offsetof(polynomial, val);
		disp[1] = offsetof(polynomial, sum);
		MPI_Type_create_struct(2, blocklen, disp, types, &mpi_polynomial);
		MPI_Type_commit(&mpi_polynomial);
	}

	if (rank == MASTER) { // This code is run by a single process
		int polynomialSize, n;
		int x = 5; // valoarea cu care se calculeaza polinomul - f(5)

		/*
			in fisierul de intrare formatul este urmatorul:
			numarul_de_coeficienti
			coeficient x^0
			coeficient x^1
			etc.
		*/

		FILE *polFunctionFile = fopen(argv[1], "rt");
		fscanf(polFunctionFile, "%d", &polynomialSize);
		MPI_Bcast(&polynomialSize, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

		/*
			in array-ul a se vor salva coeficientii ecuatiei / polinomului
			de exemplu: a = {1, 4, 4} => 1 * (x ^ 2) + 4 * (x ^ 1) + 4 * (x ^ 0)
		*/
		float *a = malloc(sizeof(float) * polynomialSize);

		fscanf(polFunctionFile, "%f", &a[0]);
		printf("Read value %f\n", a[0]);
		for (int i = 1; i < polynomialSize; i++) {
			fscanf(polFunctionFile, "%f", &a[i]);
			printf("Read value %f\n", a[i]);
			/*
				Se trimit coeficientii pentru x^1, x^2 etc. proceselor 1, 2 etc.
				Procesul 0 se ocupa de x^0 si are valoarea coeficientului lui x^0
			*/
			MPI_Send(&a[i], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}

		fclose(polFunctionFile);

		// Se trimit valorile x si suma partiala (in acest caz valoarea coeficientului lui x^0)

		polynomial p;
		p.val = x;
		p.sum = a[0];
		MPI_Send(&p, 1, mpi_polynomial, 1, 0, MPI_COMM_WORLD);
	} else {
		polynomial p;
		float x;
		int polynomialSize;

		MPI_Bcast(&polynomialSize, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
		if (rank >= polynomialSize) {
			MPI_Finalize();
			return 0;
		}
		/*
			se primesc:
			- coeficientul corespunzator procesului (exemplu procesul 1 primeste coeficientul lui x^1)
			- suma partiala
			- valoarea x din f(x)
			si se calculeaza valoarea corespunzatoare pentru c * x^r, r fiind rangul procesului curent
			si c fiind coeficientul lui x^r, si se aduna la suma
		*/
		MPI_Recv(&x, 1, MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, mpi_polynomial, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		p.sum += pow(p.val, x);

		if (rank == polynomialSize - 1) {
			printf("Polynom value is %f\n", p.sum);
		} else {
			// se trimit x si suma partiala catre urmatorul proces
			MPI_Send(&p, 1, mpi_polynomial, rank + 1, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}
