#include <stdio.h>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank, size, next, send, recv, prev;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	next = rank + 1;
	prev = rank - 1;
	if (rank == size - 1) next = 0;
	if (rank == 0) prev = size - 1;
	MPI_Sendrecv(&send, 1, MPI_INT, next, 0, &recv, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
	printf("process %d sent process %d \n", rank, next);
	MPI_Finalize();
}