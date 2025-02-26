#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

using namespace std;


double parallelRectangleIntegration(double a, double b, int n) {
    double h = (b - a) / n; 
    double sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        double x = a + (i + 0.5) * h; 
        sum += sin(x); 
    }

    return sum * h; 
}


double singleThreadRectangleIntegration(double a, double b, int n) {
    double h = (b - a) / n; 
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        double x = a + (i + 0.5) * h; 
        sum += sin(x); 
    }

    return sum * h; 
}

int main() {
    double a = 0.0; 
    double b = 3.1415; 
    int n = 10000000; 

    omp_set_num_threads(4);
    auto start_time_parallel = chrono::high_resolution_clock::now();
    double integral_parallel = parallelRectangleIntegration(a, b, n);
    auto end_time_parallel = chrono::high_resolution_clock::now();
    chrono::duration<double> time_parallel = end_time_parallel - start_time_parallel;

    
    auto start_time_single = chrono::high_resolution_clock::now();
    double integral_single = singleThreadRectangleIntegration(a, b, n);
    auto end_time_single = chrono::high_resolution_clock::now();
    chrono::duration<double> time_single = end_time_single - start_time_single;


    double analytical_solution = 2.0; 

   
    cout << "Parallel integral: " << integral_parallel << endl;
    cout << "Single-thread integral: " << integral_single << endl;
    cout << "Analytical solution: " << analytical_solution << endl;
    cout << "Time parallel: " << time_parallel.count() << " seconds" << endl;
    cout << "Time single-thread: " << time_single.count() << " seconds" << endl;

    
    double epsilon = 1e-6; 
    if (abs(integral_parallel - analytical_solution) < epsilon && abs(integral_single - analytical_solution) < epsilon) {
        cout << "Results are correct!" << endl;
    }
    else {
        cout << "Results are incorrect!" << endl;
    }

    return 0;
}