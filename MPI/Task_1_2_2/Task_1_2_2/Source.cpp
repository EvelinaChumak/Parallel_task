
#include <stdio.h>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank, size, recv;;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Request reqs[size * 2];
	MPI_Status Status[size * 2];
	for (int i = 0; i < size; ++i)
	{
		MPI_Irecv(&recv, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i]);
		printf("Process %d sent to process %d \n", rank, i);
	}
	for (int i = 0; i < size; ++i)
	{
		MPI_Isend(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[size + i]);
		printf("Process %d recive from process %d \n", rank, i);
	}
	MPI_Waitall(size*2, reqs, Status);
	MPI_Finalize();
	reruen 0;
}