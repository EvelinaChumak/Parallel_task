#include <iostream>
#include <omp.h>
#include <string>
#include <time.h>
using namespace std;

int posPar(string& s, string& c)
{
	int lenS = s.length();
	int lenC = c.length();
	bool f = false;
	int num = -1;
#pragma omp parallel for
	for (int i = 0; i <= lenS - lenC; ++i)
	{
		string str = s.substr(i, lenC);
		if (str == c)
			num = i;
	}
	return num;
}

int pos(string& s, string& c)
{
	int lenS = s.length();
	int lenC = c.length();
	bool f = false;
	int num = -1;
	for (int i = 0; i <= lenS - lenC; ++i)
	{
		string str = s.substr(i, lenC);
		if (str == c)
			num = i;
	}
	return num;
}

int main()
{
	double time1, time2;
	omp_set_num_threads(4);
	srand(time(0));

	string s = "Hellow! What are you doing now? What is your name?";
	string c = "name";

	time1 = omp_get_wtime();
	int i = pos(s, c);
	time2 = omp_get_wtime();
	cout << "the number of occurrence of the substring of the simple algorithm and the time: " << i << ' ' << time2 - time1 << '\n';

	time1 = omp_get_wtime();
	i = posPar(s, c);
	time2 = omp_get_wtime();
	cout << "the number of occurrence of the substring of the parallel algorithm and the time: " << i << ' '<< time2 - time1 << '\n';

	return 0;
}