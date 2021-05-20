#include <iostream>
#include <time.h> 
#include "mpi.h"

using namespace std;

const int n = 4, var = 200;

void Init(int* a)
{
    for (int i = 0; i < n; ++i)
    {
        a[i] = rand() % var + 1;
        cout << a[i] << ' ';
    }
    cout << endl;
}


void Merge(int* a, int first, int middle, int last)
{
    int start, final, j;
    int* mas = new int[n];
    start = first;
    final = middle + 1;
    for (j = first; j <= last; ++j)
        if ((a[start] < a[final]) && (start <= middle) || (final > last)) {
            mas[j] = a[start];
            start++;
        }
        else {
            mas[j] = a[final];
            final++;
        }
    for (j = first; j <= last; ++j)
        a[j] = mas[j];
    delete[] mas;
};


void MergeSort(int* a, int first, int last)

{

    int middle = (first + last) / 2;
    if (first == last)
        return;
    else
    {
        MergeSort(a, first, middle);
        MergeSort(a, middle + 1, last);
        Merge(a, first, middle, last);

    }

}

void MPI_Merge(int* a, int& argc, char* argv[])
{
    int size, rank, stride;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int* N = &n;

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //cout << "Bcast" << endl;

    if (rank < n % size)
        stride = n / size + 1;
    else
        stride = n / size;

    int* x = new int[stride];
    int* scounts = new int[size];
    int* displs = new int[size];
    
    //cout << "stride, size, rank " << stride << ' ' << size << ' ' << rank << endl;

    for (int i; i < size; ++i)
    {
        displs[i] = i * stride;
        scounts[i] = stride;
    }

    MPI_Scatterv(a, scounts, displs, MPI_INT, x, stride, MPI_INT, 0, MPI_COMM_WORLD);

    //cout << "Scatterv" << endl;
    
    MergeSort(x, 0, stride - 1);

    //cout << "MergeSort" << endl;
    int s = size, m = 1;
    while (s > 1)
    {
        s = s / 2 + s % 2;
        if ((rank - m) % (2 * m) == 0)
        {
            MPI_Send(&stride, 1, MPI_INT, rank - m, 0, MPI_COMM_WORLD);
            MPI_Send(x, stride, MPI_INT, rank - m, 0, MPI_COMM_WORLD);

            //cout << "send" << endl;
        }
        if ((rank % (2 * m) == 0) && (size - rank > m))
        {
            MPI_Status status;
            int recv_stride;
            MPI_Recv(&recv_stride, 1, MPI_INT, rank + m, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            int* y = new int[stride + recv_stride];

            MPI_Recv(y, recv_stride, MPI_INT, rank + m, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            //cout << "Recv" << endl;
            for (int i = 0; i < stride; ++i)
                y[i + recv_stride] = x[i];

            Merge(y, 0, recv_stride - 1, stride + recv_stride - 1);

            x = new int[stride + recv_stride];

            for (int i = 0; i < stride + recv_stride; ++i)
                x[i] = y[i];

            stride = stride + recv_stride;
        }
        m = 2 * m;
    }
    for (int i = 0; i < n; ++i)
        a[i] = x[i];

    MPI_Finalize();
}

int main(int argc, char* argv[]) {

    int* a = new int[n];
    double t1, t2;
    srand(time(0));

    double time = 0;

    Init(a);

    /*
    for (int i = 0; i < 10; ++i)
    {
        Init(a);

        t1 = MPI_Wtime();
        MPI_Merge(a, argc, argv);
        t2 = MPI_Wtime();
        time += t2 - t1;
    }
    cout << "Parallel: " << time / 10 << endl;

    time = 0;
    for (int i = 0; i < 10; ++i)
    {
        Init(a);

        t1 = MPI_Wtime();
        MergeSort(a, 0, n-1);
        t2 = MPI_Wtime();
        time += t2 - t1;
    }
    cout << "Usual: " << time / 10 << endl;
    */

    MPI_Merge(a, argc, argv);

    for (int i; i < 0; ++i)
        cout << a[i] << ' ';
    cout << endl;

    delete[] a;
    return 0;
}