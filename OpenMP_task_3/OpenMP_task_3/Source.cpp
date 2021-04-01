#include <iostream> 
#include<iomanip>
#include <omp.h>
#include <time.h> 
#include "windows.h"

using namespace std;

const int N = 4, M = 4, var = 200, thread = 4;

void Init(int** ar, int* vec, int* res)
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            ar[i][j] = rand() % var + 1;
    for (int j = 0; j < M; ++j)
        vec[j] = rand() % var + 1;
}

void MultByStr(int** ar, int* vec, int* res)
{
    for (int i = 0; i < N; ++i)
    {
        res[i] = 0;
        for (int j = 0; j < M; ++j)
            res[i] += ar[i][j] * vec[j];
    }
}

void ParallelMultByStr(int** ar, int* vec, int* res)
{
    int i, j;
#pragma omp parallel for private(j)
    for (i = 0; i < N; ++i)
    {
        res[i] = 0;
        for (j = 0; j < M; ++j)
            res[i] += ar[i][j] * vec[j];
    }
}

void ParallelMultByCol(int** ar, int* vec, int* res)
{
    int i, j;
#pragma omp parallel for  private(j)
    for (i = 0; i < M; ++i)
    {
        for (j = 0; j < N; ++j)
            res[j] += ar[j][i] * vec[i];
    }
}

void Mult(int** ar, int* vec, int* res)
{
    int threadId;
    int sizeBlock = int(sqrt(thread));
    int size = int(M / sizeBlock);
    int* threadRes = new int[M];
    omp_set_num_threads(4);
#pragma omp parallel private(threadId)
    {
        threadId = omp_get_thread_num();
        for (int i = 0; i < N; ++i)
            threadRes[i] = 0;
        int i_start = threadId / sizeBlock * size;
        int j_start = threadId % sizeBlock * size;
        for (int i = 0; i < size; ++i)
        {
            int iterResult = 0;
            for (int j = 0; j < size; ++j)
                iterResult += ar[i + i_start][j + j_start] * vec[j + i_start];
            threadRes[i + i_start] = iterResult;
        }
    }
    for (int i = 0; i < M; ++i)
        res[i] = threadRes[i];
    delete[] threadRes;
}

int main() {
    int** ar;
    int* vec;
    int* res;
    res = new int[N];
    vec = new int[M];
    ar = new int* [N];
    for (int i = 0; i < N; ++i)
        ar[i] = new int[M];
    double time1, time2;


    omp_set_num_threads(thread);


    srand(time(0));
    Init(ar, vec, res);


    time1 = omp_get_wtime();
    MultByStr(ar, vec, res);
    time2 = omp_get_wtime();
    cout <<"Simple: "<< time2 - time1 << '\n';

    for (int j = 0; j < N; ++j)
        res[j] = 0;

    time1 = omp_get_wtime();
    ParallelMultByStr(ar, vec, res);
    time2 = omp_get_wtime();
    cout <<"By strings: "<< time2 - time1 << '\n';

    for (int j = 0; j < N; ++j)
        res[j] = 0;

    time1 = omp_get_wtime();
    ParallelMultByCol(ar, vec, res);
    time2 = omp_get_wtime();
    cout << "By collumns: " << time2 - time1 << '\n';


    for (int j = 0; j < N; ++j)
        res[j] = 0;


    time1 = omp_get_wtime();
    Mult(ar, vec, res);
    time2 = omp_get_wtime();
    cout << "By blocks:" <<time2 - time1 << '\n';



    return 0;
}