#include <iostream>
#include <omp.h>
using namespace std;


const double a = 0.0, b = 1.0;
const double h = 0.1;

double f(double x)
{
    return (4 / (1 + x * x));
}

double IntSimp(int n)
{
    double Integral = 0.0;
    for (int i = 1; i <= n; i++)
        Integral = Integral + h * f(a + h * (i - 0.5));
    return Integral;
}

double IntParall(int n)
{
    double Integral = 0.0;
#pragma openmp parallel for 
    for (int i = 1; i <= n; i++)
        Integral = Integral + h * f(a + h * (i - 0.5));
    return Integral;
}

double IntReduction(int n)
{
    double Integral = 0.0;
#pragma openmp parallel for reduction(+:Integral)

    for (int i = 1; i <= n; i++)
        Integral = Integral + h * f(a + h * (i - 0.5));
    return Integral;
}
int main()
{

    double n;
    n = (b - a) / h;
    
    int I = 0;

    double time1, time2;

    time1 = omp_get_wtime();
    I = IntSimp(n);
    time2 = omp_get_wtime();
    cout << "Simple: " << IntSimp(n) << " Time: " << time2 - time1 << endl;

    time1 = omp_get_wtime();
    I = IntParall(n);
    time2 = omp_get_wtime();
    cout << "Parallel: " << IntSimp(n) << " Time: " << time2 - time1 << endl;

    time1 = omp_get_wtime();
    I = IntReduction(n);
    time2 = omp_get_wtime();
    cout << "Reduction: " << IntSimp(n) << " Time: " << time2 - time1 << endl;

    return 0;
}
