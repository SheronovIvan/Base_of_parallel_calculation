#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

vector<int> multiplyMatrixVectorSingle(const vector<vector<int>>& matrix, const vector<int>& vec) {
    int n = matrix.size();
    int m = matrix[0].size();
    vector<int> result(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}

vector<int> multiplyMatrixVectorParallel(const vector<vector<int>>& matrix, const vector<int>& vec) {
    int n = matrix.size();
    int m = matrix[0].size();
    vector<int> result(n, 0);

#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}

int main() {
    int n = 10000, m = 10000; 

    omp_set_num_threads(8); 
    vector<vector<int>> matrix(n, vector<int>(m, 1));
    vector<int> vec(m, 1);

    auto start_single = chrono::high_resolution_clock::now();
    vector<int> result_single = multiplyMatrixVectorSingle(matrix, vec);
    auto end_single = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_single = end_single - start_single;

    auto start_parallel = chrono::high_resolution_clock::now();
    vector<int> result_parallel = multiplyMatrixVectorParallel(matrix, vec);
    auto end_parallel = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_parallel = end_parallel - start_parallel;


    cout << "Single-threaded execution time: " << duration_single.count() << " seconds" << endl;
    cout << "Parallel execution time: " << duration_parallel.count() << " seconds" << endl;

    return 0;
}
