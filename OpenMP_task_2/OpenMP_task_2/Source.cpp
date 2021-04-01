#include <iostream> 
#include <omp.h>
#include <time.h> 

using namespace std;

const int N = 1000, M = 1000, var = 200;

int FuncParal(int** ar)
{
    int max = 0;
    int min = var;
    int i, j;
#pragma omp parallel for shared(ar) private(i,j, min)
    for (i = 0; i < M; ++i)
    {
        min = var;
        for (j = 0; j < N; ++j)
            if (ar[j][i] < min)
                min = ar[j][i];

        if (max < min)
            max = min;
    }
    return max;
}

int Func(int** ar)
{
    int max = 0;
    int min = var;
    int i, j;
    for (i = 0; i < M; ++i)
    {
        min = var;
        for (j = 0; j < N; ++j)
            if (ar[j][i] < min)
                min = ar[j][i];

        if (max < min)
            max = min;
    }
    return max;
}
int main() {
    int** ar;
    srand(time(0));

    int threadsNum = 4;
    omp_set_num_threads(threadsNum);

    int max = 0;
    ar = new int* [N];
    for (int i = 0; i < N; ++i)
        ar[i] = new int[M];

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            ar[i][j] = rand() % var + 1;
        }
    }

    double time1, time2;

    time1 = omp_get_wtime();
    max = FuncParal(ar);
    time2 = omp_get_wtime();
    cout << "Parallel: " << time2 - time1 << '\n';
    cout << "The maximum element of the minimum elements of the columns of the matrix: " << max << '\n';

    time1 = omp_get_wtime();
    max = Func(ar);
    time2 = omp_get_wtime();
    cout << "Simple: " << time2 - time1 << '\n';
    cout << "The maximum element of the minimum elements of the columns of the matrix: " << max;
    return 0;
}