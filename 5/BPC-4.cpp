#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <cstdlib> 

using namespace std;


long long parallelSum(const vector<int>& array) {
    long long sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < array.size(); ++i) {
        sum += array[i];
    }

    return sum;
}


long long singleThreadSum(const vector<int>& array) {
    long long sum = 0;

    for (int i = 0; i < array.size(); ++i) {
        sum += array[i];
    }

    return sum;
}

int main() {
    const size_t size = 10000000; 
    vector<int> array(size);

    
    srand(static_cast<unsigned>(time(0))); 
    for (size_t i = 0; i < size; ++i) {
        array[i] = rand() % 100; 
    }
    omp_set_num_threads(4);
    
    auto start_time_parallel = chrono::high_resolution_clock::now();
    long long sum_parallel = parallelSum(array);
    auto end_time_parallel = chrono::high_resolution_clock::now();
    chrono::duration<double> time_parallel = end_time_parallel - start_time_parallel;

    
    auto start_time_single = chrono::high_resolution_clock::now();
    long long sum_single = singleThreadSum(array);
    auto end_time_single = chrono::high_resolution_clock::now();
    chrono::duration<double> time_single = end_time_single - start_time_single;

    
    cout << "Parallel sum: " << sum_parallel << endl;
    cout << "Single-thread sum: " << sum_single << endl;
    cout << "Time parallel: " << time_parallel.count() << " seconds" << endl;
    cout << "Time single-thread: " << time_single.count() << " seconds" << endl;

    
    if (sum_parallel == sum_single) {
        cout << "Results are correct!" << endl;
    }
    else {
        cout << "Results are incorrect!" << endl;
    }

    return 0;
}