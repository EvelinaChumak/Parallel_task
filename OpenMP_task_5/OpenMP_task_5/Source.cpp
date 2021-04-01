#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;

const int Size = 2000, var = 200, threads = 2;

void init(int* ar)
{
	for (int i = 0; i < Size; ++i)
		ar[i] = rand() % var + 1;

}

void SimpleSort(int* ar)
{
	bool sorted = false;
	int init = 0;
	while (!sorted or init != 0)
	{
		sorted = true;
		for (int j = init; j < Size - 1; j+=2)
			if (ar[j] > ar[j + 1])
			{
				swap(ar[j], ar[j + 1]);
				sorted = false;
			}
		if (init == 0)
			init = 1;
		else init = 0;
	}
}

void ParallelSort(int* ar)
{
	bool sorted = false;
	int init = 0;
	while (!sorted or init != 0)
	{
		sorted = true;
#pragma omp parallel for
		for (int j = init; j < Size - 1; j += 2)
			if (ar[j] > ar[j + 1])
			{
				swap(ar[j], ar[j + 1]);
				sorted = false;
			}
		if (init == 0)
			init = 1;
		else init = 0;
	}
}

int main()
{
	omp_set_num_threads(threads);
	srand(time(0));

	double time = 0;
	double time1, time2;

	int* ar = new int[Size];

	for (int i = 0; i < 10; ++i)
	{
		init(ar);
		time1 = omp_get_wtime();
		SimpleSort(ar);
		time2 = omp_get_wtime();
		time += time2 - time1;
	}
	cout << time / 10 << endl;

	time = 0;
	for (int i = 0; i < 10; ++i)
	{
		init(ar);
		time1 = omp_get_wtime();
		ParallelSort(ar);
		time2 = omp_get_wtime();
		time += time2 - time1;
	}
	cout << time / 10 << endl;

	return 0;
}