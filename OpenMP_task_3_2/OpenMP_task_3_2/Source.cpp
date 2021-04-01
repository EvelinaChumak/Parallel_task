#include <iostream> 
#include <omp.h>
#include <time.h> 
#include "windows.h"
#include <math.h>

using namespace std;

const int Size = 4, var = 10, thread = 4;

void Init(int** ar1, int** ar2, int** res)
{
    for (int i = 0; i < Size; ++i)
        for (int j = 0; j < Size; ++j)
        {
            ar1[i][j] = rand() % var + 1;
            ar2[i][j] = rand() % var + 1;
            res[i][j] = 0;
        }
}

void MultSimple(int** ar1, int** ar2, int** res)
{
    for (int i = 0; i < Size; ++i)
    {
        for (int j = 0; j < Size; ++j)
            for (int k = 0; k < Size; ++k)
            res[i][j] += ar1[i][k] * ar2[k][j];
    }
}




void Mult(int** ar1, int** ar2, int** res)
{
    int threadId;
    int sizeBlock = int(sqrt(thread));
    int size = int(Size / sizeBlock);
    int* threadRes = new int[Size];
    if (size == Size)
        omp_set_num_threads(1);
#pragma omp parallel private(threadId)
    {
    threadId = omp_get_thread_num();
    for (int i = 0; i < Size; ++i)
        threadRes[i] = 0;
        int i_start = threadId / sizeBlock * size;
        int k_start = threadId % sizeBlock * size;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < Size; ++k)
                    res[i + i_start][j + k_start] += ar1[i + i_start][k] * ar2[k][j + k_start];
    }
}

void MultBlock(int** ar1, int** ar2, int** res)
{
    int sizeBlock = int(sqrt(thread));
    int size = int(Size / sizeBlock);
#pragma omp parallel
    {
        int threadId = omp_get_thread_num();
        int rowInd = threadId / sizeBlock;
        int colInd = threadId % sizeBlock;
        for (int iter = 0; iter < sizeBlock; ++iter)
            for (int i = rowInd * size; i < (rowInd + 1) * size; ++i)
                for (int j = colInd * size; j < (colInd + 1) * size; ++j)
                    for (int k = iter * size; k < (iter + 1) * size; ++k)
                        res[i][j] += ar1[i][k] * ar2[k][j];
    }
}

int main() {
    int** ar1;
    int** ar2;
    int** res;
    res = new int* [Size];
    ar2 = new int* [Size];
    ar1 = new int* [Size];
    for (int i = 0; i < Size; ++i)
    {
        ar1[i] = new int[Size];
        ar2[i] = new int[Size];
        res[i] = new int[Size];
    }
    double time1, time2;


    omp_set_num_threads(thread);


    srand(time(0));
    Init(ar1, ar2, res);


    time1 = omp_get_wtime();
    MultSimple(ar1, ar2, res);
    time2 = omp_get_wtime();
    cout << "Simple: " << time2 - time1 << '\n';

    for (int j = 0; j < Size; ++j)
        for (int i = 0; i < Size; ++i)
            res[j][i] = 0;

    time1 = omp_get_wtime();
    Mult(ar1, ar2, res);
    time2 = omp_get_wtime();
    cout << "Lent: " << time2 - time1 << '\n';

    for (int j = 0; j < Size; ++j)
        for (int i = 0; i < Size; ++i)
            res[j][i] = 0;


    time1 = omp_get_wtime();
    MultBlock(ar1, ar2, res);
    time2 = omp_get_wtime();
    cout << "By blocks:" << time2 - time1 << '\n';



    return 0;
}